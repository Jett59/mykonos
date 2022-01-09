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
#include <acpi/rsdp.h>
#include <acpi/tables.h>

#include <acpi/hpet.h>
#include <acpi/madt.h>
#include <acpi/rsdt.h>

#include <kmalloc.h>
#include <kout.h>
#include <string.h>

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

static TableHandler tableHandlers[] = {{"RSDT", loadRsdt},
                                       {"XSDT", loadRsdt},
                                       {"APIC", loadMadt},
                                       {"HPET", loadHpet}};
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
  kout::print("Loading ACPI table: ");
  TableHeader *header = (TableHeader *)memory::mapAddress(
      physicalAddress, sizeof(TableHeader), true);
  kout::print(header->signature, 4);
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
