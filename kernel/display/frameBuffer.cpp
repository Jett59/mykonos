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
#include <mykonos/frameBuffer.h>
#include <mykonos/kmalloc.h>
#include <mykonos/string.h>

namespace display {
FrameBuffer frameBuffer;
void initFrameBuffer() {
  frameBuffer.pointer = (uint8_t *)memory::mapAddress(
      frameBuffer.pointer, frameBuffer.pitch * frameBuffer.height, false);
  memset(frameBuffer.pointer, 0, frameBuffer.pitch * frameBuffer.height);
}
void writePixel(unsigned x, unsigned y, Pixel pixel) {
  if (x < getWidth() && y < getHeight()) {
    uint8_t *pixelPointer = frameBuffer.pointer + (y * frameBuffer.pitch +
                                                   x * (frameBuffer.depth / 8));
    pixelPointer[2] = pixel.r;
    pixelPointer[1] = pixel.g;
    pixelPointer[0] = pixel.b;
  }
}
void writeBitmap(unsigned initialX, unsigned initialY, unsigned width,
                 unsigned height, void *bitmap, Pixel foreground,
                 Pixel background) {
  unsigned bytesPerLine = (width + 7) / 8;
  if (initialX + width > getWidth()) {
    width = getWidth() - initialX;
  }
  if (initialY + height > getHeight()) {
    height = getHeight() - initialY;
  }
  unsigned bytesPerPixel = frameBuffer.depth / 8;
  uint8_t pixelBuffer[bytesPerPixel * width];
  memset((void *)pixelBuffer, 0, sizeof(pixelBuffer));
  for (unsigned y = 0; y < height; y++) {
    uint8_t *bitmapLine = (uint8_t *)bitmap + y * bytesPerLine;
    for (unsigned x = 0; x < width; x++) {
      uint8_t *pixelPointer = pixelBuffer + x * bytesPerPixel;
      uint8_t bitmapByte = bitmapLine[x / 8];
      if ((bitmapByte & (1 << (7 - x % 8))) == 0) {
        pixelPointer[2] = background.r;
        pixelPointer[1] = background.g;
        pixelPointer[0] = background.b;
      } else {
        pixelPointer[2] = foreground.r;
        pixelPointer[1] = foreground.g;
        pixelPointer[0] = foreground.b;
      }
    }
    uint8_t *displayLine =
        frameBuffer.pointer +
        ((y + initialY) * frameBuffer.pitch + initialX * bytesPerPixel);
    memcpy((void *)displayLine, (void *)pixelBuffer, sizeof(pixelBuffer));
  }
}
unsigned getWidth() { return frameBuffer.width; }
unsigned getHeight() { return frameBuffer.height; }
} // namespace display
