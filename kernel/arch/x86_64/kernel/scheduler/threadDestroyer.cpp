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
#include <mykonos/cpu.h>
#include <mykonos/processors.h>
#include <mykonos/scheduler.h>
#include <mykonos/stacks.h>
#include <mykonos/thread.h>

namespace thread {
static void *emergencyStacks[MAX_CPUS];
[[noreturn]] void destroy() {
  cpu::disableLocalIrqs();
  auto currentTask = scheduler::block();
  unsigned cpuNumber = cpu::getCpuNumber();
  if (emergencyStacks[cpuNumber] == nullptr) {
    emergencyStacks[cpuNumber] = stacks::allocateStack();
  }
  auto destructionCode = [](void *currentTaskPointer) {
    task::ControlBlock *currentTask = (task::ControlBlock *)currentTaskPointer;
    stacks::freeStack(currentTask->originalStackPointer);
    delete currentTask;
    scheduler::yield();
  };
  stacks::switchStack(emergencyStacks[cpuNumber], destructionCode,
                      (void *)currentTask);
}
} // namespace thread
