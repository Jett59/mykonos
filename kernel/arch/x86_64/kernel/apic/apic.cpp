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
} // namespace apic
