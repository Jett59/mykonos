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
#include <mykonos/spinlock.h>

namespace processors {
struct MailboxEntry {
  lock::Spinlock lock;
  callback::Callback<bool> *callback;
  bool callerCanReturn;
};
static MailboxEntry mailboxes[MAX_CPUS];

void runOn(unsigned cpuNumber, callback::Callback<bool> &&callback) {
  if (cpuNumber < MAX_CPUS) {
    MailboxEntry &mailbox = mailboxes[cpuNumber];
    mailbox.lock.acquire();
    mailbox.callback = &callback;
    mailbox.callerCanReturn = false;
    // Make sure those modifications are done
    cpu::mfence();
    // Tell the CPU to start executing the request
    apic::localApic.sendIpi(PROCESSOR_CALLBACK_INTERRUPT, APIC_FIXED_IPI, false,
                            true, true, apic::localApicIds[cpuNumber]);
    // Wait for it to be handled
    while (!mailbox.callerCanReturn) {
      cpu::relax();
    }
    // Clean up
    mailbox.callback = nullptr;
    mailbox.lock.release();
  }
}
void letCallerReturn() {
  MailboxEntry &mailbox = mailboxes[cpu::getCpuNumber()];
  mailbox.callerCanReturn = true;
}
void receiveCall() {
  unsigned cpuNumber = cpu::getCpuNumber();
  if (mailboxes[cpuNumber].callback != nullptr) {
    (*mailboxes[cpuNumber].callback)();
  }
}
} // namespace processors
