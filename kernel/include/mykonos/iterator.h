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
#ifndef _MYKONOS_ITERATOR_H
#define _MYKONOS_ITERATOR_H

#include <mykonos/util.h>
#include <stddef.h>

namespace util {
template <typename T> class RangedIterator {
public:
  RangedIterator(T *begin, T *end, T *pointer)
      : begin(begin), end(end), pointer(pointer) {}

  using DiffType = ptrdiff_t;

  Accessor<T> operator[](DiffType index) const {
    return pointer + index < end && pointer + index >= begin ? pointer + index
                                                             : nullptr;
  }
  T &operator*() { return (*this)[0]; }

  RangedIterator<T> operator++(int) {
    RangedIterator<T> result = *this;
    pointer++;
    return result;
  }
  RangedIterator<T> &operator++() {
    pointer++;
    return *this;
  }
  RangedIterator<T> operator+(DiffType offset) const {
    return {begin, end, pointer + offset};
  }
  RangedIterator<T> operator--(int) {
    RangedIterator<T> result = *this;
    pointer--;
    return result;
  }
  RangedIterator<T> &operator--() {
    pointer--;
    return *this;
  }
  RangedIterator<T> operator-(DiffType offset) const {
    return {begin, end, pointer - offset};
  }

  DiffType operator-(const RangedIterator<T> &other) const {
    if (other.begin == begin && other.end == end) {
      return pointer - other.pointer;
    } else {
      return -1;
    }
  }

  bool operator==(const RangedIterator<T> &other) const {
    return other.begin == begin && other.end == end && other.pointer == pointer;
  }
  bool operator!=(const RangedIterator<T> &other) const {
    return !((*this) == other);
  }

private:
  T *begin;
  T *end;
  T *pointer;
};
} // namespace util

#endif