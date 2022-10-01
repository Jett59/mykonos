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
#include <mykonos/apicTimer.h>
#include <mykonos/irq.h>
#include <mykonos/kpanic.h>
#include <mykonos/processors.h>
#include <mykonos/scheduler.h>

#include <stdint.h>

#define MAX_IRQ_SHARING 16

namespace irq {
struct IrqHandlerAndContext {
  IrqHandler<void> handler;
  void* context;
};

static IrqHandlerAndContext irqHandlers[256][MAX_IRQ_SHARING];

void registerIrqHandlerBase(IrqClass irqClass,
                            unsigned irq,
                            IrqHandler<void> handler,
                            void* context,
                            bool levelTriggered,
                            bool activeHigh) {
  switch (irqClass) {
    case IrqClass::STANDARD:
      apic::mapIoApicInterrupt(irq, irq + IO_APIC_INTERRUPT_BASE, levelTriggered, activeHigh,
                               0);  // TODO: IRQ balancing.
      apic::unmaskIoApicInterrupt(irq);
      irq += IO_APIC_INTERRUPT_BASE;
      break;
    case IrqClass::LOCAL_INTERRUPT:
      break;
    case IrqClass::MSI:
      irq += IO_APIC_INTERRUPT_BASE + apic::getGsiCount();
      break;
    default:
      kpanic("Invalid IRQ class");
  }
  if (irq >= 256) {
    kpanic("IRQ out of range");
  }
  for (int i = 0; i < MAX_IRQ_SHARING; i++) {
    if (irqHandlers[irq][i].handler == nullptr) {
      irqHandlers[irq][i].handler = handler;
      irqHandlers[irq][i].context = context;
      break;
    }
  }
}
}  // namespace irq

extern "C" void handleInterrupt(uint8_t interruptNumber) {
  switch (interruptNumber) {
    case APIC_TIMER_INTERRUPT: {
      apic::localApic.eoi();
      scheduler::tick();
      break;
    }
    case PROCESSOR_CALLBACK_INTERRUPT: {
      apic::localApic.eoi();
      processors::receiveCall();
      break;
    }
    default:
      for (int i = 0; i < MAX_IRQ_SHARING; i++) {
        if (irq::irqHandlers[interruptNumber][i].handler != nullptr) {
          irq::irqHandlers[interruptNumber][i].handler(irq::irqHandlers[interruptNumber][i].context);
        } else {
          break;
        }
      }
      if (apic::localApic.inService(interruptNumber)) {
        apic::localApic.eoi();
      }
      break;
  }
}
