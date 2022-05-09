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
#ifndef _MYKONOS_UTIL_H
#define _MYKONOS_UTIL_H

#include <stddef.h>

namespace util {
template <typename T> struct WithoutReference { using Type = T; };
template <typename T> struct WithoutReference<T &> { using Type = T; };
template <typename T> struct WithoutReference<T &&> { using Type = T; };

template <typename T> typename WithoutReference<T>::Type &&move(T &&value) {
  return (typename WithoutReference<T>::Type &&) value;
}
template <typename T> void move(T *dst, const T *src, size_t n) {
  for (size_t i = 0; i < n; i++) {
    dst[i] = move(src[i]);
  }
}

template <typename T> class Accessor {
public:
  Accessor(T *value = nullptr) : value(value) {}

  operator T() { return value != nullptr ? *value : T{}; }
  Accessor<T> &operator=(T newValue) {
    if (value != nullptr) {
      *value = newValue;
    }
    return *this;
  }

private:
  T *value;
};
} // namespace util

#endif