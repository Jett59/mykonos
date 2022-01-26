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
#include <mykonos/apicTimer.h>

#include <mykonos/apic.h>

namespace apic {
unsigned timerTicksPer(unsigned nanos, hpet::Hpet &hpet) {
  localApic.writeTimerLvt(false, false, APIC_TIMER_INTERRUPT);
  localApic.writeTimerDivideRegister(APIC_DIVIDE_16);
  localApic.writeTimerInitialCountRegister(0xffffffff);
  hpet.wait(nanos);
  localApic.writeTimerLvt(false, true, APIC_TIMER_INTERRUPT);
  uint32_t ticksPassed = 0xffffffff - localApic.getTimerCurrentCount();
  return ticksPassed * 16;
}
void setUpTimer(unsigned tickFrequency) {
  apic::localApic.writeTimerLvt(true, false, APIC_TIMER_INTERRUPT);
  apic::localApic.writeTimerDivideRegister(APIC_DIVIDE_16);
  apic::localApic.writeTimerInitialCountRegister(tickFrequency);
}
} // namespace apic
