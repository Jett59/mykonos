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
// Save current registers into *from and load the ones in *to into the CPU.
// Calls unlockTask() after saving the current state and lockTask() before
// restoring the new state
void swapRegisters(task::Registers *from, task::Registers *to);
// Just load *regs into the CPU.
// Calls lockTask() immediately
void restoreRegisters(task::Registers *regs);

void unlockTask(task::ControlBlock *task) { task->runLock.release(); }
void lockTask(task::ControlBlock *task) { task->runLock.acquire(); }
}

namespace scheduler {
class Scheduler {
private:
  unsigned cpuNumber;
  task::Queue tasks;
  task::ControlBlock *currentTask;
  lock::Spinlock addTaskLock;
  bool yieldLocked = false;
  bool yieldPostponed = false;

public:
  void addTask(task::ControlBlock *task) {
    task->state = task::State::RUNNABLE;
    addTaskLock.acquire();
    if (currentTask == nullptr || currentTask->state != task::State::RUNNING ||
        currentTask->priority < task->priority) {
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
    if (!yieldLocked && currentTask != nullptr &&
        currentTask->state == task::State::RUNNING &&
        --currentTask->timeSlice == 0) {
      yield();
    }
  }
  void yield() {
    if (!yieldLocked) {
      bool enableLocalIrqs = cpu::localIrqState();
      cpu::disableLocalIrqs();
      task::ControlBlock *from = currentTask;
      task::ControlBlock *to = tasks.pop();
      if (to != nullptr) {
        to->state = task::State::RUNNING;
        to->timeSlice = INITIAL_TIME_SLICE;
        currentTask = to;
        if (from != nullptr) {
          if (from->state == task::State::RUNNING) {
            from->state = task::State::RUNNABLE;
            tasks.push(from);
          }
          swapRegisters(&from->registers, &to->registers);
        } else {
          restoreRegisters(&to->registers);
        }
      } else if (currentTask == nullptr) {
        cpu::haultWithIrqs();
      } else if (currentTask->state != task::State::RUNNING) {
        auto currentTask = this->currentTask;
        // Save the registers
        swapRegisters(&currentTask->registers, &currentTask->registers);
        // This will be run twice: once after swapRegisters() returns, and once
        // when we are unblocked
        if (currentTask->state != task::State::RUNNING) {
          // swapRegisters() returned
          removeCurrent()->runLock.release();
          cpu::haultWithIrqs();
        }else {
          // Unblocked!
        }
      }
      if (enableLocalIrqs) {
        cpu::enableLocalIrqs();
      }
    } else {
      yieldPostponed = true;
    }
  }

  void lock() {
    yieldLocked = true;
    yieldPostponed = false;
  }
  void unlock() {
    yieldLocked = false;
    if (yieldPostponed) {
      yieldPostponed = false;
      yield();
    }
  }

  task::ControlBlock *getCurrentTask() { return currentTask; }

  task::ControlBlock *removeCurrent() {
    task::ControlBlock *result = currentTask;
    currentTask = nullptr;
    return result;
  }
  task::ControlBlock *block() {
    currentTask->state = task::State::BLOCKING;
    return currentTask;
  }

  unsigned taskCount() {
    if (currentTask == nullptr || currentTask->state != task::State::RUNNING) {
      return tasks.getSize();
    } else {
      return tasks.getSize() + 1;
    }
  }

  void setInitialTask(task::ControlBlock *task) {
    if (currentTask == nullptr) {
      bool enableLocalIrqs = cpu::localIrqState();
      task->runLock.acquire();
      if (enableLocalIrqs) {
        cpu::enableLocalIrqs();
      }
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
task::ControlBlock *removeSelf() {
  return schedulers[cpu::getCpuNumber()].removeCurrent();
}
task::ControlBlock *block() { return schedulers[cpu::getCpuNumber()].block(); }
void lock() { schedulers[cpu::getCpuNumber()].lock(); }
void unlock() { schedulers[cpu::getCpuNumber()].unlock(); }

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
