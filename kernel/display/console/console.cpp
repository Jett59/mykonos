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

#include <kmalloc.h>

#include <string.h>

namespace kout {
static unsigned line, column;
static unsigned lines, columns;

static char *screenBuffer;
#define SCREEN_BUFFER_CHARACTER(COLUMN, LINE)                                  \
  (screenBuffer[(LINE) * (columns + 1) + (COLUMN)])
#define SCREEN_BUFFER_LINE(LINE) (&screenBuffer[(LINE) * (columns + 1)])

void scrollDown() {
  unsigned x = 0;
  unsigned y = 0;
  for (unsigned i = 1; i < lines; i++) {
    x = 0;
    y += font::getHeight();
    char *line = SCREEN_BUFFER_LINE(i);
    char *previousLine = SCREEN_BUFFER_LINE(i - 1);
    int lineLength = strlen(line);
    unsigned previousLineLength = strlen(previousLine);
    for (unsigned i = lineLength; i < (unsigned)previousLineLength; i++) {
      display::removeCharacter(x, y - font::getHeight());
      x += font::getWidth();
    }
    strcpy(previousLine, line);
    x = 0;
    for (unsigned i = 0; i < (unsigned)lineLength; i++) {
      display::writeCharacter(x, y - font::getHeight(), previousLine[i]);
      x += font::getWidth();
    }
  }
  char *lastLine = SCREEN_BUFFER_LINE(lines - 1);
  int lastLineLength = strlen(lastLine);
  x = 0;
  y = font::getHeight() * (lines - 1);
  for (unsigned i = 0; i < (unsigned)lastLineLength; i++) {
    display::removeCharacter(x, y);
    x += font::getWidth();
  }
  lastLine[0] = 0;
}

void print(const char *str) {
  if (columns == 0) {
    columns = display::getWidth() / font::getWidth();
    lines = display::getHeight() / font::getHeight();
    screenBuffer = (char *)memory::kmalloc((columns + 1) * lines);
    for (unsigned i = 0; i < lines; i++) {
      SCREEN_BUFFER_CHARACTER(0, i) = 0;
    }
  }
  unsigned x = column * font::getWidth();
  unsigned y = line * font::getHeight();
  while (*str != 0) {
    if (column >= columns || *str == '\n') {
      column = x = 0;
      if (line < lines - 1) {
        line++;
        y += font::getHeight();
      } else {
        scrollDown();
        line = lines - 1;
        y = line * font::getHeight();
      }
      if (*str == '\n') {
        str++;
        continue;
      }
    }
    display::writeCharacter(x, y, *str);
    SCREEN_BUFFER_CHARACTER(column, line) = *str;
    SCREEN_BUFFER_CHARACTER(column + 1, line) = 0;
    str++;
    x += font::getWidth();
    column++;
  }
}
void print(unsigned long value, unsigned long base) {
  char buffer[sizeof(unsigned long) * 8 + 1];
  unsigned i = 0;
  do {
    buffer[i++] =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/"
            [value % base];
    value /= base;
  } while (value != 0);
  buffer[i] = 0;
  unsigned count = i;
  unsigned end = i - 1;
  for (i = 0; i < count / 2; i++) {
    char temp = buffer[i];
    buffer[i] = buffer[end - i];
    buffer[end - i] = temp;
  }
  print(buffer);
}
} // namespace kout
