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
void addTask(task::ControlBlock *task);
void yield();

void init(unsigned numCpus);
void setInitialTask(task::ControlBlock *task);
void tick();
} // namespace scheduler

#endif