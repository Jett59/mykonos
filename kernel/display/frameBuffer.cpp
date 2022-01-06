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
#include <display.h>
#include <frameBuffer.h>
#include <kmalloc.h>
#include <string.h>

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
unsigned getWidth() { return frameBuffer.width; }
unsigned getHeight() { return frameBuffer.height; }
} // namespace display
