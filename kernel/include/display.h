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
#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

namespace display {
    struct Pixel {
      uint8_t r, g, b;
    };
    void writePixel(unsigned x, unsigned y, Pixel pixel);

    void writeString(unsigned x, unsigned y, const char* string);
    }  // namespace display

#endif