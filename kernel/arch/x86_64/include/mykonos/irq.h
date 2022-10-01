/*
    Copyright (C) 2022 Jett Thompson

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
#ifndef _MYKONOS_IRQ_H
#define _MYKONOS_IRQ_H

namespace irq {
enum class IrqClass {
  STANDARD,  // IOAPIC
  LOCAL_INTERRUPT,
  MSI
};
template <typename Context>
using IrqHandler = void (*)(Context*);

template <typename Context>
static inline void registerIrqHandler(IrqClass irqClass, unsigned irq, IrqHandler<Context> handler, Context* context, bool levelTriggered, bool activeHigh) {
  return registerIrqHandler<void>(irqClass, irq, (IrqHandler<void>)handler, (void*)context, levelTriggered, activeHigh);
}
template <>
void registerIrqHandler<void>(IrqClass irqClass, unsigned irq, IrqHandler<void> handler, void* context, bool levelTriggered, bool activeHigh);
}  // namespace irq

#endif