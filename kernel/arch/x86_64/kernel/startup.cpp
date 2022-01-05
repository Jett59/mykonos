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

#include <acpi/rsdp.h>
#include <acpi/tables.h>

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
  kout::print("Initialised the console\n\n");
  if (test::runTests(kout::print)) {
    // Continue
    interrupts::init();
    kout::print("Installed interrupt handlers\n\n");
    if (!memeq(acpi::rsdp.signature, "RSD PTR ", 8)) {
      kpanic("No acpi found");
    }
    if (!acpi::rsdp.doChecksum()) {
      kpanic("Checksum of rsdp failed");
    }
    kout::print("Found rsdp (revision: ");
    kout::print(acpi::rsdp.revision);
    kout::print(")\n");
    void *rsdtAddress = (acpi::rsdp.revision >= 2)
                            ? (void *)(size_t)acpi::rsdp.xsdtAddress
                            : (void *)(size_t)acpi::rsdp.rsdtAddress;
    acpi::TableManager *rsdt = acpi::loadTable(rsdtAddress);
    if (rsdt == nullptr) {
      kpanic("Error loading rsdt");
    }
      kout::print("Found rsdt\n");
    kpanic("It all worked");
  } else {
    // The tests failed! Abort
    kpanic("The tests failed!");
  }
}
