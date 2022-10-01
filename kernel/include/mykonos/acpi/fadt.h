/*
    Copyright (C) 2022 Jett Thompson

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
#ifndef _MYKONOS_ACPI_FADT_H
#define _MYKONOS_ACPI_FADT_H

#include <mykonos/acpi/tables.h>

#include <stdint.h>

namespace acpi {
class FadtTableManager : public TableManager {
 public:
  FadtTableManager(TableHeader* header);

#ifdef __x86_64__
  bool getHasPs2Controller() { return hasPs2Controller; }
#endif

 private:
#ifdef __x86_64__
  bool hasPs2Controller;
#endif
};
// https://wiki.osdev.org/FADT
struct Fadt {
  TableHeader header;
  uint32_t firmwareCtrl;
  uint32_t dsdt;

  // field used in ACPI 1.0; no longer in use, for compatibility only
  uint8_t reserved;
  uint8_t preferredPowerManagementProfile;
  uint16_t sciInterrupt;
  uint32_t smiCommandPort;
  uint8_t acpiEnable;
  uint8_t acpiDisable;
  uint8_t s4BiosReq;
  uint8_t pStateControl;
  uint32_t Pm1aEventBlock;
  uint32_t pm1bEventBlock;
  uint32_t pm1aControlBlock;
  uint32_t pm1bControlBlock;
  uint32_t pm2ControlBlock;
  uint32_t pmTimerBlock;
  uint32_t gpe0Block;
  uint32_t gpe1Block;
  uint8_t pm1EventLength;
  uint8_t pm1ControlLength;
  uint8_t pm2ControlLength;
  uint8_t pmTimerLength;
  uint8_t gpe0Length;
  uint8_t gpe1Length;
  uint8_t gpe1Base;
  uint8_t cStateControl;
  uint16_t worstC2Latency;
  uint16_t worstC3Latency;
  uint16_t flushSize;
  uint16_t flushStride;
  uint8_t dutyOffset;
  uint8_t dutyWidth;
  uint8_t dayAlarm;
  uint8_t monthAlarm;
  uint8_t century;

  // reserved in ACPI 1.0; used since ACPI 2.0+
  uint16_t bootArchitectureFlags;
  uint8_t reserved2;
  uint32_t flags;

  // 12 byte structure; see below for details
  Address resetReg;
  uint8_t resetValue;
  uint8_t reserved3[3];

  // 64bit pointers - Available on ACPI 2.0+
  uint64_t xFirmwareControl;
  uint64_t xDsdt;
  Address xPm1aEventBlock;
  Address xPm1bEventBlock;
  Address xPm1aControlBlock;
  Address xPm1bControlBlock;
  Address xPm2ControlBlock;
  Address xPmTimerBlock;
  Address xGpe0Block;
  Address xGpe1Block;
} __attribute__((packed));
}  // namespace acpi

#endif