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
#ifndef _MYKONOS_TASK_TASK_QUEUE_H
#define _MYKONOS_TASK_TASK_QUEUE_H

#include <mykonos/spinlock.h>
#include <mykonos/task/controlBlock.h>

namespace task {
class Queue {
public:
  Queue() = default;
  Queue(const Queue &other) = delete;
  Queue &operator=(Queue &other) = delete;

  void push(ControlBlock *value) {
    lock.acquire();
    if (head == nullptr) {
      head = tail = value;
    } else {
      tail->next = value;
      tail = value;
    }
    size++;
    lock.release();
  }
  ControlBlock *pop() {
    lock.acquire();
    ControlBlock *result = head;
    if (head != nullptr) {
      if (head == tail) {
        head = tail = nullptr;
      } else {
        head = head->next;
      }
      size--;
    }
    lock.release();
    return result;
  }
  unsigned getSize() {
    lock.acquire();
    unsigned result = size;
    lock.release();
    return result;
  }

private:
  lock::Spinlock lock;
  ControlBlock *head = nullptr;
  ControlBlock *tail = nullptr;
  unsigned size;
};
} // namespace task

#endif