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
#ifndef _ACPI_TABLES_H
#define _ACPI_TABLES_H

#include <stdint.h>

namespace acpi {
struct TableHeader {
  char signature[4];
  uint32_t length;
  uint8_t revision;
  uint8_t checksum;
  char oemId[6];
  char oemTableId[8];
  uint32_t oemRevision;
  char creatorId[4];
  uint32_t creatorRevision;
};
static_assert(sizeof(TableHeader) == 36, "TableHeader incorrect size");
struct __attribute__((packed)) Address {
  uint8_t adressSpace;
  uint8_t bitWidth;
  uint8_t bitOffset;
  uint8_t reserved;
  uint64_t address;
};
static_assert(sizeof(Address) == 12, "Address is not packed");

enum class TableType { RSDT, MADT, HPET };
class TableManager {
public:
  const TableType type;

  virtual ~TableManager() {}

protected:
  TableManager(TableType type) : type(type) {}
};

TableManager *loadTable(void *physicalAddress);
} // namespace acpi

#endif