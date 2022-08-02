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
#ifndef _MYKONOS_ACPI_MADT_H
#define _MYKONOS_ACPI_MADT_H

#include <mykonos/acpi/tables.h>

#include <mykonos/apic.h>

namespace acpi {
#define MAX_GSI_OVERRIDES 16
struct MadtGsiOverride {
  bool levelTriggered;
  bool activeHigh;
  uint8_t source;
  uint32_t destination;
};
class MadtTableManager : public TableManager {
 public:
  MadtTableManager(TableHeader* header);

  bool getHasPic() { return hasPic; }
  void* getLocalApicAddress() { return localApicAddress; }

  unsigned localApicCount() { return numLocalApics; }
  apic::LocalApicDescriptor getLocalApic(size_t i) { return i < numLocalApics ? localApics[i] : apic::LocalApicDescriptor(); }

  unsigned ioApicCount() { return numIoApics; }
  apic::IoApicDescriptor getIoApic(size_t i) { return i < numIoApics ? ioApics[i] : apic::IoApicDescriptor(); }

  unsigned gsiOverrideCount() { return numGsiOverrides; }
  MadtGsiOverride getGsiOverride(size_t i) { return i < numGsiOverrides ? gsiOverrides[i] : MadtGsiOverride(); }

 private:
  bool hasPic;
  void* localApicAddress;
  unsigned numLocalApics = 0;
  apic::LocalApicDescriptor localApics[MAX_LOCAL_APICS];
  unsigned numIoApics = 0;
  apic::IoApicDescriptor ioApics[MAX_IO_APICS];
  unsigned numGsiOverrides = 0;
  MadtGsiOverride gsiOverrides[MAX_GSI_OVERRIDES];
};
struct __attribute__((packed)) MadtTable {
  TableHeader header;
  uint32_t localInterruptController;
  uint32_t flags;
};
static_assert(sizeof(MadtTable) == sizeof(TableHeader) + 8, "MadtTable has not been packed");
// Has legacy PIC (officially PCAT_COMPAT)
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
static_assert(sizeof(MadtLocalApicEntry) == 8, "MadtLocalApicEntry has not been packed");
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
static_assert(sizeof(MadtIoApicEntry) == 12, "MadtIoApicEntry has not been packed");
struct __attribute__((packed)) MadtGsiOverrideEntry {
  uint8_t type;
  uint8_t length;
  uint8_t bus;
  uint8_t source;
  uint32_t destination;
  uint16_t flags;
};
static_assert(sizeof(MadtGsiOverrideEntry) == 10, "MadtGsiOverrideEntry has not been packed");
}  // namespace acpi

#endif