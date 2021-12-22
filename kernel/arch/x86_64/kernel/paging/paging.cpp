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
#include <pageTables.h>
#include <paging.h>

#include <physicalMemory.h>

#include <string.h>

#define PML4TABLE ((*PAGE_TABLE_POINTER)[511][511][511])
#define PML3TABLE(PML4INDEX) ((*PAGE_TABLE_POINTER)[511][511][PML4INDEX])
#define PML2TABLE(PML4INDEX, PML3INDEX)                                        \
  ((*PAGE_TABLE_POINTER)[511][PML4INDEX][PML3INDEX])
#define PML1TABLE(PML4INDEX, PML3INDEX, PML2INDEX)                             \
  ((*PAGE_TABLE_POINTER)[PML4INDEX][PML3INDEX][PML2INDEX])

#define PML4ENTRY(PML4INDEX) (PML4TABLE[PML4INDEX])
#define PML3ENTRY(PML4INDEX, PML3INDEX) (PML3TABLE(PML4INDEX)[PML3INDEX])
#define PML2ENTRY(PML4INDEX, PML3INDEX, PML2INDEX)                             \
  (PML2TABLE(PML4INDEX, PML3INDEX)[PML2INDEX])
#define PML1ENTRY(PML4INDEX, PML3INDEX, PML2INDEX, PML1INDEX)                  \
  (PML1TABLE(PML4INDEX, PML3INDEX, PML2INDEX)[PML1INDEX])

namespace paging {
static PageTableEntry allocatePageTable() {
  return (PageTableEntry)(memory::allocateFrame() << 12) |
         PageTableFlags::PRESENT | PageTableFlags::WRITABLE |
         PageTableFlags::ALLOCATED | PageTableFlags::USER;
}
// If not allocatePageTables, return nullptr if the page table does not exist
static PageTableEntry *getPageTableEntry(void *virtualAddress,
                                         bool allocatePageTables) {
  uint64_t address = (uint64_t)virtualAddress &
                     ((1UL << 48) - 1); // Mask for 48 bit address space
  // Mask and shift out the parts of the address
  uint64_t pml1Index = address & 511;
  address >>= 9;
  uint64_t pml2Index = address & 511;
  address >>= 9;
  uint64_t pml3Index = address & 511;
  address >>= 9;
  uint64_t pml4Index = address & 511;
  // Make sure the page tables all exist
  // Check the pml4 entry
  if ((PML4ENTRY(pml4Index) & PageTableFlags::PRESENT) == 0) {
    if (allocatePageTable) {
      PML4ENTRY(pml4Index) = allocatePageTable();
      memset(PML3TABLE(pml4Index), 0, 4096);
    } else {
      return nullptr;
    }
  }
  if ((PML3ENTRY(pml4Index, pml3Index) & PageTableFlags::PRESENT) == 0) {
    if (allocatePageTable) {
      PML3ENTRY(pml4Index, pml3Index) = allocatePageTable();
      memset(PML2TABLE(pml4Index, pml3Index), 0, 4096);
    } else {
      return nullptr;
    }
  }
  if ((PML2ENTRY(pml4Index, pml3Index, pml2Index) & PageTableFlags::PRESENT) ==
      0) {
    if (allocatePageTable) {
      PML2ENTRY(pml4Index, pml3Index, pml2Index) = allocatePageTable();
      memset(PML1TABLE(pml4Index, pml3Index, pml2Index), 0, 4096);
    } else {
      return nullptr;
    }
  }
  return &PML1ENTRY(pml4Index, pml3Index, pml2Index, pml1Index);
}
void mapPage(void *virtualAddress, void *physicalAddress, PageTableFlags flags,
             bool allocated) {
  flags |= PageTableFlags::PRESENT;
  if (allocated) {
    flags |= PageTableFlags::ALLOCATED;
  }
  PageTableEntry pageTableEntry =
      (PageTableEntry)((uint64_t)flags | ((uint64_t)physicalAddress & ~4095ul));
  *getPageTableEntry(virtualAddress, true) = pageTableEntry;
}
void unmapPage(void *virtualAddress) {
  PageTableEntry *pageTableEntry = getPageTableEntry(virtualAddress, false);
  if (pageTableEntry != nullptr) {
    if ((*pageTableEntry & PageTableFlags::ALLOCATED) != 0) {
      memory::returnFrame(((uint64_t)*pageTableEntry >> 12) &
                          ((1ul << 52) - 1));
    }
    *pageTableEntry = (PageTableEntry)0;
  }
}
} // namespace paging
