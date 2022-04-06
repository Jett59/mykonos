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
#ifndef _MYKONOS_SCHEDULER_H
#define _MYKONOS_SCHEDULER_H

#include <mykonos/task/controlBlock.h>

#define PRIORITY_BACKGROUND 0
#define PRIORITY_LOW 1
#define PRIORITY_NORMAL 2
#define PRIORITY_HIGH 3
#define PRIORITY_HIGHEST 4

namespace scheduler {
// You should release any spinlocks first.
void yield();

task::ControlBlock *currentTask();
// Returns the current task and removes it from the scheduler. Don't lose it!
task::ControlBlock *removeSelf();
// Set the current task state to 'blocking' and return it. Don't lose it!
task::ControlBlock *block();
// Block yield() on the current CPU.
void lock();
// Unblock yield() on the current CPU. Calls yield() if any yield() requests
// were blocked.
void unlock();

// Add the task to the scheduler. Used for blocking tasks mostly.
void addTask(task::ControlBlock *task);

// Call on startup to initialize the scheduler
void init(unsigned numCpus);
// Call on each CPU with a freshly allocated task::ControlBlock
void setInitialTask(task::ControlBlock *task);
// Call every 10ms for time slicing
void tick();
} // namespace scheduler

#endif