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
#ifndef _MYKONOS_PHYSICAL_MEMORY_H
#define _MYKONOS_PHYSICAL_MEMORY_H

#include <mykonos/memoryBlock.h>
#include <mykonos/pageConstants.h>

#include <stddef.h>

namespace memory {
extern BlockMap physicalMemory;
static inline size_t allocateFrame() {
  return (size_t)physicalMemory.allocate(PAGE_SIZE) / PAGE_SIZE;
}
static inline void returnFrame(size_t frameIndex) {
  physicalMemory.returnMemory((void*)(frameIndex * PAGE_SIZE), PAGE_SIZE);
}
}  // namespace memory

#endif