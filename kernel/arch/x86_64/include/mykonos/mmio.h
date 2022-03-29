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
#ifndef _MYKONOS_MMIO_H
#define _MYKONOS_MMIO_H

#include <stdint.h>

#include <mykonos/cpu.h>

// These functions are the only correct way of writing to Memory-Mapped I/O.
namespace mmio {
template <typename T> static inline void write(volatile T *ptr, T value) {
  cpu::mfence();
  *ptr = value;
}
template <typename T> static inline T read(volatile T *ptr) {
  cpu::mfence();
  return *ptr;
}
} // namespace mmio

#endif