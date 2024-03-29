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
#include <mykonos/display.h>
#include <mykonos/fontRenderer.h>

#define FOREGROUND \
  { .r = 0xFF, .g = 0xFF, .b = 0xFF }
#define BACKGROUND \
  { .r = 0, .g = 0, .b = 0 }

namespace display {
void writeCharacter(unsigned x, unsigned y, char c) {
  font::render(c, x, y, FOREGROUND, BACKGROUND);
}
void writeString(unsigned x, unsigned y, const char* str) {
  for (unsigned i = 0; str[i] != 0; i++) {
    font::render(str[i], x + i * font::getWidth(), y, FOREGROUND, BACKGROUND);
  }
}
void removeCharacter(unsigned x, unsigned y) {
  font::render(' ', x, y, FOREGROUND, BACKGROUND);
}
}  // namespace display
