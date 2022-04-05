/*
    Copyright (C) 2022  Jett Thompson

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
#define RAX_OFFSET 0
#define RCX_OFFSET 8
#define RDX_OFFSET 16
#define RBX_OFFSET 24
#define RSP_OFFSET 32
#define RBP_OFFSET 40
#define RSI_OFFSET 48
#define RDI_OFFSET 56
#define R8_OFFSET 64
#define R9_OFFSET 72
#define R10_OFFSET 80
#define R11_OFFSET 88
#define R12_OFFSET 96
#define R13_OFFSET 104
#define R14_OFFSET 112
#define R15_OFFSET 120
#define CR3_OFFSET 128
#define RIP_OFFSET 136
#define RFLAGS_OFFSET 144

.text
.globl swapRegisters
swapRegisters:
/*Save old registers*/
/*get rip and rflags*/
popq RIP_OFFSET(%rdi)
pushfq
popq RFLAGS_OFFSET(%rdi)

/*Save the GPRs*/
movq %rax, RAX_OFFSET(%rdi)
movq %rcx, RCX_OFFSET(%rdi)
movq %rdx, RDX_OFFSET(%rdi)
movq %rbx, RBX_OFFSET(%rdi)
movq %rsp, RSP_OFFSET(%rdi)
movq %rbp, RBP_OFFSET(%rdi)
movq %rsi, RSI_OFFSET(%rdi)
movq %rdi, RDI_OFFSET(%rdi)
movq %r8, R8_OFFSET(%rdi)
movq %r9, R9_OFFSET(%rdi)
movq %r10, R10_OFFSET(%rdi)
movq %r11, R11_OFFSET(%rdi)
movq %r12, R12_OFFSET(%rdi)
movq %r13, R13_OFFSET(%rdi)
movq %r14, R14_OFFSET(%rdi)
movq %r15, R15_OFFSET(%rdi)
/*Save cr3*/
movq %cr3, %rax
movq %rax, CR3_OFFSET(%rdi)
/*Unlock the task*/
/*Save to into %rbx*/
movq %rsi, %rbx
/*%rdi already contains the old task*/
call unlockTask
/*Load new registers*/
movq %rbx, %rdi
jmp restoreRegisters

.globl restoreRegisters
restoreRegisters:
/*Lock the task*/
movq %rdi, %rbx
call lockTask
movq %rbx, %rdi

movq CR3_OFFSET(%rdi), %rdx
cmp %rax, %rdx
je 1f
movq %rdx, %cr3
1:
movq RAX_OFFSET(%rdi), %rax
movq RCX_OFFSET(%rdi), %rcx
movq RDX_OFFSET(%rdi), %rdx
movq RBX_OFFSET(%rdi), %rbx
movq RSP_OFFSET(%rdi), %rsp
movq RBP_OFFSET(%rdi), %rbp
movq RSI_OFFSET(%rdi), %rsi
/*Don't movq RDI_OFFSET(%rdi), %rdi to not lose our pointer*/
movq R8_OFFSET(%rdi), %r8
movq R9_OFFSET(%rdi), %r9
movq R10_OFFSET(%rdi), %r10
movq R11_OFFSET(%rdi), %r11
movq R12_OFFSET(%rdi), %r12
movq R13_OFFSET(%rdi), %r13
movq R14_OFFSET(%rdi), %r14
movq R15_OFFSET(%rdi), %r15
/*Get rip and rflags*/
pushq RIP_OFFSET(%rdi)
pushq RFLAGS_OFFSET(%rdi)
/*Reload %rdi and return*/
movq RDI_OFFSET(%rdi), %rdi
popfq
retq
