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

.macro errorCodeExceptionHandler number
.section .text.interruptHandlers, "ax"
1:
pushq $\number
jmp genericExceptionHandler
.section .rodata.interruptHandlers, "a"
.quad 1b
.endm

.macro noErrorCodeExceptionHandler number
.section .text.interruptHandlers, "ax"
1:
pushq $0 /*Fake exception code*/
pushq $\number
jmp genericExceptionHandler
.section .rodata.interruptHandlers, "a"
.quad 1b
.endm

.macro reservedExceptions count=1
.rep \count
errorCodeExceptionHandler -1
.endr
.endm

.section .rodata.interruptHandlers, "a"
.globl idtFunctions
idtFunctions:

noErrorCodeExceptionHandler 0 /*#DE: divide by 0*/
noErrorCodeExceptionHandler 1 /*#DB: debug*/
noErrorCodeExceptionHandler 2 /*#NMI: Non-maskable interrupt*/
noErrorCodeExceptionHandler 3 /*#BP: Breakpoint*/
noErrorCodeExceptionHandler 4 /*#OF: Overflow*/
noErrorCodeExceptionHandler 5 /*#BR: Bound range exceeded*/
noErrorCodeExceptionHandler 6 /*#UD: Invalid opcode*/
noErrorCodeExceptionHandler 7 /*#NM: Device not available*/
errorCodeExceptionHandler 8 /*#DF: Double fault*/
reservedExceptions 1
errorCodeExceptionHandler 10 /*#TS: Invalid tss*/
errorCodeExceptionHandler 11 /*#NP: Segment not present*/
errorCodeExceptionHandler 12 /*#SS: Stack exception*/
errorCodeExceptionHandler 13 /*#GP: General protection fault*/
errorCodeExceptionHandler 14 /*#PF: Page fault*/
reservedExceptions 1
noErrorCodeExceptionHandler 16 /*#MF: X87 FPU exception*/
errorCodeExceptionHandler 17 /*#AC: Alignment check exception*/
noErrorCodeExceptionHandler 18 /*#MC: Machine check*/
noErrorCodeExceptionHandler 19 /*#XF: SSE exception*/
reservedExceptions 1
errorCodeExceptionHandler 21 /*#CP: Control transfer exception*/
reservedExceptions 7
noErrorCodeExceptionHandler 28 /*#HV: Hypervisor injection exception*/
errorCodeExceptionHandler 29 /*#VC: VMM communication error*/
errorCodeExceptionHandler 30 /*#SX: Security exception*/
reservedExceptions 1

currentInterruptNumber = 32
.rept 255 - 32 /*Number of real (non-exception) interrupts*/
.section .text.interruptHandlers, "ax"
1:
pushq $currentInterruptNumber
jmp genericInterruptHandler

.section .rodata.interruptHandlers, "a"
.quad 1b

currentInterruptNumber = currentInterruptNumber + 1
.endr

.section .text.interruptHandlers, "ax"
genericInterruptHandler:
addq $8, %rsp
iretq
genericExceptionHandler:
pushq %rbp
movq %rsp, %rbp
pushq %rax
pushq %rcx
pushq %rdx
pushq %rdi
pushq %rsi
pushq %r8
pushq %r9
pushq %r10
pushq %r11
movq 8(%rbp), %rdi /*Exception number*/
movq 16(%rbp), %rsi /*Error code*/
leaq 24(%rbp), %rdx /*Interrupt stack frame*/
callq handleCpuException
popq %r11
popq %r10
popq %r9
popq %r8
popq %rsi
popq %rdi
popq %rdx
popq %rcx
popq %rax
movq %rbp, %rsp
popq %rbp
subq $16, %rsp /*Remove exception number and error code*/
iretq
