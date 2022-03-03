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
#include <mykonos/cpu.h>
#include <mykonos/kpanic.h>
#include <mykonos/processors.h>
#include <mykonos/scheduler.h>
#include <mykonos/task/controlBlock.h>
#include <mykonos/task/taskQueue.h>

#define INITIAL_TIME_SLICE 5

extern "C" {
// Assembly function
void swapRegisters(task::Registers *from, task::Registers *to);
}

namespace scheduler {
class Scheduler {
private:
  task::Queue tasks;
  task::ControlBlock *currentTask;

public:
  void addTask(task::ControlBlock *task) { tasks.push(task); }
  void tick() {
    if (--currentTask->timeSlice == 0) {
      yield();
    }
  }
  void yield() {
    task::ControlBlock *from = currentTask;
    task::ControlBlock *to = tasks.pop();
    tasks.push(from);
    to->timeSlice = INITIAL_TIME_SLICE;
    currentTask = to;
    swapRegisters(&from->registers, &to->registers);
  }

  void setInitialTask(task::ControlBlock *task) {
    if (currentTask == nullptr) {
      currentTask = task;
    } else {
      kpanic("Cannot set initial task twice");
    }
  }
};
static Scheduler schedulers[MAX_CPUS];

static unsigned cpuCount = 0;

void addTask(task::ControlBlock *task) {
  schedulers[cpu::getCpuNumber()].addTask(task);
}
void tick() { schedulers[cpu::getCpuNumber()].tick(); }
void yield() { schedulers[cpu::getCpuNumber()].yield(); }

void init(unsigned numCpus) { cpuCount = numCpus; }
void setInitialTask(task::ControlBlock *task) {
  schedulers[cpu::getCpuNumber()].setInitialTask(task);
}
} // namespace scheduler
