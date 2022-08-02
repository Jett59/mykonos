/*
    Copyright (C) 2022  Jett Thompson

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
#include <mykonos/acpi/hpet.h>

#include <mykonos/kmalloc.h>
#include <mykonos/kout.h>

namespace acpi {
HpetTableManager::HpetTableManager(TableHeader* header) : TableManager(TableType::HPET) {
  HpetTable* hpet = (HpetTable*)header;
  if (hpet->adress.adressSpace != 0) {
    kout::print("Unusable HPET: Not memory mapped\n");
  } else {
    legacyReplacementCapable = hpet->legacyReplacementCapable;
    numComparators = hpet->numComparators + 1;
    physicalAddress = (void*)(size_t)hpet->adress.address;
    kout::printf("Found HPET with %d comparators\n", numComparators);
  }
  memory::unmapMemory(header, header->length);
}
}  // namespace acpi
