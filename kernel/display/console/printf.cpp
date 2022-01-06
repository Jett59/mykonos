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
#include <kout.h>

#include <stdarg.h>

#include <string.h>

namespace kout {
void printf(const char *format, ...) {
  va_list args;
  va_start(args, format);
  while (*format != 0) {
      if (*format == '%') {
        format++;
        switch (*format) {
            case 'n': {
              break;
            }
            case '%': {
              print("%", 1);
              break;
            }
            case 'c': {
              char c = (char)va_arg(args, int);
              print(&c, 1);
              break;
            }
            case 'd': {
              int n = va_arg(args, int);
              print(n);
              break;
            }
            case 'l': {
              long l = va_arg(args, long);
              print(l);
              break;
            }
            case 'o': {
              long l = va_arg(args, long);
              print(l, 8);
              break;
            }
            case 'p': {
              void *ptr = va_arg(args, void *);
              print("0x");
              print((unsigned long)ptr, 16);
              break;
            }
              case 's': {
                const char *str = va_arg(args, const char *);
                print(str);
                break;
              }
              case 'x': {
                long l = va_arg(args, long);
                print(l, 16);
                break;
              }
              default:
                print("<unknown type specifier>");
            }
      }else {
        const char *nextSpecifier = strchr(format, '%');
        int len;
        if (nextSpecifier != 0) {
          len = nextSpecifier - format;
        }else {
          len = strlen(format);
        }
        print(format, len);
      }
      format++;
  }
  va_end(args);
}
} // namespace kout
