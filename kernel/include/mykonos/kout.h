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
#ifndef _MYKONOS_KOUT_H
#define _MYKONOS_KOUT_H

#include <mykonos/string.h>

namespace kout {
// Don't skip locking unless you are printf.
void print(const char *str, int len, bool skipLocking = false);
static inline void print(const char *str) { print(str, strlen(str)); }
void print(unsigned long value, unsigned long base = 10,
           bool skipLocking = false);

// Don't use unless you are printf
void acquireConsoleLock();
void releaseConsoleLock();

void printf(const char *format, ...);
} // namespace kout

#endif