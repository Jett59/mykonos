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
#ifndef _IDT_H
#define _IDT_H

#include <stdint.h>

namespace interrupts {
struct IdtEntry {
  uint16_t offsetLow; // 15:0
  uint16_t gdtSelector;
  uint8_t istEntry; // 3-bit interrupt stack table number
  uint8_t type : 4; // 0xE = interrupt gate, 0xF = trap gate
  uint8_t zero : 1; // Must be zero
  uint8_t privilege : 2;
  uint8_t present : 1;
  uint16_t offsetMiddle; // 31:16
  uint32_t offsetHigh;   // 63:32
  uint32_t reserved;

  IdtEntry() {}
  IdtEntry(void (*function)(), int istNum, bool trap)
      : gdtSelector(8), istEntry(istNum), type(trap ? 0xF : 0xE), present(1) {
    uint64_t functionPointer = (uint64_t)function;
    offsetLow = functionPointer & 0xFFFF;
    functionPointer >>= 16;
    offsetMiddle = functionPointer & 0xFFFF;
    functionPointer >>= 16;
    offsetHigh = functionPointer;
  }
} __attribute__((packed));
static_assert(sizeof(IdtEntry) == 16, "IdtEntry must be 16 bytes");

struct IdtPointer {
  uint16_t limit; // size - 1
  IdtEntry *pointer;
} __attribute__((packed));
static_assert(sizeof(IdtPointer) == 10, "IdtPointer must be 10 bytes");
} // namespace interrupts

#endif