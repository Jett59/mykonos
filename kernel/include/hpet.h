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
#ifndef _HPET_H
#define _HPET_H

#include <stddef.h>
#include <stdint.h>

#include <kmalloc.h>

#include <mmio.h>

namespace hpet {
class Hpet {
public:
  Hpet(void *physicalAddress)
      : registerPointer(
            (uint64_t *)memory::mapAddress(physicalAddress, 1024, false)) {}
  ~Hpet() { memory::unmapMemory(registerPointer, 1024); }

private:
  uint64_t *registerPointer;
  
  void writeRegister(size_t offset, uint64_t value) {
    mmio::write(registerPointer + (offset / 8), value);
  }
  uint64_t readRegister(size_t offset) {
    return mmio::read(registerPointer + (offset / 8));
  }
};
} // namespace hpet

#endif