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
#include <mykonos/fontRenderer.h>

#include <stdint.h>

#include <mykonos/display.h>

struct PsfFile {
  uint32_t magic;
  uint32_t version;
  uint32_t headerSize;
  uint32_t flags;
  uint32_t numGlyphs;
  uint32_t glyphSize;
  uint32_t height;
  uint32_t width;
  uint8_t data[];
};

extern "C" {
extern PsfFile _binary_display_font_font_psf_start;
}
#define fontFile _binary_display_font_font_psf_start

namespace font {
void render(char c, unsigned x, unsigned y, display::Pixel foreground,
            display::Pixel background) {
  uint8_t *glyph = fontFile.data + (c * fontFile.glyphSize);
  for (unsigned glyphY = 0; glyphY < fontFile.height; glyphY++) {
    for (unsigned glyphX = 0; glyphX < fontFile.width; glyphX++) {
      uint8_t currentByte = glyph[glyphX / 8];
      uint8_t currentBit = glyphX % 8;
      unsigned displayX =
          fontFile.width - glyphX + x; // We are currently going backwards
      unsigned displayY = y + glyphY;
      if ((currentByte & (1 << currentBit)) != 0) {
        display::writePixel(displayX, displayY, foreground);
      } else {
        display::writePixel(displayX, displayY, background);
      }
    }
    glyph += fontFile.glyphSize / fontFile.height;
  }
}
unsigned getWidth() { return fontFile.width; }
unsigned getHeight() { return fontFile.height; }
} // namespace font
