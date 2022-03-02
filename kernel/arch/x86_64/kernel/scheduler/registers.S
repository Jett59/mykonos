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
/*Save*/
/*get rip and rflags*/
popq RIP_OFFSET(%rdi)
pushfq
popq RFLAGS_OFFSET(%rdi)

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
movq %cr3, %rax
movq %rax, CR3_OFFSET(%rdi)
/*Load*/
movq CR3_OFFSET(%rsi), %rdx
cmp %rax, %rdx
je 1f
movq %rdx, %cr3
1:
movq RAX_OFFSET(%rsi), %rax
movq RCX_OFFSET(%rsi), %rcx
movq RDX_OFFSET(%rsi), %rdx
movq RBX_OFFSET(%rsi), %rbx
movq RSP_OFFSET(%rsi), %rsp
movq RBP_OFFSET(%rsi), %rbp
/*Don't movq RSI_OFFSET(%rsi), %rsi to not lose our pointer*/
movq RDI_OFFSET(%rsi), %rdi
movq R8_OFFSET(%rsi), %r8
movq R9_OFFSET(%rsi), %r9
movq R10_OFFSET(%rsi), %r10
movq R11_OFFSET(%rsi), %r11
movq R12_OFFSET(%rsi), %r12
movq R13_OFFSET(%rsi), %r13
movq R14_OFFSET(%rsi), %r14
movq R15_OFFSET(%rsi), %r15
/*Get rip and rflags*/
pushq RIP_OFFSET(%rsi)
pushq RFLAGS_OFFSET(%rsi)
/*Reload %rsi and return*/
movq RSI_OFFSET(%rsi), %rsi
popfq
retq
