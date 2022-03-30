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
#ifndef _MYKONOS_IO_PORTS_H
#define _MYKONOS_IO_PORTS_H

#include <stdint.h>

namespace io {
// These map to the in and out instructions.
static inline void writePort8(uint8_t value, uint16_t port) {
  __asm__ volatile("outb %0, %1" : : "a"(value), "nd"(port) : "memory");
}
static inline void writePort16(uint16_t value, uint16_t port) {
  __asm__ volatile("outw %0, %1" : : "a"(value), "nd"(port) : "memory");
}
static inline void writePort32(uint32_t value, uint16_t port) {
  __asm__ volatile("outl %0, %1" : : "a"(value), "nd"(port) : "memory");
}
static inline uint8_t readPort8(uint16_t port) {
  uint8_t value;
  __asm__ volatile("inb %1, %0" : "=a"(value) : "nd"(port) : "memory");
  return value;
}
static inline uint16_t readPort16(uint16_t port) {
  uint16_t value;
  __asm__ volatile("inw %1, %0" : "=a"(value) : "nd"(port) : "memory");
  return value;
}
static inline uint32_t readPort32(uint16_t port) {
  uint32_t value;
  __asm__ volatile("inl %1, %0" : "=a"(value) : "nd"(port) : "memory");
  return value;
}

static inline void delay() { writePort8(0, 0x80); }
} // namespace io

#endif