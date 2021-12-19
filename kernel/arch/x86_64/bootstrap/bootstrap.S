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

.code32

#define MULTIBOOT2_EAX_MAGIC 0x36d76289

    .section .init
    .globl _start
_start:
movl $initial_stack_top, %esp
cmpl $MULTIBOOT2_EAX_MAGIC, %eax
jne no_multiboot
1: jmp 1b
no_multiboot:
movl $0x1000000, %ecx
1: decl %ecx
jnz 1b
int $0 /*Tripple fault*/

.section .init.bss, "aw", @nobits
initial_stack:
.fill 1024 /*Thats enough for now*/
initial_stack_top:
