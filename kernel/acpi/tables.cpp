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
#include <acpi/tables.h>

#include <acpi/rsdt.h>

#include <kmalloc.h>
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

static TableHandler tableHandlers[] = {{"RSDT", loadRsdt}, {"XSDT", loadRsdt}};
#define numTableHandlers (sizeof(tableHandlers) / sizeof(TableHandler))

TableManager *loadTable(void *physicalAddress) {
  TableHeader *header =
      (TableHeader *)memory::mapAddress(physicalAddress, sizeof(TableHeader));
  size_t tableSize = header->length;
  memory::unmapMemory(header, sizeof(TableHeader));
  header = (TableHeader *)memory::mapAddress(physicalAddress, tableSize);
  for (unsigned i = 0; i < numTableHandlers; i++) {
    TableHandler &handler = tableHandlers[i];
    if (memeq(header->signature, handler.signature, 4)) {
      return handler.creator(header);
    }
  }
  return nullptr;
}
} // namespace acpi
