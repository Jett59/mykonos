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
#ifndef _APIC_H
#define _APIC_H

#define MAX_LOCAL_APICS 64
#define MAX_IO_APICS 24

#include <stddef.h>
#include <stdint.h>

#include <kmalloc.h>
#include <mmio.h>

namespace apic {
struct LocalApicDescriptor {
  uint8_t apicId;
};
struct IoApicDescriptor {
  void *physicalAddress;
  uint32_t gsiBase;
};

#define LOCAL_APIC_VERSION_REGISTER 0x30
#define LOCAL_APIC_ERROR_REGISTER 0x280

#define LOCAL_APIC_VERSION_MASK 0xFF

#define LOCAL_APIC_SPURIOUS_INTERRUPT_VECTOR 0xff

class LocalApic {
public:
  void init(void *physicalAddress);

  uint32_t getVersion() {
    return readRegister(LOCAL_APIC_VERSION_REGISTER) & LOCAL_APIC_VERSION_MASK;
  }

  void clearErrors() { writeRegister(LOCAL_APIC_ERROR_REGISTER, 0); }

  void sendIpi(uint8_t vector, uint8_t messageType, bool logicalDestination,
               bool assert, bool levelTriggered, uint8_t destinationApicId);

 private:
  uint32_t *registers = nullptr;

  void writeRegister(size_t offset, uint32_t value) {
    mmio::write(registers + (offset / 4), value);
  }
  uint32_t readRegister(size_t offset) {
    return mmio::read(registers + (offset / 4));
  }
};

extern LocalApic localApic;
} // namespace apic

#endif