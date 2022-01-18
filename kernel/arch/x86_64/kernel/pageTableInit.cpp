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
#include <mykonos/pageConstants.h>
#include <mykonos/pageTableInit.h>
#include <mykonos/pageTables.h>

extern "C" {
void *kernelVirtualOffset[0];
}

#define PHYSICAL_ADDRESS(PTR) ((uint64_t)PTR - (uint64_t)kernelVirtualOffset)

namespace paging {
extern "C" {
static PageTableEntry pml1[512] __attribute__((aligned(4096)));
static PageTableEntry pml2[512] __attribute__((aligned(4096)));
static PageTableEntry pml3[512] __attribute__((aligned(4096)));
// Globally accessible for use in multiprocessor initialization
PageTableEntry pml4[512] __attribute__((aligned(4096)));
}
void initPageTables() {
  // Set up the mappings
  pml4[256] = (PageTableEntry)PHYSICAL_ADDRESS(pml3) | PageTableFlags::PRESENT |
              PageTableFlags::WRITABLE;
  pml3[0] = (PageTableEntry)PHYSICAL_ADDRESS(pml2) | PageTableFlags::PRESENT |
            PageTableFlags::WRITABLE;
  pml2[1] = (PageTableEntry)PHYSICAL_ADDRESS(pml1) | PageTableFlags::PRESENT |
            PageTableFlags::WRITABLE;
  for (unsigned i = 0; i < 512; i++) {
    uint64_t address = i * 4096 + 0x200000;
    pml1[i] = (PageTableEntry)address | PageTableFlags::PRESENT |
              PageTableFlags::WRITABLE;
  }
  // Set up the recursive mapping
  pml4[511] = (PageTableEntry)PHYSICAL_ADDRESS(pml4) | PageTableFlags::PRESENT |
              PageTableFlags::WRITABLE;
  // Load it
  __asm__ volatile("mov %0, %%cr3" : : "a"(pml4[511]) : "memory");
}
} // namespace paging
