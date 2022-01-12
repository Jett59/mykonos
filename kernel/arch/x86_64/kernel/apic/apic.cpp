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
#include <apic.h>

#define LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER 0xf0
#define LOCAL_APIC_IPI_REGISTER_LOW 0x300
#define LOCAL_APIC_IPI_REGISTER_HIGH 0x310

#define LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER_ENABLE (1 << 8)

namespace apic {
LocalApic localApic;

void LocalApic::init(void *physicalAddress) {
  if (registers == nullptr) {
    registers = (uint32_t *)memory::mapAddress(physicalAddress, 4096, false);
    writeRegister(LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER,
                  LOCAL_APIC_SPURIOUS_INTERRUPT_VECTOR |
                      LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER_ENABLE);
  }
}
void LocalApic::sendIpi(uint8_t vector, uint8_t messageType,
                        bool logicalDestination, bool assert,
                        bool levelTriggered, uint8_t destinationApicId) {
  // Send the ipi
  writeRegister(LOCAL_APIC_IPI_REGISTER_HIGH,
                (uint32_t)destinationApicId << 24);
  writeRegister(LOCAL_APIC_IPI_REGISTER_LOW,
                ((uint32_t)vector << 0) | ((uint32_t)messageType << 8) |
                    ((uint32_t)logicalDestination << 11) |
                    ((uint32_t)assert << 14) |
                    ((uint32_t)levelTriggered << 15));
  // Wait for delivery
  while ((readRegister(LOCAL_APIC_IPI_REGISTER_LOW) & (1 << 12)) != 0) {
    cpu::relax();
  }
}
} // namespace apic
