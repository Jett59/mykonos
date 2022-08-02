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
#include <mykonos/idt.h>
#include <mykonos/interrupts.h>

typedef void (*InterruptHandlerFunction)();

extern "C" {
extern InterruptHandlerFunction idtFunctions[256];
}

namespace interrupts {
static IdtEntry idt[256];
static IdtPointer idtPointer;
void init() {
  for (int i = 0; i < 256; i++) {
    idt[i] = IdtEntry(
        idtFunctions[i], 0,
        i < 32);  // First 32 are traps (exceptions), the rest are interrupts
  }
  idtPointer.pointer = idt;
  idtPointer.limit = sizeof(idt) - 1;
}
void install() {
  __asm__ volatile("lidt %0" : : "m"(idtPointer) : "memory");
}
}  // namespace interrupts
