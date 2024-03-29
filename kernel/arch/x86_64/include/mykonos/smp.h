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
#ifndef _MYKONOS_SMP_H
#define _MYKONOS_SMP_H

#include <mykonos/hpet.h>
#include <stdint.h>

namespace smp {
bool startCpu(uint8_t apicId, hpet::Hpet& hpet);
// To allocate the stacks for all of the other CPUS. Call this before the first
// call of startCpu.
void allocateStacks(unsigned numLocalApics);

// Get the stack pointer allocated with allocateStacks for the given cpuNumber
void* getInitialStackPointer(unsigned cpuNumber);
}  // namespace smp

#endif