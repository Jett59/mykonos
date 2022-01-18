/*
    Copyright (C) 2022  Jett Thompson

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
#include <mykonos/pic.h>

#include <mykonos/ioPorts.h>

#define PIC_REMAP_BASE 0xf8

namespace pic {
void disablePic() {
  // Reset the PICs and remap them
  io::writePort8(0x11, 0x20);
  io::writePort8(0x11, 0xa0);
  io::delay();
  // Remap the PICs
  io::writePort8(PIC_REMAP_BASE, 0x21);
  io::writePort8(PIC_REMAP_BASE, 0xa1);
  io::delay();
  // Enable them
  io::writePort8(4, 0x21);
  io::writePort8(2, 0xa1);
  io::delay();
  io::writePort8(1, 0x21);
  io::writePort8(1, 0xa1);
  io::delay();

  // Mask all IRQs
  io::writePort8(0xff, 0x21);
  io::writePort8(0xff, 0xa1);
  io::delay();
}
} // namespace pic
