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
#include <mykonos/kmalloc.h>
#include <mykonos/kpanic.h>
#include <mykonos/pageConstants.h>
#include <mykonos/paging.h>
#include <mykonos/physicalMemory.h>
#include <mykonos/processors.h>
#include <mykonos/virtualMemory.h>

#include <stdint.h>

#define ADD_TO_POINTER(PTR, AMOUNT) ((void *)((uint8_t *)PTR + AMOUNT))

namespace memory {
struct KmallocHeader {
  size_t size;
};
void *allocateMemory(size_t size) {
  size = PAGE_ALIGN_UP(size + sizeof(KmallocHeader));
  void *ptr = virtualMemory.allocate(size);
  if (ptr == nullptr) {
    return nullptr;
  }
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    size_t frame = memory::allocateFrame();
    if (frame != 0) {
      paging::mapPage(ADD_TO_POINTER(ptr, i), (void *)(frame * PAGE_SIZE),
                      paging::PageTableFlags::WRITABLE, true, true);
    } else {
      kpanic("Out of memory");
    }
  }
  return ptr;
}
void *kmalloc(size_t size) {
  void *ptr = allocateMemory(size);
  if (ptr == nullptr) {
    return nullptr;
  }
  KmallocHeader *headerPtr = (KmallocHeader *)ptr;
  *headerPtr = {.size = size};
  ptr = (void *)(headerPtr + 1);
  return ptr;
}
void kfree(void *ptr) {
  if (ptr == nullptr) {
    kpanic("Attempt to free null");
  }
  KmallocHeader *headerPtr = (KmallocHeader *)ptr - 1;
  size_t size = headerPtr->size;
  ptr = (void *)headerPtr;
  unmapMemory(ptr, size);
}
void *mapAddress(void *physicalAddress, size_t size, bool cacheable) {
  size_t pageOffset = (size_t)physicalAddress % PAGE_SIZE;
  void *physicalEnd = (void *)PAGE_ALIGN_UP((size_t)physicalAddress + size);
  physicalAddress = (void *)PAGE_ALIGN_DOWN((size_t)physicalAddress);
  size = (size_t)((size_t)physicalEnd - (size_t)physicalAddress);
  void *ptr = virtualMemory.allocate(size);
  if (ptr == nullptr) {
    return nullptr;
  }
  for (size_t i = 0; i < size; i += PAGE_SIZE) {
    paging::mapPage(ADD_TO_POINTER(ptr, i), ADD_TO_POINTER(physicalAddress, i),
                    paging::PageTableFlags::WRITABLE, false, cacheable);
  }
  return ADD_TO_POINTER(ptr, pageOffset);
}
void unmapMemory(void *address, size_t size) {
  if (address != nullptr) {
    void *endAddress = (void *)PAGE_ALIGN_UP((size_t)address + size);
    address = (void *)PAGE_ALIGN_DOWN((size_t)address);
    size = (size_t)endAddress - (size_t)address;
    for (size_t i = 0; i < size; i += PAGE_SIZE) {
      paging::unmapPage(ADD_TO_POINTER(address, i));
    }
    // TLB shootdown
    unsigned processorCount = processors::processorCount();
    for (unsigned i = 0; i < processorCount; i++) {
      if (i != cpu::getCpuNumber()) {
        auto tlbInvalidationCode = [=]() -> bool {
          for (size_t offset = 0; offset < size; offset += 4096) {
            paging::invalidateTlb(ADD_TO_POINTER(address, offset));
          }
          processors::letCallerReturn();
          return true;
        };
        processors::runOn(i,
                          callback::Lambda<decltype(tlbInvalidationCode), bool>(
                              tlbInvalidationCode));
      }
    }
    virtualMemory.returnMemory(address, size);
  } else {
    kpanic("Attempt to unmap null");
  }
}
} // namespace memory

void *operator new(size_t size) { return memory::kmalloc(size); }
void *operator new[](size_t size) { return memory::kmalloc(size); }

void operator delete(void *ptr) { memory::kfree(ptr); }
void operator delete[](void *ptr) { memory::kfree(ptr); }

void operator delete(void *ptr, size_t) { memory::kfree(ptr); }
void operator delete[](void *ptr, size_t) { memory::kfree(ptr); }
