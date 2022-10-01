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
#include <mykonos/apic.h>
#include <mykonos/kout.h>
#include <mykonos/kpanic.h>

#define LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER 0xf0
#define LOCAL_APIC_IPI_REGISTER_LOW 0x300
#define LOCAL_APIC_IPI_REGISTER_HIGH 0x310

#define LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER_ENABLE (1 << 8)

namespace apic {
uint8_t localApicIds[MAX_LOCAL_APICS];

LocalApic localApic;

void LocalApic::init(void* physicalAddress) {
  if (registers == nullptr) {
    registers = (uint32_t*)memory::mapAddress(physicalAddress, 4096, false);
  }
}

void LocalApic::enable() {
  writeRegister(LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER,
                LOCAL_APIC_SPURIOUS_INTERRUPT_VECTOR | LOCAL_APIC_SPURIOUS_INTERRUPT_REGISTER_ENABLE);
}

void LocalApic::sendIpi(uint8_t vector,
                        uint8_t messageType,
                        bool logicalDestination,
                        bool assert,
                        bool levelTriggered,
                        uint8_t destinationApicId) {
  // Send the ipi
  writeRegister(LOCAL_APIC_IPI_REGISTER_HIGH, (uint32_t)destinationApicId << 24);
  writeRegister(LOCAL_APIC_IPI_REGISTER_LOW, ((uint32_t)vector << 0) | ((uint32_t)messageType << 8) |
                                                 ((uint32_t)logicalDestination << 11) | ((uint32_t)assert << 14) |
                                                 ((uint32_t)levelTriggered << 15));
  // Wait for delivery
  while ((readRegister(LOCAL_APIC_IPI_REGISTER_LOW) & (1 << 12)) != 0) {
    cpu::relax();
  }
}
void LocalApic::maskAllInternal() {
  maskLvtRegister(LOCAL_APIC_TIMER_LVT_REGISTER);
  maskLvtRegister(LOCAL_APIC_THERMAL_LVT_REGISTER);
  maskLvtRegister(LOCAL_APIC_PERFORMANCE_LVT_REGISTER);
  maskLvtRegister(LOCAL_APIC_ERROR_LVT_REGISTER);
}

struct IoApic {
  void* base;
  uint32_t gsiCount;
  uint32_t gsiBase;

  uint32_t readRegister(uint32_t registerIndex) {
    mmio::write((uint32_t*)base, registerIndex);
    return mmio::read((uint32_t*)base + 4);
  }
  void writeRegister(uint32_t registerIndex, uint32_t value) {
    mmio::write((uint32_t*)base, registerIndex);
    mmio::write((uint32_t*)base + 4, value);
  }

  void mapGsi(uint32_t gsi, uint32_t vector, bool levelTriggered, bool activeHigh, uint8_t targetLocalApic) {
    uint32_t redirectionEntry = readRegister(0x10 + gsi * 2);
    redirectionEntry |= (uint32_t)vector << 0;
    redirectionEntry |= (uint32_t)levelTriggered << 15;
    redirectionEntry |= (uint32_t)!activeHigh << 13;
    writeRegister(0x10 + gsi * 2, redirectionEntry);
    writeRegister(0x10 + gsi * 2 + 1, (uint32_t)targetLocalApic << 24);
  }

  void maskGsi(uint32_t gsi) { writeRegister(0x10 + gsi * 2, readRegister(0x10 + gsi * 2) | (1 << 16)); }
  void unmaskGsi(uint32_t gsi) { writeRegister(0x10 + gsi * 2, readRegister(0x10 + gsi * 2) & ~(1 << 16)); }
};

static IoApic ioApics[MAX_IO_APICS];
static unsigned ioApicCount = 0;

void initIoApics(const IoApicDescriptor* descriptors, size_t count) {
  ioApicCount = count;
  uint32_t nextGsiBase = 0;
  for (unsigned i = 0; i < count; i++) {
    const IoApicDescriptor& descriptor = descriptors[i];
    if (descriptor.gsiBase != nextGsiBase) {
      kout::printf("Expected GSI base %d but got %d\n", nextGsiBase, descriptor.gsiBase);
      kpanic("Invalid GSI base");
    }
    ioApics[i].base = memory::mapAddress(descriptor.physicalAddress, 32, false);
    ioApics[i].gsiCount = ((ioApics[i].readRegister(1) >> 16) & 0xff) + 1;  // n + 1 encoded.
    ioApics[i].gsiBase = descriptor.gsiBase;
    nextGsiBase += ioApics[i].gsiCount;
  }
}
void mapGsi(uint32_t gsi, uint32_t vector, bool levelTriggered, bool activeHigh, uint8_t targetCpuNumber) {
  uint32_t targetLocalApic = localApicIds[targetCpuNumber];
  for (unsigned i = 0; i < ioApicCount; i++) {
    if (gsi >= ioApics[i].gsiBase && gsi < ioApics[i].gsiBase + ioApics[i].gsiCount) {
      ioApics[i].mapGsi(gsi - ioApics[i].gsiBase, vector, levelTriggered, activeHigh, targetLocalApic);
      return;
    }
  }
  kout::printf("Failed to map GSI %d\n", gsi);
  kpanic("Failed to map GSI");
}
void maskGsi(uint32_t gsi) {
  for (unsigned i = 0; i < ioApicCount; i++) {
    if (gsi >= ioApics[i].gsiBase && gsi < ioApics[i].gsiBase + ioApics[i].gsiCount) {
      ioApics[i].maskGsi(gsi - ioApics[i].gsiBase);
      return;
    }
  }
  kout::printf("Failed to mask GSI %d\n", gsi);
  kpanic("Failed to mask GSI");
}
void unmaskGsi(uint32_t gsi) {
  for (unsigned i = 0; i < ioApicCount; i++) {
    if (gsi >= ioApics[i].gsiBase && gsi < ioApics[i].gsiBase + ioApics[i].gsiCount) {
      ioApics[i].unmaskGsi(gsi - ioApics[i].gsiBase);
      return;
    }
  }
  kout::printf("Failed to unmask GSI %d\n", gsi);
  kpanic("Failed to unmask GSI");
}
uint32_t getGsiCount() {
  uint32_t result = 0;
  for (unsigned i = 0; i < ioApicCount; i++) {
    result += ioApics[i].gsiCount;
  }
  return result;
}
}  // namespace apic
