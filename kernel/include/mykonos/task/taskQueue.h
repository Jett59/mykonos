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
#include <mykonos/util.h>

namespace task {
class Queue {
 public:
  Queue() = default;
  Queue(const Queue& other) = delete;
  Queue& operator=(Queue& other) = delete;

  // Move constructors.
  Queue(Queue&& other) {
    lock = util::move(other.lock);
    head = other.head;
    tail = other.tail;
    size = other.size;
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
  }
  Queue& operator=(Queue&& other) {
    lock = util::move(other.lock);
    head = other.head;
    tail = other.tail;
    size = other.size;
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
    return *this;
  }

  void push(ControlBlock* value) {
    lock.acquire();
    if (head == nullptr) {
      head = tail = value;
    } else {
      tail->next = value;
      tail = value;
    }
    __atomic_fetch_add(&size, 1, __ATOMIC_SEQ_CST);
    lock.release();
  }
  void push_front(ControlBlock* value) {
    lock.acquire();
    if (head == nullptr) {
      head = tail = value;
    } else {
      value->next = head;
      head = value;
    }
    __atomic_fetch_add(&size, 1, __ATOMIC_SEQ_CST);
    lock.release();
  }
  ControlBlock* pop() {
    lock.acquire();
    ControlBlock* result = head;
    if (head != nullptr) {
      if (head == tail) {
        head = tail = nullptr;
      } else {
        head = head->next;
      }
      __atomic_fetch_sub(&size, 1, __ATOMIC_SEQ_CST);
    }
    lock.release();
    return result;
  }
  unsigned getSize() { return __atomic_load_n(&size, __ATOMIC_SEQ_CST); }

 private:
  lock::Spinlock lock;
  ControlBlock* head = nullptr;
  ControlBlock* tail = nullptr;
  unsigned size = 0;
};
}  // namespace task

#endif