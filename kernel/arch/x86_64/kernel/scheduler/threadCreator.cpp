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
#include <mykonos/thread.h>

#include <mykonos/scheduler.h>
#include <mykonos/stacks.h>
#include <mykonos/task/controlBlock.h>

namespace thread {
void create(void (*entrypoint)(void* context),
            void* context,
            unsigned priority) {
  void* stack = stacks::allocateStack();
  task::ControlBlock* task = new task::ControlBlock();
  task->originalStackPointer = stack;
  task->priority = priority;
  task->registers.rip = (void*)entrypoint;
  task->registers.rdi = (uint64_t)context;
  task->registers.rflags = 1 << 9;  // Interrupt enable bit
  task->registers.rsp = (uint64_t)stack;
  void* cr3;
  __asm__("movq %%cr3, %0" : "=r"(cr3));
  task->registers.cr3 = cr3;
  scheduler::addTask(task);
}
}  // namespace thread
