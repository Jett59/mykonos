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
#ifndef _MYKONOS_VECTOR_H
#define _MYKONOS_VECTOR_H

#include <mykonos/iterator.h>
#include <mykonos/util.h>
#include <stddef.h>

namespace util {
#define MINIMUM_VECTOR_SIZE 16
template <typename T>
class Vector {
 public:
  Vector() : capacity(0), data(nullptr) {}
  Vector(size_t initialCapacity) : capacity(initialCapacity), data(new T[initialCapacity]) {}

  Vector(const Vector&) = delete;
  Vector& operator=(const Vector&) = delete;

  Vector(Vector&& other) : capacity(other.capacity), size(other.size), data(other.data) {
    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
  }
  Vector& operator=(Vector&& other) {
    if (data != nullptr) {
      delete[] data;
    }
    data = other.data;
    capacity = other.capacity;
    size = other.size;
    other.data = nullptr;
    other.capacity = 0;
    other.size = 0;
    return *this;
  }

  ~Vector() {
    if (data != nullptr) {
      delete[] data;
    }
  }

  Accessor<T> operator[](size_t offset) const { return begin()[offset]; }

  using Iterator = RangedIterator<T>;

  Iterator begin() const { return {data, data + size, data}; }
  Iterator end() const { return {data, data + size, data + size}; }

  void push_back(T value) {
    size_t index = size;
    if (index >= capacity) {
      if (capacity < MINIMUM_VECTOR_SIZE) {
        resize(MINIMUM_VECTOR_SIZE);
      } else {
        resize(capacity * 2);
      }
    }
    data[index] = move(value);
    size++;
  }
  T pop_back() {
    size_t index = --size;
    T result = move(data[index]);
    if (size * 2 <= capacity && size > MINIMUM_VECTOR_SIZE) {
      resize(size);
    } else if (size == 0) {
      delete[] data;
      data = nullptr;
      capacity = 0;
    }
    return move(result);
  }

  size_t getSize() const { return size; }

 private:
  size_t capacity;
  size_t size = 0;
  T* data;

  void resize(size_t newCapacity) {
    T* newData = new T[newCapacity];
    if (data != nullptr) {
      move(newData, data, size);
      delete[] data;
    }
    data = newData;
    capacity = newCapacity;
  }
};
}  // namespace util

#endif