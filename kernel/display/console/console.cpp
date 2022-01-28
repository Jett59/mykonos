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
#include <mykonos/kout.h>

#include <mykonos/display.h>
#include <mykonos/fontRenderer.h>

#include <mykonos/kmalloc.h>

#include <mykonos/string.h>

#include <mykonos/spinlock.h>

namespace kout {
static unsigned line, column;
static unsigned lines, columns;

static char *screenBuffer;
#define SCREEN_BUFFER_CHARACTER(COLUMN, LINE)                                  \
  (screenBuffer[(LINE) * (columns + 1) + (COLUMN)])
#define SCREEN_BUFFER_LINE(LINE) (&screenBuffer[(LINE) * (columns + 1)])

static unsigned displayWidth, displayHeight;
static unsigned fontWidth, fontHeight;

static lock::Spinlock consoleLock;

void scrollDown() {
  unsigned x = 0;
  unsigned y = 0;
  for (unsigned i = 1; i < lines; i++) {
    y += fontHeight;
    char *line = SCREEN_BUFFER_LINE(i);
    char *previousLine = SCREEN_BUFFER_LINE(i - 1);
    int lineLength = strlen(line);
    unsigned previousLineLength = strlen(previousLine);
    x = lineLength * fontWidth;
    for (unsigned i = lineLength; i < (unsigned)previousLineLength; i++) {
      display::removeCharacter(x, y - fontHeight);
      x += fontWidth;
    }
    strcpy(previousLine, line);
    x = 0;
    for (unsigned i = 0; i < (unsigned)lineLength; i++) {
      display::writeCharacter(x, y - fontHeight, previousLine[i]);
      x += fontWidth;
    }
  }
  char *lastLine = SCREEN_BUFFER_LINE(lines - 1);
  int lastLineLength = strlen(lastLine);
  x = 0;
  for (unsigned i = 0; i < (unsigned)lastLineLength; i++) {
    display::removeCharacter(x, y);
    x += fontWidth;
  }
  lastLine[0] = 0;
}

void print(const char *str, int len, bool skipLocking) {
  if (!skipLocking) {
    consoleLock.acquire();
  }
  if (columns == 0) {
    displayWidth = display::getWidth();
    displayHeight = display::getHeight();
    fontWidth = font::getWidth();
    fontHeight = font::getHeight();
    columns = displayWidth / fontWidth;
    lines = displayHeight / fontHeight;
    screenBuffer = (char *)memory::kmalloc((columns + 1) * lines);
    for (unsigned i = 0; i < lines; i++) {
      SCREEN_BUFFER_CHARACTER(0, i) = 0;
    }
  }
  unsigned x = column * fontWidth;
  unsigned y = line * fontHeight;
  for (int i = 0; i < len; i++) {
    if (column >= columns || *str == '\n') {
      column = x = 0;
      if (line < lines - 1) {
        line++;
        y += fontHeight;
      } else {
        scrollDown();
        line = lines - 1;
        y = line * fontHeight;
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
    x += fontWidth;
    column++;
  }
  if (!skipLocking) {
    consoleLock.release();
  }
}
void print(unsigned long value, unsigned long base, bool skipLocking) {
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
  print(buffer, strlen(buffer), skipLocking);
}

void acquireConsoleLock() { consoleLock.acquire(); }
void releaseConsoleLock() { consoleLock.release(); }
} // namespace kout
