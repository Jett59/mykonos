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
#ifndef _ACPI_MADT_H
#define _ACPI_MADT_H

#include <acpi/tables.h>

#include <apic.h>

namespace acpi {
class MadtTableManager : public TableManager {
public:
  MadtTableManager(TableHeader *header);

private:
  bool hasPic;
  void *localApicAddress;
  unsigned numLocalApics = 0;
  apic::LocalApicDescriptor localApics[MAX_LOCAL_APICS];
  unsigned numIoApics = 0;
  apic::IoApicDescriptor ioApics[MAX_IO_APICS];
};
struct __attribute__((packed)) MadtTable {
  TableHeader header;
  uint32_t localInterruptController;
  uint32_t flags;
};
static_assert(sizeof(MadtTable) == sizeof(TableHeader) + 8,
              "MadtTable has not been packed");
#define MADT_FLAGS_PIC (1 << 0)
struct MadtEntry {
  uint8_t type;
  uint8_t length;
};
static_assert(sizeof(MadtEntry) == 2, "MadtEntry has not been packed");
#define MADT_TYPE_LOCAL_APIC 0
#define MADT_TYPE_IO_APIC 1
#define MADT_TYPE_GSI_OVERRIDE 2
struct MadtLocalApicEntry {
  uint8_t type;
  uint8_t length;
  uint8_t acpiId;
  uint8_t apicId;
  uint32_t flags;
};
static_assert(sizeof(MadtLocalApicEntry) == 8,
              "MadtLocalApicEntry has not been packed");
#define MADT_FLAGS_ENABLED (1 << 0)
#define MADT_FLAGS_ENABLEABLE (1 << 1)
struct MadtIoApicEntry {
  uint8_t type;
  uint8_t length;
  uint8_t ioApicId;
  uint8_t reserved;
  uint32_t physicalAddress;
  uint32_t gsiBase;
};
static_assert(sizeof(MadtIoApicEntry) == 12,
              "MadtIoApicEntry has not been packed");
} // namespace acpi

#endif