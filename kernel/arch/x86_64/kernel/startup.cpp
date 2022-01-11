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
#include <string.h>

#include <mbi.h>
#include <pageTableInit.h>

#include <frameBuffer.h>
#include <kout.h>

#include <test.h>

#include <interrupts.h>

#include <kpanic.h>

#include <acpi/hpet.h>
#include <acpi/madt.h>
#include <acpi/rsdp.h>
#include <acpi/rsdt.h>
#include <acpi/tables.h>

#include <hpet.h>

#include <apic.h>

typedef void (*ConstructorOrDestructor)();

extern "C" {
extern ConstructorOrDestructor __init_array_start[0], __init_array_end[0];
extern ConstructorOrDestructor __fini_array_start[0], __fini_array_end[0];
}

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
    kout::printf("Initialized local APIC with version %x\n",
                 apic::localApic.getVersion());
    kpanic("It all worked");
  } else {
    // The tests failed! Abort
    kpanic("The tests failed!");
  }
}
