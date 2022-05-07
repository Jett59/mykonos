/*
    Copyright (C) 2021  Jett Thompson

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
#ifndef _MYKONOS_STRING_H
#define _MYKONOS_STRING_H

#include <mykonos/iterator.h>
#include <stddef.h>

extern "C" {
void *memset(void *str, int c, size_t size);
void *memcpy(void *dst, const void *src, size_t size);
int memcmp(const void *a, const void *b, size_t n);
void *memchr(const void *str, int c, size_t n);
int strlen(const char *str);
char *strcpy(char *dst, const char *src);
char *strchr(const char *str, int c);
}

static inline bool memeq(const void *a, const void *b, size_t n) {
  return memcmp(a, b, n) == 0;
}

class String {
public:
  String() : cString(nullptr), length(0) {}
  String(const char *cString) : cString(cString), length(strlen(cString)) {}
  String(const char *cString, size_t length)
      : cString(cString), length(length) {}

  size_t len() const { return length; }

  char operator*() const { return (*this)[0]; }
  char operator[](size_t index) const {
    return index < length ? cString[index] : 0;
  }

  using Iterator = util::RangedIterator<const char>;

  Iterator begin() const { return {cString, cString + length, cString}; }
  Iterator end() const { return {cString, cString + length, cString + length}; }

  String subString(size_t begin, size_t end) const {
    if (begin > length || begin > end) {
      return {};
    } else {
      if (end > length) {
        end = length;
      }
      return String(cString + begin, end - begin);
    }
  }
  String subString(size_t begin) const { return subString(begin, length); }

  String findBefore(char delimiter) const {
    return subString(0, length - findNext(delimiter).length);
  }

  String findNext(char c) const {
    if (length == 0) {
      return {};
    }
    const char *match = (const char *)memchr(cString + 1, c, length);
    if (match == nullptr) {
      return {};
    } else {
      return String(match, cString + length - match);
    }
  }

  bool operator==(nullptr_t) const { return cString == nullptr; }
  bool operator!=(nullptr_t) const { return cString != nullptr; }

  bool operator==(const String &other) const {
    if (other.length != length) {
      return false;
    } else if (length == 0) {
      return true;
    } else {
      return memeq((const void *)cString, (const void *)other.cString, length);
    }
  }
  bool operator!=(const String &other) const { return !(*this == other); }

private:
  const char *cString;
  size_t length;
};

static inline String operator"" _s(const char *str) { return String(str); }

#endif