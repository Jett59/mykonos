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
#ifndef _MYKONOS_CPU_EXCEPTIONS_H
#define _MYKONOS_CPU_EXCEPTIONS_H

#define CPU_DE 0  /*Division exception*/
#define CPU_DB 1  /*Debug*/
#define CPU_NMI 2 /*Non-Maskable Interrupt*/
#define CPU_BP 3  /*Breakpoint*/
#define CPU_OF 4  /*Overflow exception*/
#define CPU_BR 5  /*Bound Range exception*/
#define CPU_UD 6  /*Invalid opcode*/
#define CPU_NM 7  /*Device not available*/
#define CPU_DF 8  /*Double fault*/
#define CPU_TS 10 /*Invalid TSS*/
#define CPU_NP 11 /*Segment Not Present*/
#define CPU_SS 12 /*Stack exception*/
#define CPU_GP 13 /*General Protection Fault*/
#define CPU_PF 14 /*Page Fault*/
#define CPU_MF 16 /*X87 FPU exception*/
#define CPU_AC 17 /*Alignment Check exception*/
#define CPU_MC 18 /*Machine Check*/
#define CPU_XF 19 /*SSE exception*/
#define CPU_CP 21 /*Control transfer exception*/
#define CPU_HV 28 /*Hyper Visor injection exception*/
#define CPU_VC 29 /*VMM Communication exception*/
#define CPU_SX 30 /*Security exception*/

#define CPU_UNKNOWN -1 /*Unknown/invalid exception*/

#endif