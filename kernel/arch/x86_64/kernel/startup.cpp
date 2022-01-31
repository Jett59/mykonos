/*
    Copyright (C) 2021  Jett Thompson

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
#include <mykonos/string.h>

#include <mykonos/mbi.h>
#include <mykonos/pageTableInit.h>

#include <mykonos/frameBuffer.h>
#include <mykonos/kout.h>

#include <mykonos/test.h>

#include <mykonos/interrupts.h>

#include <mykonos/kpanic.h>

#include <mykonos/acpi/hpet.h>
#include <mykonos/acpi/madt.h>
#include <mykonos/acpi/rsdp.h>
#include <mykonos/acpi/rsdt.h>
#include <mykonos/acpi/tables.h>

#include <mykonos/hpet.h>

#include <mykonos/kmalloc.h>

#include <mykonos/apic.h>
#include <mykonos/smp.h>

#include <mykonos/pic.h>

#include <mykonos/apicTimer.h>

typedef void (*ConstructorOrDestructor)();

extern "C" {
extern ConstructorOrDestructor __init_array_start[0], __init_array_end[0];
extern ConstructorOrDestructor __fini_array_start[0], __fini_array_end[0];

extern uint8_t smpTrampolineStart, smpTrampolineEnd;
}

static volatile unsigned localApicTickSetting = 0;

extern "C" [[noreturn]] void kstart() {
  // Global constructors must be called
  // We use init_array method, for simplicity
  for (ConstructorOrDestructor *initArrayElement = __init_array_start;
       initArrayElement != __init_array_end; initArrayElement++) {
    (*initArrayElement)();
  }
  // Now that that's over
  multiboot::parseMbi();
  paging::initPageTables();
  display::initFrameBuffer();
  interrupts::init();
  interrupts::install();
  kout::print("Initialized the console\n\n");
  if (test::runTests(kout::print)) {
    // Continue
    if (!memeq(acpi::rsdp.signature, "RSD PTR ", 8)) {
      kpanic("No ACPI found");
    }
    if (!acpi::rsdp.doChecksum()) {
      kpanic("Checksum of RSDP failed");
    }
    kout::printf("Found RSDP (revision: %d)\n", acpi::rsdp.revision);
    void *rsdtAddress = (acpi::rsdp.revision >= 2)
                            ? (void *)(size_t)acpi::rsdp.xsdtAddress
                            : (void *)(size_t)acpi::rsdp.rsdtAddress;
    acpi::RsdtTableManager *rsdt =
        (acpi::RsdtTableManager *)acpi::loadTable(rsdtAddress);
    if (rsdt == nullptr) {
      kpanic("Error loading RSDT");
    }
    kout::print("Found RSDT\n");
    acpi::HpetTableManager *hpetTableManager =
        (acpi::HpetTableManager *)rsdt->get(acpi::TableType::HPET);
    if (hpetTableManager == nullptr) {
      kpanic("No HPET found");
    }
    if (hpetTableManager->comparatorCount() == 0) {
      kpanic("No usable HPET found");
    }
    hpet::Hpet hpet(hpetTableManager->getPhysicalAddress());
    kout::printf("HPET says current time is %lns sinse reset (with precision "
                 "of %lkHz)\n",
                 hpet.nanoTime(), hpet.getFrequencyKhz());
    acpi::MadtTableManager *madt =
        (acpi::MadtTableManager *)rsdt->get(acpi::TableType::MADT);
    if (madt == nullptr) {
      kpanic("No MADT found");
    }
    apic::localApic.init(madt->getLocalApicAddress());
    apic::localApic.enable();
    kout::printf("Initialized local APIC with version %x\n",
                 apic::localApic.getVersion());
    if (madt->getHasPic()) {
      kout::print("Disabling legacy PIC\n");
      pic::disablePic();
    }
    if (madt->localApicCount() > 1) {
      // Copy smp trampoline to low memory
      void *smpTrampolineDestination =
          memory::mapAddress((void *)0x8000, 0x1000, false);
      size_t smpTrampolineSize =
          (size_t)&smpTrampolineEnd - (size_t)&smpTrampolineStart;
      memcpy(smpTrampolineDestination, &smpTrampolineStart, smpTrampolineSize);
      memory::unmapMemory(smpTrampolineDestination, 0x1000);
      smp::allocateStacks(madt->localApicCount());
      kout::print("Beginning SMP initialization\n");
      uint8_t myApicId = apic::localApic.getApicId();
      for (unsigned i = 0; i < madt->localApicCount(); i++) {
        uint8_t apicId = madt->getLocalApic(i).apicId;
        if (apicId != myApicId) {
          if (!smp::startCpu(apicId, hpet)) {
            kout::printf("CPU %d failed to start\n", i);
            kpanic("Error starting CPUs");
          }
        } else {
          kout::printf("I am CPU %d\n", i);
        }
      }
    }
    // Store our CPU number (0 for BSP)
    cpu::setCpuNumber(0);
    unsigned ticksPer10ms = apic::timerTicksPer(10000000, hpet);
    localApicTickSetting = ticksPer10ms;
    kout::printf("APIC timer runs at %dHz\n", ticksPer10ms * 100);
    // Mask all internal interrupts so we can start getting timer IRQs
    apic::localApic.maskAllInternal();
    cpu::enableLocalIrqs();
    // Set up the APIC timer
    apic::setUpTimer(localApicTickSetting);
    // Wait a bit for some APIC timer interrupts
    hpet.wait(250000000);
    kpanic("It all worked");
  } else {
    // The tests failed! Abort
    kpanic("The tests failed!");
  }
}

extern "C" [[noreturn]] void kstartApCpu(uint8_t cpuNumber) {
  // Store the CPU number for later use
  cpu::setCpuNumber(cpuNumber);
  interrupts::install();
  kout::printf("Started CPU %d\n", cpuNumber);
  apic::localApic.enable();
  // Mask all internal interrupts so we can start getting timer IRQs
  apic::localApic.maskAllInternal();
  cpu::enableLocalIrqs();
  // Wait for the BSP to figure out the APIC setting
  while (localApicTickSetting == 0) {
    cpu::relax();
  }
  // Set up the APIC timer
  apic::setUpTimer(localApicTickSetting);
  // Just hault for now
  cpu::hault();
}
