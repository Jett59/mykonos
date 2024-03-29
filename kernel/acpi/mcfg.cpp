/*
    Copyright (C) 2022 Jett Thompson

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
#include <mykonos/acpi/mcfg.h>

#include <mykonos/kmalloc.h>
#include <mykonos/kout.h>

namespace acpi {
McfgTableManager::McfgTableManager(TableHeader* header) : TableManager(TableType::MCFG) {
  this->numEntries = (header->length - 44) / 16;
  McfgTableEntry* tableEntries = (McfgTableEntry*)((size_t)header + 44);
  for (unsigned i = 0; i < numEntries; i++) {
    auto& tableEntry = tableEntries[i];
    void* mappedAddress = memory::mapAddress(
        (void*)tableEntry.address, (tableEntry.lastBusNumber - tableEntry.firstBusNumber + 1) * 1048576, false);
    entries[i] = {mappedAddress, tableEntry.segmentGroup, tableEntry.firstBusNumber, tableEntry.lastBusNumber};
  }
  kout::printf("Found %d MCFG entries\n", numEntries);
  memory::unmapMemory(header, header->length);
}
}  // namespace acpi
