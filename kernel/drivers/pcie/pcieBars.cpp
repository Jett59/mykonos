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
#include <mykonos/kmalloc.h>
#include <mykonos/kout.h>
#include <stddef.h>
#include <stdint.h>

#define PCIE_BAR_MEMORY (1 << 0)
#define PCIE_BAR_TYPE(BAR) (((BAR)&0x6) >> 1)
#define PCIE_BAR_64 2
#define PCIE_BAR_32 0

namespace drivers {
void *PcieDeviceAccess::mapBar(unsigned number) {
  if (number > 5) {
    kout::printf("Invalid BAR number %d\n", number);
    return nullptr;
  }
  uint64_t bar = readBar(number);
  if ((bar & PCIE_BAR_MEMORY) == 0) {
    kout::print("BAR not present in memory\n");
    return nullptr;
  }
  bar &= 0xfffffff0;
  if (number < 5 && PCIE_BAR_TYPE(bar) == PCIE_BAR_64) {
    bar |= (uint64_t)readBar(number + 1) << 32;
  }
  // Setting the BAR to ffffffff lets us find the number of bits we can change
  // which tells us the size of the mapped region
  writeBar(number, 0xffffffff);
  size_t mappedSize = ~(readBar(number) & 0xfffffff0) + 1;
  writeBar(number, bar & 0xffffffff);
  return memory::mapAddress((void *)bar, mappedSize, false);
}
} // namespace drivers
