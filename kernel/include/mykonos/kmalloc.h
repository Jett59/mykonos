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
#ifndef _MYKONOS_KMALLOC_H
#define _MYKONOS_KMALLOC_H

#include <stddef.h>

namespace memory {
// We keep track of the size of the memory for you
void *kmalloc(size_t size);
void kfree(void *ptr);

void *mapAddress(void *physicalAddress, size_t size, bool cacheable);
void unmapMemory(void *address, size_t size);

// You keep track of the size of the memory for yourself
void *allocateMemory(size_t size);
static inline void freeMemory(void *ptr, size_t size) {
  unmapMemory(ptr, size);
}
} // namespace memory

#endif