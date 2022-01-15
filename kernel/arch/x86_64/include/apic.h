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
#define LOCAL_APIC_ID_REGISTER 0x20

#define LOCAL_APIC_VERSION_MASK 0xFF
#define LOCAL_APIC_ID_POSITION 24

#define LOCAL_APIC_SPURIOUS_INTERRUPT_VECTOR 0xff

#define APIC_FIXED_IPI 0
#define APIC_LOWEST_PRIORITY_IPI 1
#define APIC_SMI_IPI 2
#define APIC_REMOTE_READ_IPI 3
#define APIC_NMI_IPI 4
#define APIC_INIT_IPI 5
#define APIC_STARTUP_IPI 6
#define APIC_EXTERNAL_INTERRUPT_IPI 7

class LocalApic {
public:
  void init(void *physicalAddress);

  void enable();

  uint32_t getVersion() {
    return readRegister(LOCAL_APIC_VERSION_REGISTER) & LOCAL_APIC_VERSION_MASK;
  }
  uint8_t getApicId() {
    return readRegister(LOCAL_APIC_ID_REGISTER) >> LOCAL_APIC_ID_POSITION;
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