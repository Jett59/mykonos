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
   #ifndef PAGE_TABLES_H
   #define PAGE_TABLES_H

#include <stdint.h>

namespace paging {
enum class PageTableFlags : uint64_t {
  PRESENT = 1 << 0,
  WRITABLE = 1 << 1,
  USER = 1 << 2
};
PageTableFlags operator|(PageTableFlags a, PageTableFlags b) {
  return (PageTableFlags)((uint64_t)a + (uint64_t)b);
}
PageTableFlags& operator|= (PageTableFlags& a, PageTableFlags b) {
  return a = a | b;
}
typedef PageTableFlags PageTableEntry;
typedef PageTableEntry PageTable[512];

#define NUM_OF_PML1S 133955584
#define NUM_OF_PML2S 261632
#define NUM_OF_PML3S 511

// Same structure as used in the recursive mapping on x86-64
struct PageTables {
    PageTable pml1s[NUM_OF_PML1S];
    PageTable pml2s[NUM_OF_PML2S];
    PageTable pml3s[NUM_OF_PML3S];
    PageTable pml4;
};
}

// Recursive mapping
#define PAGE_TABLE_POINTER ((PageTables*)0xFFFFFF8000000000)

#endif