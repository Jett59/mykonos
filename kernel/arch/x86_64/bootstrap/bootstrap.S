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

#include <gdtbits.h>

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
movl $pml3, %eax
orl $3, %eax /*present | writable*/
movl %eax, pml4
movl %eax, pml4 + 256 * 8 /*For higher half*/
movl $pml2, %eax
orl $3, %eax
movl %eax, pml3
/*Now set up pml2*/
movl $512, %ecx /*512 * 2mb = 1gb = amount mapped by pml2*/
movl $pml2, %edi
movl $0x83, %eax /*present | writable | large*/
1: movl %eax, (%edi)
addl $0x200000, %eax /*2mb = increment in pml2*/
addl $8, %edi /*Next entry*/
decl %ecx
jnz 1b /*next*/
/*We don't put in pml1s because we use large pages*/
movl $pml4, %eax
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
/*Now set up the stack*/
movabsq $initial_stack_top, %rsp
/*We actually made it! Let's go to C++*/
pushq $0 /*Fake return address*/
jmp kstart

.section .bootstrap-bss, "aw", @nobits
/*Page tables*/
.align 4096
pml4:
.fill 4096
pml3: /*(or pdp if you prefer)*/
.fill 4096
pml2: /*(or pde if you prefer)*/
.fill 4096

.globl mbiPointer
mbiPointer:
.quad 0 /*64 bit pointer*/

.section .bootstrap-rodata, "a"

gdt_pointer:
.word gdt_end - gdt - 1 /*Size - 1*/
.quad gdt /*Pointer*/

gdt:
.quad 0 /*The first descriptor must be null*/
.quad GDT_PRESENT | GDT_LONG | GDT_NOT_SYSTEM | GDT_EXECUTABLE /*Kernel code segment*/
.quad GDT_PRESENT | GDT_NOT_SYSTEM | GDT_WRITABLE /*syscall stack segment*/
.quad GDT_PRESENT | GDT_WRITABLE | GDT_NOT_SYSTEM | GDT_PRIVILEGE(3) /*User mode stack segment*/
.quad GDT_PRESENT | GDT_EXECUTABLE | GDT_LONG | GDT_NOT_SYSTEM | GDT_PRIVILEGE(3) /*User mode code segment*/
gdt_end:

.bss

initial_stack:
.fill 4096 /*Should be fine*/
initial_stack_top:
