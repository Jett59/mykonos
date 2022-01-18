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
#include <mykonos/hpet.h>

#include <mykonos/kmalloc.h>

#define HPET_REGISTER_GENERAL_CAPABILITIES 0x0
#define HPET_REGISTER_GENERAL_CONFIGURATION 0x10

#define HPET_CONFIGURATION_ENABLE_BIT (1 << 0)
#define HPET_CONFIGURATION_LEGACY_BIT (1 << 1)

namespace hpet {
Hpet::Hpet(void *physicalAddress)
    : registerPointer(
          (uint64_t *)memory::mapAddress(physicalAddress, 1024, false)) {
  frequencyFemtos = readRegister(HPET_REGISTER_GENERAL_CAPABILITIES) >> 32;
  // Remove legacy mappings
  writeRegister(HPET_REGISTER_GENERAL_CONFIGURATION,
                readRegister(HPET_REGISTER_GENERAL_CONFIGURATION) &
                    ~HPET_CONFIGURATION_LEGACY_BIT);
  reset();
}
Hpet::~Hpet() { memory::unmapMemory(registerPointer, 1024); }

void Hpet::reset() {
  // Stop the counter
  writeRegister(HPET_REGISTER_GENERAL_CONFIGURATION,
                readRegister(HPET_REGISTER_GENERAL_CONFIGURATION) &
                    ~HPET_CONFIGURATION_ENABLE_BIT);
  // Set it to 0
  writeRegister(HPET_REGISTER_COUNTER, 0);
  // Enable the counter
  writeRegister(HPET_REGISTER_GENERAL_CONFIGURATION,
                readRegister(HPET_REGISTER_GENERAL_CONFIGURATION) |
                    HPET_CONFIGURATION_ENABLE_BIT);
}
} // namespace hpet
