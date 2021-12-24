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
#include <kout.h>

#include <display.h>
#include <fontRenderer.h>

namespace kout {
static unsigned line, column;
static unsigned lines, columns;
void print(const char* str) {
    if (columns == 0) {
      columns = display::getWidth() / font::getWidth();
      lines = display::getHeight() / font::getHeight();
    }
  unsigned x = column * font::getWidth();
  unsigned y = line * font::getHeight();
  while (*str != 0) {
    display::writeCharacter(x, y, *str);
    str++;
    x += font::getWidth();
    column++;
    if (column >= columns) {
      column = x = 0;
      line++;
      y += font::getHeight();
    }
  }
}
}
