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
  bool locked() { return __atomic_load_n(&lock, __ATOMIC_SEQ_CST); }
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
  void release() { __atomic_store_n(&lock, 0, __ATOMIC_SEQ_CST); }
};
} // namespace lock

#endif