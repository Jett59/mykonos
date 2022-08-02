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
#ifndef _MYKONOS_SPINLOCK_H
#define _MYKONOS_SPINLOCK_H

#include <stdint.h>

#include <mykonos/cpu.h>

namespace lock {
class Spinlock {
 private:
  uint8_t lock = 0;
  bool enableIrqs = false;

 public:
  Spinlock() = default;
  // We don't want spinlocks to be copied
  Spinlock(const Spinlock&) = delete;
  Spinlock& operator=(const Spinlock&) = delete;

  // We do want move capabilities.
  Spinlock(Spinlock&& other) {
    lock = other.lock;
    enableIrqs = other.enableIrqs;
    other.lock = 1;
    other.enableIrqs = false;
  }
  Spinlock& operator=(Spinlock&& other) {
    lock = other.lock;
    enableIrqs = other.enableIrqs;
    other.lock = 1;
    other.enableIrqs = false;
    return *this;
  }

  bool locked() { return __atomic_load_n(&lock, __ATOMIC_SEQ_CST) != 0; }
  void acquire() {
    bool oldIrqState = cpu::localIrqState();
    cpu::disableLocalIrqs();
    while (true) {
      while (locked()) {
        cpu::relax();
      }
      if (__sync_bool_compare_and_swap(&lock, 0, 1)) {
        break;
      }
    }
    enableIrqs = oldIrqState;
  }
  void release() {
    bool shouldReenableIrqs = enableIrqs;
    __atomic_store_n(&lock, 0, __ATOMIC_SEQ_CST);
    if (shouldReenableIrqs) {
      cpu::enableLocalIrqs();
    }
  }
};
}  // namespace lock

#endif