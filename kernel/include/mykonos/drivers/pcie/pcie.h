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
#ifndef _MYKONOS_DRIVERS_PCIE_PCIE_H
#define _MYKONOS_DRIVERS_PCIE_PCIE_H

#include <mykonos/mmio.h>
#include <stdint.h>

namespace drivers {
struct PcieDeviceHeader {
  uint16_t vendorId;
  uint16_t deviceId;
  uint16_t command;
  uint16_t status;
  uint8_t revision;
  uint8_t registerInterface;
  uint8_t subclass;
  uint8_t classId;
  uint8_t cacheLineSize;
  uint8_t latencyTimer;
  uint8_t headerType;
  uint8_t selfTest;
  uint32_t bar1;
  uint32_t bar2;
  uint32_t bar3;
  uint32_t bar4;
  uint32_t bar5;
};
class PcieDeviceAccess {
public:
  PcieDeviceAccess(PcieDeviceHeader *devicePointer)
      : devicePointer(devicePointer) {}

  uint16_t getVendorId() { return mmio::read(&devicePointer->vendorId); }
  uint16_t getDeviceId() { return mmio::read(&devicePointer->deviceId); }

  uint16_t getCommand() { return mmio::read(&devicePointer->command); }
  uint16_t getStatus() { return mmio::read(&devicePointer->status); }

  uint8_t getRevision() { return mmio::read(&devicePointer->revision); }
  uint8_t getRegisterInterface() {
    return mmio::read(&devicePointer->registerInterface);
  }
  uint8_t getSubclass() { return mmio::read(&devicePointer->subclass); }
  uint8_t getClass() { return mmio::read(&devicePointer->classId); }

private:
  const PcieDeviceHeader *devicePointer;
};
} // namespace drivers

#endif