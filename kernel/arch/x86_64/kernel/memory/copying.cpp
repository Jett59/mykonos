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

extern "C" void *memset(void *str, int c, size_t n) {
  void *tmpStr = str;
  __asm__ volatile("rep stosq"
                   : "+D"(tmpStr)
                   : "a"((unsigned char)c * 0x0101010101010101), "c"(n / 8)
                   : "memory");
  __asm__ volatile("rep stosb"
                   :
                   : "D"(tmpStr), "c"(n & 7), "a"((unsigned char)c)
                   : "memory");
  return str;
}
extern "C" void *memcpy(void *dst, const void *src, size_t n) {
  void *tmpDst = dst;
  const void *tmpSrc = src;
  __asm__ volatile("rep movsq"
                   : "+D"(tmpDst), "+S"(tmpSrc)
                   : "c"(n / 8)
                   : "memory");
  __asm__ volatile("rep movsb"
                   :
                   : "D"(tmpDst), "S"(tmpSrc), "c"(n & 7)
                   : "memory");
  return dst;
}
extern "C" int memcmp(const void *a, const void *b, size_t n) {
  size_t finalN = n;
  __asm__ volatile("repe cmpsb" : "+c"(finalN) : "D"(a), "S"(b) : "memory");
  if (finalN == 0) {
    return 0;
  } else {
    return ((unsigned char *)a)[n - finalN] - ((unsigned char *)b)[n - finalN];
  }
}
