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
#ifndef _FONT_RENDERER_H
#define _FONT_RENDERER_H

#include <display.h>

namespace font {
void render(char c, unsigned x, unsigned y, display::Pixel foreground,
            display::Pixel background);
unsigned getWidth();
unsigned getHeight();
} // namespace font

#endif