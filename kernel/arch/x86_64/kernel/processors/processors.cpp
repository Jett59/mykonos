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
#include <mykonos/processors.h>

#include <mykonos/apic.h>
#include <mykonos/callback.h>
#include <mykonos/cpu.h>

namespace processors {
callback::Callback<void> *mailboxes[MAX_LOCAL_APICS];

void runOn(unsigned cpuNumber, callback::Callback<void> &&callback) {
  if (cpuNumber < MAX_LOCAL_APICS) {
    // Wait for all previous requests to complete
    while (true) {
      while (mailboxes[cpuNumber] != nullptr) {
        cpu::relax();
      }
      if (__sync_bool_compare_and_swap(&mailboxes[cpuNumber], nullptr,
                                       &callback)) {
        break;
      }
    }
    // Tell the CPU to start executing the request
    apic::localApic.sendIpi(PROCESSOR_CALLBACK_INTERRUPT, APIC_FIXED_IPI, false,
                            true, true, apic::localApicIds[cpuNumber]);
    // Wait for it to be handled
    while (!callback.hasRun()) {
      cpu::relax();
    }
    // Clean up
    mailboxes[cpuNumber] = nullptr;
  }
}
} // namespace processors
