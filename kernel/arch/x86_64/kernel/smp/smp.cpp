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
#include <smp.h>

#include <apic.h>

#include <stacks.h>

extern "C" {
volatile uint8_t runningCpus = 1;

void *initialStackPointers[MAX_LOCAL_APICS];
}

#define SMP_TRAMPOLINE_PAGE 8

namespace smp {
bool startCpu(uint8_t apicId, hpet::Hpet &hpet) {
  uint8_t previousRunningCpus = runningCpus;
  // Send the init ipi
  apic::localApic.clearErrors();
  apic::localApic.sendIpi(0, APIC_INIT_IPI, false, true, false, apicId);
  // Wait for 10 milliseconds
  hpet.wait(10000000);
  // Send the first sipi
  apic::localApic.clearErrors();
  apic::localApic.sendIpi(SMP_TRAMPOLINE_PAGE, APIC_STARTUP_IPI, false, true,
                          false, apicId);
  // Wait for 1 millisecond
  hpet.wait(1000000);
  // Check if the cpu incremented the runningCpus counter
  if (runningCpus > previousRunningCpus) {
    return true;
  } else {
    // Send the second sipi
    apic::localApic.clearErrors();
    apic::localApic.sendIpi(SMP_TRAMPOLINE_PAGE, APIC_STARTUP_IPI, false, true,
                            false, apicId);
    // Poll for an incremented runningCpus with a timeout of 1 second
    uint64_t pollStart = hpet.nanoTime();
    while (runningCpus == previousRunningCpus &&
           hpet.nanoTime() < pollStart + 1000000000) {
      cpu::relax();
    }
  }
  return previousRunningCpus < runningCpus;
}
void allocateStacks(unsigned numLocalApics) {
  // Allocate stacks for CPUs 1..numLocalApics - 1 (not for the bsp)
  for (unsigned i = 1; i < numLocalApics; i++) {
    initialStackPointers[i] = stacks::allocateStack();
  }
}
} // namespace smp
