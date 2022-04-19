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
#include <mykonos/kout.h>

#define PCIE_DEVICE(BASE, BUS, DEVICE, FUNCTION)                               \
  PcieDeviceAccess {                                                           \
    (PcieDeviceHeader *)((size_t)(BASE) + ((BUS) << 20) + ((DEVICE) << 15) +   \
                         ((FUNCTION) << 12))                                   \
  }

namespace drivers {
void PcieDeviceTree::load() {
  kout::print("Scanning PCIE configuration\n");
  unsigned unknownDeviceCount = 0;
  for (size_t i = 0; i < mcfg->entryCount(); i++) {
    auto mcfgEntry = mcfg->getEntry(i);
    size_t busCount = mcfgEntry.lastBusNumber - mcfgEntry.firstBusNumber + 1;
    for (size_t bus = 0; bus < busCount; bus++) {
      for (size_t device = 0; device < 32; device++) {
        auto access = PCIE_DEVICE(mcfgEntry.address, bus, device, 0);
        if (access.getVendorId() != 0xffff) {
          kout::printf("Vendor: %x, device: %x, class: %x, subclass: %x\n",
                       access.getVendorId(), access.getDeviceId(),
                       access.getClass(), access.getSubclass());
          unknownDeviceCount++;
        }
      }
    }
  }
  kout::printf("%d unknown PCIE devices\n", unknownDeviceCount);
}
} // namespace drivers
