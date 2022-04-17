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
#include <mykonos/cleaner.h>
#include <mykonos/cpu.h>
#include <mykonos/kpanic.h>
#include <mykonos/processors.h>
#include <mykonos/scheduler.h>
#include <mykonos/stacks.h>
#include <mykonos/thread.h>

namespace thread {
[[noreturn]] void destroy() {
  cpu::disableLocalIrqs();
  auto currentTask = scheduler::block();
  auto destructionCode = [](void *currentTaskPointer) {
    task::ControlBlock *currentTask = (task::ControlBlock *)currentTaskPointer;
    // Use the runLock to wait for the task to finish running
    currentTask->runLock.acquire();
    currentTask->runLock.release();
    if (currentTask->originalStackPointer != nullptr) {
      stacks::freeStack(currentTask->originalStackPointer);
    }
    delete currentTask;
  };
  cleaner::addObject((void *)currentTask, destructionCode);
  scheduler::yield();
  // Make the compiler happy about noreturn
  kpanic("thread::destroy somehow returned!?\n");
}
} // namespace thread
