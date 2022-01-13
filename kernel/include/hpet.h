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

#include <cpu.h>
#include <mmio.h>

#define HPET_REGISTER_COUNTER 0xf0

namespace hpet {
class Hpet {
public:
  Hpet(void *physicalAddress);
  ~Hpet();

  Hpet(Hpet &other) = delete;
  Hpet &operator=(Hpet &other) = delete;

  uint64_t nanoTime() {
    return (readRegister(HPET_REGISTER_COUNTER) * frequencyFemtos) / 1000000;
  }
  uint64_t getFrequencyKhz() { return 1000000000000l / frequencyFemtos; }

  void reset();

  void wait(uint64_t nanos) {
    uint64_t currentNanos = nanoTime();
    while (nanoTime() < currentNanos + nanos) {
      cpu::relax();
    }
  }

private:
  uint64_t *registerPointer;

  uint64_t frequencyFemtos;

  void writeRegister(size_t offset, uint64_t value) {
    mmio::write(registerPointer + (offset / 8), value);
  }
  uint64_t readRegister(size_t offset) {
    return mmio::read(registerPointer + (offset / 8));
  }
};
} // namespace hpet

#endif