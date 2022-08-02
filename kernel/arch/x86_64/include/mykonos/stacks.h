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
#ifndef _MYKONOS_STACKS_H
#define _MYKONOS_STACKS_H

#include <mykonos/kmalloc.h>

#define DEFAULT_STACK_SIZE 16384

namespace stacks {
// Call freeStack() to free the stack. Do not use kfree!
static inline void* allocateStack() {
  return (void*)((uint8_t*)memory::allocateMemory(DEFAULT_STACK_SIZE) +
                 DEFAULT_STACK_SIZE);
}
// stackPointer is a pointer returned by allocateStack().
static inline void freeStack(void* stackPointer) {
  memory::freeMemory((void*)((uint8_t*)stackPointer - DEFAULT_STACK_SIZE),
                     DEFAULT_STACK_SIZE);
}

// Run the callback on another stack. Will never return.
extern "C" [[noreturn]] void switchStack(void* newStack,
                                         [[noreturn]] void (*callback)(void*),
                                         void* context);
}  // namespace stacks

#endif