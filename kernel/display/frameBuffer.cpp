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

static void* backBuffer;

void initFrameBuffer() {
  frameBuffer.pointer = (uint8_t*)memory::mapAddress(
      frameBuffer.pointer, frameBuffer.pitch * frameBuffer.height, false);
  backBuffer = memory::allocateMemory(frameBuffer.pitch * frameBuffer.height);
  memset(frameBuffer.pointer, 0, frameBuffer.pitch * frameBuffer.height);
  memset(backBuffer, 0, frameBuffer.pitch * frameBuffer.height);
}
void writePixel(unsigned x, unsigned y, Pixel pixel) {
  if (x < getWidth() && y < getHeight()) {
    unsigned pixelOffset =
        (y * frameBuffer.pitch + x * (frameBuffer.depth / 8));
    uint8_t* pixelPointer = (uint8_t*)backBuffer + pixelOffset;
    uint8_t* displayPixelPointer = frameBuffer.pointer + pixelOffset;
    pixelPointer[2] = pixel.r;
    pixelPointer[1] = pixel.g;
    pixelPointer[0] = pixel.b;
    memcpy((void*)displayPixelPointer, (void*)pixelPointer,
           frameBuffer.depth / 8);
  }
}

void clipRectangle(unsigned x, unsigned y, unsigned& width, unsigned& height) {
  if (x >= getWidth()) {
    width = 0;
    return;
  }
  if (y >= getHeight()) {
    height = 0;
    return;
  }
  if (x + width > getWidth()) {
    width = getWidth() - x;
  }
  if (y + height > getHeight()) {
    height = getHeight() - y;
  }
}

void writeBitmap(unsigned initialX,
                 unsigned initialY,
                 unsigned width,
                 unsigned height,
                 void* bitmap,
                 Pixel foreground,
                 Pixel background) {
  clipRectangle(initialX, initialY, width, height);
  if (width == 0 || height == 0) {
    return;
  }
  unsigned bytesPerBitmapLine = (width + 7) / 8;
  unsigned bytesPerPixel = frameBuffer.depth / 8;
  for (unsigned y = 0; y < height; y++) {
    uint8_t* bitmapLine = (uint8_t*)bitmap + y * bytesPerBitmapLine;
    unsigned pixelOffset =
        ((y + initialY) * frameBuffer.pitch + initialX * bytesPerPixel);
    uint8_t* backBufferLine = (uint8_t*)backBuffer + pixelOffset;
    for (unsigned x = 0; x < width; x++) {
      uint8_t* pixelPointer = backBufferLine + x * bytesPerPixel;
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
    uint8_t* displayLine = frameBuffer.pointer + pixelOffset;
    memcpy((void*)displayLine, (void*)backBufferLine, bytesPerPixel * width);
  }
}
unsigned getWidth() {
  return frameBuffer.width;
}
unsigned getHeight() {
  return frameBuffer.height;
}

void copyRegion(unsigned x,
                unsigned y,
                unsigned width,
                unsigned height,
                unsigned newX,
                unsigned newY) {
  clipRectangle(x, y, width, height);
  clipRectangle(newX, newY, width, height);
  if (width == 0 || height == 0) {
    return;
  }
  unsigned bytesPerPixel = frameBuffer.depth / 8;
  for (unsigned yOffset = 0; yOffset < height; yOffset++) {
    unsigned sourceOffset =
        ((y + yOffset) * frameBuffer.pitch + x * bytesPerPixel);
    void* sourcePointer = (void*)((uint8_t*)backBuffer + sourceOffset);
    unsigned destOffset =
        ((newY + yOffset) * frameBuffer.pitch + newX * bytesPerPixel);
    void* destBackBufferPointer = (void*)((uint8_t*)backBuffer + destOffset);
    void* destScreenPointer = (void*)(frameBuffer.pointer + destOffset);
    memcpy(destBackBufferPointer, sourcePointer, width * bytesPerPixel);
    memcpy(destScreenPointer, sourcePointer, width * bytesPerPixel);
  }
}
}  // namespace display
