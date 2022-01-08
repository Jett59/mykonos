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
#include <acpi/madt.h>
#include <apic.h>

#include <stddef.h>

#include <kmalloc.h>
#include <kout.h>

namespace acpi {
MadtTableManager::MadtTableManager(TableHeader *header)
    : TableManager(TableType::MADT) {
  MadtTable *madt = (MadtTable *)header;
  localApicAddress = (void *)(size_t)madt->localInterruptController;
  hasPic = (madt->flags & MADT_FLAGS_PIC) != 0;
  MadtEntry *entry = (MadtEntry *)((uint8_t *)madt + sizeof(MadtTable));
  while ((size_t)entry - (size_t)madt < madt->header.length) {
    switch (entry->type) {
    case MADT_TYPE_LOCAL_APIC: {
      MadtLocalApicEntry *localApicEntry = (MadtLocalApicEntry *)entry;
      if ((localApicEntry->flags & MADT_FLAGS_ENABLED) != 0 &&
          numLocalApics < MAX_LOCAL_APICS) {
        localApics[numLocalApics++].apicId = localApicEntry->apicId;
      }
      break;
    }
    case MADT_TYPE_IO_APIC: {
      MadtIoApicEntry *ioApicEntry = (MadtIoApicEntry *)entry;
      if (numIoApics < MAX_IO_APICS) {
        apic::IoApicDescriptor &ioApicDescriptor = ioApics[numIoApics++];
        ioApicDescriptor.gsiBase = ioApicEntry->gsiBase;
        ioApicDescriptor.physicalAddress =
            (void *)(size_t)ioApicEntry->physicalAddress;
      }
      break;
    }
    case MADT_TYPE_GSI_OVERRIDE: {
      MadtGsiOverrideEntry *gsiOverrideEntry = (MadtGsiOverrideEntry *)entry;
      if (numGsiOverrides < MAX_GSI_OVERRIDES) {
        MadtGsiOverride &gsiOverride = gsiOverrides[numGsiOverrides++];
        gsiOverride.activeHigh = (gsiOverrideEntry->flags & 2) == 0;
        gsiOverride.levelTriggered = (gsiOverrideEntry->flags & 8) != 0;
        gsiOverride.source = gsiOverrideEntry->source;
        gsiOverride.destination = gsiOverrideEntry->destination;
      }
      break;
    }
    default:
      kout::printf("Unknown MADT entry type %d\n", entry->type);
    }
    entry = (MadtEntry *)((uint8_t *)entry + entry->length);
  }
  kout::printf("Found %d local APICs, %d io APICs and %d GSI overrides\n",
               numLocalApics, numIoApics, numGsiOverrides);
  memory::unmapMemory(header, header->length);
}
} // namespace acpi
