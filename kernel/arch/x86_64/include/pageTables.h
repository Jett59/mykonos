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
  // Standard bits
  PRESENT = 1 << 0,
  WRITABLE = 1 << 1,
  USER = 1 << 2,
  UNCACHEABLE = 1 << 4,
  // Available bits
  ALLOCATED = 1 << 11
};
inline PageTableFlags operator|(PageTableFlags a, PageTableFlags b) {
  return (PageTableFlags)((uint64_t)a + (uint64_t)b);
}
inline PageTableFlags &operator|=(PageTableFlags &a, PageTableFlags b) {
  return a = a | b;
}
inline uint64_t operator&(PageTableFlags a, PageTableFlags b) {
  return (uint64_t)a & (uint64_t)b;
}

typedef PageTableFlags PageTableEntry;

typedef PageTableEntry Pml1[512];
typedef Pml1 Pml2[512];
typedef Pml2 Pml3[512];
typedef Pml3 Pml4[512];

// Quick check: is the pml4 512gb in size?
static_assert(sizeof(Pml4) == 549755813888, "Pml4 must be 512gb in size");
} // namespace paging

// Recursive mapping
#define PAGE_TABLE_POINTER ((paging::Pml4 *)0xFFFFFF8000000000)

#endif