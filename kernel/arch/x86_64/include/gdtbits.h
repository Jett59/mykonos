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
/*gdtbits.h: definitions of the bits in the gdt (global descriptor table)*/

/*Note: this may be included by assembly code, assuming .S suffix*/

   #ifndef _GDTBITS_H
   #define _GDTBITS_H

#define GDT_TYPE(X) ((X & 0xF) << 40)
#define GDT_WRITABLE (1 << 41)
#define GDT_CONFORMING (1 << 42)
#define GDT_EXECUTABLE (1 << 43)
#define GDT_NOT_SYSTEM (1 << 44)
#define GDT_PRIVILEGE(X)  ((X & 3) << 45)
#define GDT_PRESENT (1 << 47)
#define GDT_LONG (1 << 53)
#define GDT_SIZE (1 << 54)

   #endif