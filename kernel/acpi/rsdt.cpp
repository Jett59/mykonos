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
#include <acpi/rsdt.h>

#include <kmalloc.h>

namespace acpi {
RsdtTableManager::RsdtTableManager(TableHeader *header)
    : TableManager(TableType::RSDT) {
  size_t entrySize = header->signature[0] == 'X' ? 8 : 4;
  numChildren = (header->length - sizeof(TableHeader)) / entrySize;
  children = new TableManager *[numChildren];
  // TODO: Load the subtables
  uint8_t *entryStart = (uint8_t *)header + sizeof(TableHeader);
  for (unsigned i = 0; i < numChildren; i++) {
    switch (entrySize) {
    case 4: {
      children[i] = loadTable((void *)(size_t)((uint32_t *)entryStart)[i]);
      break;
    }
    case 8: {
      children[i] = loadTable((void *)(size_t)((uint64_t *)entryStart)[i]);
      break;
    }
    default:
      children[i] = nullptr;
      break;
    }
  }
  memory::unmapMemory(header, header->length);
}
RsdtTableManager::~RsdtTableManager() {
  for (unsigned i = 0; i < numChildren; i++) {
    if (children[i] != nullptr) {
      delete children[i];
    }
  }
}
} // namespace acpi
