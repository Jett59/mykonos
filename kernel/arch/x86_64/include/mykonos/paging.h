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
#ifndef _MYKONOS_PAGING_H
#define _MYKONOS_PAGING_H

#include <mykonos/pageTables.h>

namespace paging {
void mapPage(void* virtualAddress,
             void* physicalAddress,
             PageTableFlags flags,
             bool allocated,
             bool cacheable);
// Unmaps the page and invalidates the TLB for the page
void unmapPage(void* virtualAddress);

static inline void invalidateTlb(void* address) {
  __asm__ volatile("invlpg (%0)" : : "r"(address) : "memory");
}
}  // namespace paging

#endif