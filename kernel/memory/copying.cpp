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
#include <string.h>

extern "C" void *memset(void *str, int c, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((unsigned char *)str)[i] = (unsigned char)c;
  }
  return str;
}
extern "C" void *memcpy(void *dst, const void *src, size_t size) {
  for (size_t i = 0; i < size; i++) {
    ((unsigned char *)dst)[i] = ((unsigned char *)src)[i];
  }
  return dst;
}