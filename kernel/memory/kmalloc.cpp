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

#define ADD_TO_POINTER(PTR, AMOUNT) ((void *)((uint8_t *)PTR + AMOUNT))

namespace memory {
struct KmallocHeader {
  size_t size;
};
void *kmalloc(size_t size) {
  size = PAGE_ALIGN_UP(size);
  void *ptr = virtualMemory.allocate(size);
  if (ptr == nullptr) {
    return nullptr;
  }
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::mapPage(ADD_TO_POINTER(ptr, i),
                    (void *)(memory::allocateFrame() * PAGE_SIZE),
                    paging::PageTableFlags::WRITABLE, true);
  }
  KmallocHeader *headerPtr = (KmallocHeader *)ptr;
  *headerPtr = {.size = size};
  ptr = (void *)(headerPtr + 1);
  return ptr;
}
void kfree(void *ptr) {
  KmallocHeader *headerPtr = (KmallocHeader *)ptr - 1;
  size_t size = headerPtr->size;
  ptr = (void *)headerPtr;
  virtualMemory.returnMemory(ptr, size);
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::unmapPage(ADD_TO_POINTER(ptr, i));
  }
}
void *mapAddress(void *physicalAddress, size_t size) {
  void *physicalEnd = (void *)PAGE_ALIGN_UP((size_t)physicalAddress + size);
  physicalAddress = (void *)PAGE_ALIGN_DOWN((size_t)physicalAddress);
  size = (size_t)((size_t)physicalEnd - (size_t)physicalAddress);
  void *ptr = virtualMemory.allocate(size);
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::mapPage(ADD_TO_POINTER(ptr, i), ADD_TO_POINTER(physicalAddress, i),
                    paging::PageTableFlags::WRITABLE, false);
  }
  return ptr;
}
} // namespace memory
