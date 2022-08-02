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
#ifndef _MYKONOS_ACPI_MCFG_H
#define _MYKONOS_ACPI_MCFG_H

#include <mykonos/acpi/tables.h>

#include <stddef.h>
#include <stdint.h>

namespace acpi {
struct McfgEntry {
  void* address;
  unsigned segmentGroup;
  unsigned firstBusNumber;
  unsigned lastBusNumber;
};
#define MAX_MCFG_ENTRIES 16
class McfgTableManager : public TableManager {
 public:
  McfgTableManager(TableHeader* header);

  unsigned entryCount() { return numEntries; }
  McfgEntry getEntry(unsigned index) {
    return index < numEntries ? entries[index] : McfgEntry{nullptr, 0, 0, 0};
  }

 private:
  unsigned numEntries;
  McfgEntry entries[MAX_MCFG_ENTRIES];
};

struct McfgTableEntry {
  uint64_t address;
  uint16_t segmentGroup;
  uint8_t firstBusNumber;
  uint8_t lastBusNumber;
  uint32_t reserved;
};
static_assert(sizeof(McfgTableEntry) == 16, "McfgTableEntry must be 16 bytes");
}  // namespace acpi

#endif