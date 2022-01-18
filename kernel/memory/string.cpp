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
#include <mykonos/string.h>

extern "C" int strlen(const char *str) {
  int i;
  for (i = 0; *str != 0; i++) {
    str++;
  }
  return i;
}
extern "C" char *strcpy(char *dst, const char *src) {
  int size = strlen(src);
  memcpy(dst, src, size);
  dst[size] = 0;
  return dst;
}
extern "C" char *strchr(const char *str, int c) {
  char chr = (char)c;
  int len = strlen(str);
  for (int i = 0; i < len; i++) {
    if (str[i] == chr) {
      return (char *)str + i;
    }
  }
  return 0;
}
