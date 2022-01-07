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
#ifndef _SPINLOCK_H
#define _SPINLOCK_H

#include <stdint.h>

namespace lock {
class Spinlock {
private:
  uint8_t lock = 0;

public:
  bool locked() {
    bool result = lock != 0; // Atomic because it is on a natural boundry (uint8
                             // has no alignment requirements)
                             // Compiler memory fence
    __asm__ volatile("nop" : : : "memory");
    return result;
  }
  void acquire() {
    while (true) {
      // Wait for the lock to be unlocked
      while (locked())
        ;
      if (__sync_bool_compare_and_swap(&lock, 0, 1)) {
        break;
      }
    }
  }
};
} // namespace lock

#endif