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
#include <mykonos/kout.h>

#include <stdarg.h>

#include <mykonos/string.h>

namespace kout {
void printf(String format, ...) {
  va_list args;
  va_start(args, format);
  acquireConsoleLock();
  while (format.len() > 0) {
    if (*format == '%') {
      format = format.subString(1);
      switch (*format) {
      case 'n': {
        break;
      }
      case '%': {
        print("%", true);
        break;
      }
      case 'c': {
        char c = (char)va_arg(args, int);
        print(String(&c, 1), true);
        break;
      }
      case 'd': {
        int n = va_arg(args, int);
        print(n, 10, true);
        break;
      }
      case 'l': {
        long l = va_arg(args, long);
        print(l, 10, true);
        break;
      }
      case 'o': {
        long l = va_arg(args, long);
        print(l, 8, true);
        break;
      }
      case 'p': {
        void *ptr = va_arg(args, void *);
        print("0x", true);
        print((unsigned long)ptr, 16, true);
        break;
      }
      case 's': {
        const char *str = va_arg(args, const char *);
        print(str, true);
        break;
      }
      case 'x': {
        long l = va_arg(args, long);
        print(l, 16, true);
        break;
      }
      default:
        print("<unknown type specifier>", true);
      }
      format = format.subString(1);
    } else {
      print(format.findBefore('%'), true);
      format = format.findNext('%');
    }
  }
  releaseConsoleLock();
  va_end(args);
}
} // namespace kout
