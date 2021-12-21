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
   #include <paging.h>
   #include <pageTables.h>

#include <physicalMemory.h>

   namespace paging {
       static PageTableEntry* getPageTableEntry(void* virtualAddress) {
         uint64_t address = (uint64_t)virtualAddress & ((1UL << 48) - 1); // Mask for 48 bit address space
         // Mask and shift out the parts of the address
         uint64_t pml1Index = address & 511;
         address >>= 9;
         uint64_t pml2Index = address & 511;
         address >>= 9;
         uint64_t pml3Index = address & 511;
         address >>= 9;
         uint64_t pml4Index = address & 511;
         return &(*PAGE_TABLE_POINTER)[pml4Index][pml3Index][pml2Index][pml1Index];
       }
       void mapPage(void* virtualAddress, void* physicalAddress, PageTableFlags flags, bool allocated) {
           if (allocated) {
             flags |= PageTableFlags::ALLOCATED;
           }
           PageTableEntry pageTableEntry =
               (PageTableEntry)((uint64_t)flags |
                                ((uint64_t)physicalAddress & ~4095ul));
       }
   }
