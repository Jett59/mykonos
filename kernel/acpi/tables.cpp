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
#include <mykonos/acpi/rsdp.h>
#include <mykonos/acpi/tables.h>

#include <mykonos/acpi/hpet.h>
#include <mykonos/acpi/madt.h>
#include <mykonos/acpi/mcfg.h>
#include <mykonos/acpi/rsdt.h>

#include <mykonos/kmalloc.h>
#include <mykonos/kout.h>
#include <mykonos/string.h>

namespace acpi {
typedef TableManager *(*TableManagerCreator)(TableHeader *);

struct TableHandler {
  char signature[5]; // Includes unused null pointer
  TableManagerCreator creator;
};

TableManager *loadRsdt(TableHeader *header) {
  return new RsdtTableManager(header);
}
TableManager *loadMadt(TableHeader *header) {
  return new MadtTableManager(header);
}
TableManager *loadHpet(TableHeader *header) {
  return new HpetTableManager(header);
}
TableManager *loadMcfg(TableHeader *header) {
  return new McfgTableManager(header);
}

static TableHandler tableHandlers[] = {{"RSDT", loadRsdt},
                                       {"XSDT", loadRsdt},
                                       {"APIC", loadMadt},
                                       {"HPET", loadHpet},
                                       {"MCFG", loadMcfg}};
#define numTableHandlers (sizeof(tableHandlers) / sizeof(TableHandler))

static bool doChecksum(TableHeader *header) {
  unsigned char *tablePointer = (unsigned char *)header;
  unsigned char sum = 0;
  for (unsigned i = 0; i < header->length; i++) {
    sum += tablePointer[i];
  }
  return sum == 0;
}

TableManager *loadTable(void *physicalAddress) {
  TableHeader *header = (TableHeader *)memory::mapAddress(
      physicalAddress, sizeof(TableHeader), true);
  kout::print("Loading ACPI table: ");
  kout::print(String(header->signature, 4));
  kout::print("\n");
  size_t tableSize = header->length;
  if (tableSize < sizeof(TableHeader)) {
    kout::print("Invalid table: Size too small\n");
    return nullptr;
  }
  memory::unmapMemory(header, sizeof(TableHeader));
  header = (TableHeader *)memory::mapAddress(physicalAddress, tableSize, true);
  if (!doChecksum(header)) {
    kout::print("Table did not pass checksum\n");
    memory::unmapMemory(header, header->length);
    return nullptr;
  }
  for (unsigned i = 0; i < numTableHandlers; i++) {
    TableHandler handler = tableHandlers[i];
    if (memeq(header->signature, handler.signature, 4)) {
      return handler.creator(header);
    }
  }
  // No suitable handler
  memory::unmapMemory(header, tableSize);
  return nullptr;
}
} // namespace acpi
