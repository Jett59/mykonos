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
#ifndef _ACPI_HPET_H
#define _ACPI_HPET_H

#include <acpi/tables.h>

namespace acpi {
class HpetTableManager : public TableManager {
public:
  HpetTableManager(TableHeader *header);

  void *getPhysicalAddress() { return physicalAddress; }
  uint8_t comparatorCount() { return numComparators; }
  bool isLegacyReplacementCapable() { return legacyReplacementCapable; }

private:
  void *physicalAddress;
  uint8_t numComparators = 0;
  bool legacyReplacementCapable;
};
struct __attribute__((packed)) HpetTable {
  TableHeader header;
  uint8_t hardwareRevId;
  uint8_t numComparators : 5;
  uint8_t counterSize : 1; // 0 = 32 bits, 1 = 64 bits
  uint8_t reserved : 1;
  uint8_t legacyReplacementCapable : 1;
  uint16_t vendorId;
  Address adress;
  uint8_t hpetNumber;
  uint16_t
      minimumTicks; // Minimum ticks to not lose interrupts in periodic mode
  uint8_t pageProtection;
};
static_assert(sizeof(HpetTable) == sizeof(TableHeader) + 20,
              "HpetTable is not packed");
} // namespace acpi

#endif