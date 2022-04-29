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
#include <mykonos/drivers/pcie/pcie.h>
#include <mykonos/drivers/pcieTree.h>
#include <mykonos/drivers/usb/xhci/xhciDriver.h>
#include <mykonos/kout.h>

#define PCIE_DEVICE(BASE, BUS, DEVICE, FUNCTION)                               \
  PcieDeviceAccess {                                                           \
    (PcieDeviceHeader *)((size_t)(BASE) + ((BUS) << 20) + ((DEVICE) << 15) +   \
                         ((FUNCTION) << 12))                                   \
  }

namespace drivers {
struct PcieDriver {
  // Used if vendorId != 0xffff
  uint16_t vendorId;
  uint16_t deviceId;
  // Used if vendorId == 0xffff
  uint8_t classId;
  uint8_t subclass;
  uint8_t registerInterface; // Used if != 0xff

  DeviceTree *(*get)(PcieDeviceAccess);
};

static DeviceTree *loadXhciDriver(PcieDeviceAccess access) {
  return new xhci::XhciDriver(
      xhci::XhciRegisterAccess((xhci::XhciRegisters *)access.mapBar(0)));
}

static PcieDriver pcieDrivers[] = {
    {0xffff, 0xffff, 0x0c, 0x03, 0x30, loadXhciDriver}};

void PcieDeviceTree::load() {
  kout::print("Scanning PCIE configuration\n");
  unsigned unknownDeviceCount = 0;
  for (size_t i = 0; i < mcfg->entryCount(); i++) {
    auto mcfgEntry = mcfg->getEntry(i);
    for (size_t bus = mcfgEntry.firstBusNumber; bus <= mcfgEntry.lastBusNumber;
         bus++) {
      for (size_t device = 0; device < 32; device++) {
        for (size_t function = 0; function < 8; function++) {
          auto access = PCIE_DEVICE(mcfgEntry.address, bus, device, function);
          unsigned headerType = access.getHeaderType();
          if ((headerType & 0x7f) == 0 && access.getVendorId() != 0xffff) {
            kout::printf("Vendor: %x, device: %x, class: %x, subclass: %x\n",
                         access.getVendorId(), access.getDeviceId(),
                         access.getClass(), access.getSubclass());
            bool matched = false;
            for (auto &driver : pcieDrivers) {
              if (driver.vendorId != 0xffff) {
                matched = driver.vendorId == access.getVendorId() &&
                          driver.deviceId == access.getDeviceId();
              } else {
                matched =
                    driver.classId == access.getClass() &&
                    driver.subclass == access.getSubclass() &&
                    (driver.registerInterface == 0xff ||
                     driver.registerInterface == access.getRegisterInterface());
              }
              if (matched) {
                appendAndLoad(driver.get(access));
                break;
              }
            }
            if (!matched) {
              unknownDeviceCount++;
            }
          }
          if (function == 0 && (headerType & 0x80) == 0) {
            break;
          }
        }
      }
    }
  }
  kout::printf("%d unknown PCIE devices\n", unknownDeviceCount);
}
} // namespace drivers
