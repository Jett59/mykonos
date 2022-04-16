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
#ifndef _MYKONOS_QUEUE_H
#define _MYKONOS_QUEUE_H

#include <mykonos/spinlock.h>

namespace util {
template <typename T> class QueueElement {
private:
  T value;
  QueueElement<T> *next;
};
template <typename T> class Queue {
private:
  lock::Spinlock lock;
  QueueElement<T> *head = nullptr;
  QueueElement<T> *tail = nullptr;

public:
  void push(const T &value) {
    auto newElement = new QueueElement<T>();
    newElement->value = value;
    lock.acquire();
    if (tail != nullptr) {
      tail->next = newElement;
      tail = newElement;
    } else {
      tail = head = newElement;
    }
    lock.release();
  }
  T pop() {
    lock.acquire();
    auto headElement = head;
    if (head == tail) {
      head = tail = nullptr;
    } else {
      head = head->next;
    }
    lock.release();
    T result = headElement->value;
    delete headElement;
    return result;
  }
};
} // namespace util

#endif