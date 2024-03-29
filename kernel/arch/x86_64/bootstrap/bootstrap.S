/*
    Copyright (C) 2021  Jett Thompson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
    */
    /*Handle going into 64-bit mode*/

#include <mykonos/gdtbits.h>

.code32

#define MULTIBOOT2_EAX_MAGIC 0x36d76289

    .section .bootstrap, "ax"
    bootstrap_error:
movl $0x100000, %ecx
1: decl %ecx
jnz 1b
int $0 /*Tripple fault*/
    .globl _start
    .type _start, @function
_start:
check_multiboot:
cmpl $MULTIBOOT2_EAX_MAGIC, %eax
jne bootstrap_error
movl %ebx, mbiPointer /*Thank you little-endian: this makes a 64 bit pointer for use later*/
check_long_mode:
movl $0x80000000, %eax
cpuid /*Highest cpuid number*/
cmpl $0x80000001, %eax
jb bootstrap_error /*Not high enough*/
movl $0x80000001, %eax
cpuid /*Extended feature flags*/
testl $(1 << 29), %edx
jz bootstrap_error /*Not supported*/
build_page_tables:
movl $initialPml3, %eax
orl $3, %eax /*present | writable*/
movl %eax, initialPml4
movl %eax, initialPml4 + 256 * 8 /*For higher half*/
movl $initialPml2, %eax
orl $3, %eax
movl %eax, initialPml3
/*Now set up pml2*/
movl $512, %ecx /*512 * 2mb = 1gb = amount mapped by pml2*/
movl $initialPml2, %edi
movl $0x83, %eax /*present | writable | large*/
1: movl %eax, (%edi)
addl $0x200000, %eax /*2mb = increment in pml2*/
addl $8, %edi /*Next entry*/
decl %ecx
jnz 1b /*next*/
/*We don't put in pml1s because we use large pages*/
movl $initialPml4, %eax
movl %eax, %cr3 /*Tell the CPU where it is*/
enable_long_mode:
/*PAE, LME and PG*/
/*PAE*/
movl %cr4, %eax
orl $(1 << 5), %eax /*PAE bit*/
movl %eax, %cr4 /*PAE enabled*/
/*LME*/
movl $0xC0000080, %ecx /*EFER (Extended Feature Enable Register)*/
rdmsr /*Get EFER register*/
orl $(1 << 8), %eax /*Set bit 8 (LME, Long Mode Enable)*/
wrmsr /*Enable long mode*/
/*PG*/
movl %cr0, %eax
orl $(1 << 31), %eax /*PG (paging) bit*/
movl %eax, %cr0 /*Paging enabled*/
/*Now we are in compatibility (32 bit) long mode*/
load_gdt:
movl $gdt_pointer, %esi
lgdt (%esi)
exit_compatibility_mode:
/*Into 64 bit mode*/
jmp $8, $begin_long_mode

.code64

begin_long_mode: /*We made it! Just a few more things to set up*/
/*Reload the gdt because it was only half loaded before*/
movq $gdt_pointer, %rsi
lgdt (%rsi)
/*Reset the segment registers to null*/
xorl %eax, %eax
movl %eax, %ds
movl %eax, %es
movl %eax, %fs
movl %eax, %gs
movl %eax, %ss
/*Now set up the stack*/
movabsq $initial_stack_top, %rsp
/*We actually made it! Let's go to C++*/
xorl %ebp, %ebp /*Call stack root*/
pushq $0 /*Fake return address*/
movabsq $kstart, %rax
jmp * %rax

.section .bootstrap-bss, "aw", @nobits
/*Page tables*/
.align 4096
initialPml4:
.fill 4096
initialPml3: /*(or pdp if you prefer)*/
.fill 4096
initialPml2: /*(or pde if you prefer)*/
.fill 4096

.globl mbiPointer
mbiPointer:
.quad 0 /*64 bit pointer*/

.section .bootstrap-rodata, "a"

gdt_pointer:
.word gdt_end - gdt - 1 /*Size - 1*/
.quad gdt /*Pointer*/

.section .rodata, "a"

gdt:
.quad 0 /*The first descriptor must be null*/
.quad GDT_PRESENT | GDT_LONG | GDT_NOT_SYSTEM | GDT_EXECUTABLE /*Kernel code segment*/
.quad GDT_PRESENT | GDT_NOT_SYSTEM | GDT_WRITABLE /*syscall stack segment*/
.quad GDT_PRESENT | GDT_WRITABLE | GDT_NOT_SYSTEM | GDT_PRIVILEGE(3) /*User mode stack segment*/
.quad GDT_PRESENT | GDT_EXECUTABLE | GDT_LONG | GDT_NOT_SYSTEM | GDT_PRIVILEGE(3) /*User mode code segment*/
gdt_end:

.bss

initial_stack:
.fill 65536 /*Should be fine*/
initial_stack_top:

.section .smp-trampoline, "ax"
.globl smpTrampolineStart
smpTrampolineStart:
.code16
jmp $0, $0x8020

.align 8
smpGdtPointer:
.word gdt_end - gdt - 1 /*Size - 1*/
.quad gdt /*Pointer*/
smpIdtPointer:
.word 0 /*Zero limit causes tripple fault*/
.quad 0 /*Null pointer*/

.align 8
/*Set up the segment registers and load the idt*/
movw %cs, %ax
movw %ax, %ds
lidt (0x8012)

/*Load the page table*/
movl $initialPml4, %esi
movl %esi, %cr3

/*Enable PAE, LME, PG and PE*/
/*PAE*/
movl %cr4, %eax
orl $(1 << 5), %eax /*PAE bit*/
movl %eax, %cr4 /*PAE enabled*/
/*LME*/
movl $0xC0000080, %ecx /*EFER (Extended Feature Enable Register)*/
rdmsr /*Get EFER register*/
orl $(1 << 8), %eax /*Set bit 8 (LME, Long Mode Enable)*/
wrmsr /*Enable long mode*/
/*PG and PE*/
movl %cr0, %eax
orl $(1 << 31) | (1 << 0), %eax /*PG (paging) bit and PE (Protection Enable) bit*/
movl %eax, %cr0 /*Paging and protected mode enabled*/
/*Load the gdt and leave real mode*/
lgdt (0x8008)
jmp $8, $0x8080

.code64

.align 128
/*Reload the gdt (see begin_long_mode above)*/
lgdt (gdt_pointer)

/*Go to the higher half*/
movabsq $1f, %rdi
jmpq * %rdi
1:
/*Enable caching*/
movq %cr0, %rdx
andq $~(1 << 30), %rdx
movq %rdx, %cr0

/*Load the official page tables using the recursive mapping*/
movabsq $pml4, %rsi
movq 511 * 8(%rsi), %rax
movq %rax, %cr3

movabsq $runningCpus, %rdi
movl $1, %eax
lock xaddb %al, (%rdi) /*Tell the smp init code that all is well*/
/*Our CPU number is now in %rax*/

/*Set up the segment registers*/
xorl %edx, %edx
movw %dx, %ds
movw %dx, %es
movw %dx, %fs
movw %dx, %gs
movw %dx, %ss

/*Get our stack from the initialStackPointer table*/
movabsq $initialStackPointers, %rsi
movq (%rsi, %rax,8), %rsp

/*Set up a stack frame and go to c++*/
xorl %ebp, %ebp
movq %rax, %rdi
movabsq $kstartApCpu, %rax
pushq $0
jmpq * %rax
.globl smpTrampolineEnd
smpTrampolineEnd:
