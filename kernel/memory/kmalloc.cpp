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
#include <kmalloc.h>

#include <pageConstants.h>
#include <paging.h>
#include <physicalMemory.h>
#include <virtualMemory.h>

#include <stdint.h>

namespace memory {
void *kmalloc(size_t size) {
  size = PAGE_ALIGN_UP(size);
  void *ptr = memory::virtualMemory.allocate(size);
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::mapPage((void *)((uint8_t *)ptr + i),
                    (void*)(memory::allocateFrame() * PAGE_SIZE),
                    paging::PageTableFlags::WRITABLE, true);
  }
  return ptr;
}
void kfree(void *ptr) {
  size_t size = memory::virtualMemory.free(ptr);
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::unmapPage((void *)((uint8_t *)ptr + i));
  };
}
} // namespace memory
