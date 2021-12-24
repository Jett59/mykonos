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
#ifndef _FRAME_BUFFER_H
#define _FRAME_BUFFER_H

#include <stdint.h>

namespace display {
struct FrameBuffer {
  uint8_t *pointer;
  unsigned width;
  unsigned height;
  unsigned pitch;
  unsigned depth;
};
extern FrameBuffer frameBuffer;
void initFrameBuffer();
} // namespace display

#endif