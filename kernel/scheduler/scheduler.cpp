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
#include <mykonos/spinlock.h>
#include <mykonos/task/controlBlock.h>
#include <mykonos/task/taskQueue.h>

#define INITIAL_TIME_SLICE 5

extern "C" {
// Assembly functions
// Save current registers into *from and load the ones in *to into the CPU.
void swapRegisters(task::Registers *from, task::Registers *to);
// Just load *regs into the CPU.
void restoreRegisters(task::Registers *regs);
}

namespace scheduler {
class Scheduler {
private:
  unsigned cpuNumber;
  task::Queue tasks;
  task::ControlBlock *currentTask;
  lock::Spinlock addTaskLock;

public:
  void addTask(task::ControlBlock *task) {
    addTaskLock.acquire();
    if (currentTask == nullptr || currentTask->priority < task->priority) {
      tasks.push_front(task);
      if (cpuNumber == cpu::getCpuNumber()) {
        addTaskLock.release();
        yield();
      } else {
        auto codeToRun = []() -> bool {
          processors::letCallerReturn();
          scheduler::yield();
          return true;
        };
        addTaskLock.release();
        processors::runOn(
            cpuNumber, callback::Lambda<decltype(codeToRun), bool>(codeToRun));
      }
    } else {
      tasks.push(task);
      addTaskLock.release();
    }
  }
  void tick() {
    if (currentTask != nullptr && --currentTask->timeSlice == 0) {
      yield();
    }
  }
  void yield() {
    bool enableLocalIrqs = cpu::localIrqState();
    cpu::disableLocalIrqs();
    task::ControlBlock *from = currentTask;
    task::ControlBlock *to = tasks.pop();
    if (to != nullptr) {
      to->timeSlice = INITIAL_TIME_SLICE;
      currentTask = to;
      if (from != nullptr) {
        tasks.push(from);
        swapRegisters(&from->registers, &to->registers);
      } else {
        restoreRegisters(&to->registers);
      }
    }else if (currentTask == nullptr) {
      cpu::haultWithIrqs();
    }
    if (enableLocalIrqs) {
      cpu::enableLocalIrqs();
    }
  }

  task::ControlBlock *getCurrentTask() { return currentTask; }

  task::ControlBlock *block() {
    task::ControlBlock *result =
        __atomic_load_n(&currentTask, __ATOMIC_SEQ_CST);
    __atomic_store_n(&currentTask, nullptr, __ATOMIC_SEQ_CST);
    return result;
  }

  unsigned taskCount() {
    if (currentTask == nullptr) {
      return tasks.getSize();
    } else {
      return tasks.getSize() + 1;
    }
  }

  void setInitialTask(task::ControlBlock *task) {
    if (currentTask == nullptr) {
      currentTask = task;
    } else {
      kpanic("Cannot set initial task twice");
    }
  }

  void setCpuNumber(unsigned cpuNumber) { this->cpuNumber = cpuNumber; }
};
static Scheduler schedulers[MAX_CPUS];

static unsigned cpuCount = 0;

static Scheduler &getLeastBusy() {
  Scheduler *bestScheduler = &schedulers[cpu::getCpuNumber()];
  unsigned bestTaskCount = bestScheduler->taskCount();
  for (unsigned i = 0; i < cpuCount; i++) {
    if (bestTaskCount == 0) {
      break;
    }
    if (schedulers[i].taskCount() < bestTaskCount) {
      bestScheduler = &schedulers[i];
      bestTaskCount = bestScheduler->taskCount();
    }
  }
  return *bestScheduler;
}

void addTask(task::ControlBlock *task) { getLeastBusy().addTask(task); }
void tick() { schedulers[cpu::getCpuNumber()].tick(); }
void yield() { schedulers[cpu::getCpuNumber()].yield(); }
task::ControlBlock *currentTask() {
  return schedulers[cpu::getCpuNumber()].getCurrentTask();
}
task::ControlBlock *block() { return schedulers[cpu::getCpuNumber()].block(); }

void init(unsigned numCpus) {
  cpuCount = numCpus;
  for (unsigned i = 0; i < numCpus; i++) {
    schedulers[i].setCpuNumber(i);
  }
}
void setInitialTask(task::ControlBlock *task) {
  schedulers[cpu::getCpuNumber()].setInitialTask(task);
}
} // namespace scheduler
