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
#include <cpuExceptions.h>
#include <kout.h>
#include <kpanic.h>

#include <stdint.h>

extern "C" void handleCpuException(uint64_t exceptionNumber,
                                   uint64_t errorCode) {
  switch (exceptionNumber) {
  case CPU_DE: {
    kout::print("Division exception\n");
    break;
  }
  case CPU_DB: {
    kout::print("Debug trap\n");
    break;
  }
  case CPU_NMI: {
    kout::print("Non-maskable interrupt\n");
    break;
  }
  case CPU_BP: {
    kout::print("Breakpoint\n");
    break;
  }
  case CPU_OF: {
    kout::print("Overflow exception\n");
    break;
  }
  case CPU_BR: {
    kout::print("Bound range exception\n");
    break;
  }
  case CPU_UD: {
    kout::print("Invalid opcode\n");
    break;
  }
  case CPU_NM: {
    kout::print("Device not available\n");
    break;
  }
  case CPU_DF: {
    kout::print("Double fault\n");
    break;
  }
  case CPU_TS: {
    kout::print("Invalid TSS\n");
    break;
  }
  case CPU_NP: {
    kout::print("Segment not present: ");
    kout::print(errorCode);
    kout::print("\n");
    break;
  }
  case CPU_SS: {
    kout::print("Stack exception\n");
    break;
  }
  case CPU_GP: {
    kout::print("General protection fault (0x");
    kout::print(errorCode, 16);
    kout::print(")\n");
    break;
  }
  case CPU_PF: {
    kout::print("Page fault (");
    kout::print(errorCode, 2);
    kout::print(") on address ");
    void *cr2Address;
    __asm__ volatile("movq %%cr2, %0" : "=a"(cr2Address));
    kout::print((unsigned long)cr2Address, 16);
    kout::print("\n");
    break;
  }
  case CPU_MF: {
    kout::print("X87 error\n");
    break;
  }
  case CPU_AC: {
    kout::print("Alignment check exception\n");
    break;
  }
  case CPU_MC: {
    kout::print("Machine check exception\n");
    break;
  }
  case CPU_XF: {
    kout::print("SSE exception\n");
    break;
  }
  case CPU_CP: {
    kout::print("Control transfer exception\n");
    break;
  }
  case CPU_HV: {
    kout::print("Hyper visor exception\n");
    break;
  }
  case CPU_VC: {
    kout::print("VMM communication exception\n");
    break;
  }
  case CPU_SX: {
    kout::print("Security exception\n");
    break;
  }
  default:
    kout::print("Unimplemented exception number\n");
    break;
  }
  kpanic("Non recoverable exception caught\n");
}
