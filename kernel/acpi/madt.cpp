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

namespace acpi {
MadtTableManager::MadtTableManager(TableHeader *header)
    : TableManager(TableType::MADT) {
  MadtTable *madt = (MadtTable *)header;
  localApicAddress = (void *)(size_t)madt->localInterruptController;
  hasPic = (madt->flags & MADT_FLAGS_PIC) != 0;
  MadtEntry *entry = (MadtEntry *)((uint8_t *)madt + sizeof(MadtTable));
  while ((size_t)entry - (size_t)madt < madt->header.length) {
    entry = (MadtEntry *)((uint8_t *)entry + entry->length);
  }
}
} // namespace acpi
