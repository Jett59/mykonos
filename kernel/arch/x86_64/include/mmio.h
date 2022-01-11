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
#ifndef _MMIO_H
#define _MMIO_H

#include <stdint.h>

#include <cpu.h>

namespace mmio {
template <typename T> static inline void write(T *ptr, T value) {
  cpu::mfence();
  *ptr = value;
}
template <typename T> static inline T read(T *ptr) {
  cpu::mfence();
  return *ptr;
}
} // namespace mmio

#endif