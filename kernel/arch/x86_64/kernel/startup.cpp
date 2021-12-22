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

#include <mbi.h>
#include <pageTableInit.h>

#include <fontRenderer.h>
#include <frameBuffer.h>

typedef void (*ConstructorOrDestructor)();

extern "C" {
extern ConstructorOrDestructor __init_array_start[0], __init_array_end[0];
extern ConstructorOrDestructor __fini_array_start[0], __fini_array_end[0];
}

extern "C" [[noreturn]] void kstart() {
  // Global constructors must be called
  // We use init_array method, for simplicity
  for (ConstructorOrDestructor *initArrayElement = __init_array_start;
       initArrayElement != __init_array_end; initArrayElement++) {
    (*initArrayElement)();
  }
  // Now that that's over
  multiboot::parseMbi();
  paging::initPageTables();
  display::initFrameBuffer();
  font::render('A', 20, 20, {.r = 0xFF, .g = 0xff, .b = 0}, {});
  while (true) {
    __asm__("hlt");
  }
}
