/*
    Copyright (C) 2022 Jett Thompson

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
#include <mykonos/initramfs.h>
#include <stddef.h>

namespace initramfs {
static unsigned parseOctal(const char *str, size_t length) {
  unsigned result = 0;
  for (size_t i = 0; i < length; i++) {
    result *= 8;
    result += str[i] - '0';
  }
  return result;
}

#define SIGNATURE_OFFSET 257
#define SIGNATURE "USTAR"
#define SIGNATURE_LENGTH 5

#define TYPE_OFFSET 156
#define TYPE_FILE '0'
#define TYPE_DIRECTORY '5'

#define SIZE_OFFSET 124
#define SIZE_LENGTH 11

InitramfsFsProvider::InitramfsFsProvider() {
  char *initramfsPointer = (char *)initramfs.pointer;
  while (
      memeq(initramfsPointer + SIGNATURE_OFFSET, SIGNATURE, SIGNATURE_LENGTH)) {
    char type = initramfsPointer[TYPE_OFFSET];
    fs::FileType fileType = type == TYPE_FILE ? fs::FileType::FILE
                                              : type == TYPE_DIRECTORY
                                                    ? fs::FileType::DIRECTORY
                                                    : fs::FileType::NONE;
    size_t size = parseOctal(initramfsPointer + SIZE_OFFSET, SIZE_LENGTH);
    if (fileType != fs::FileType::NONE) {
      void *data = (void *)(initramfsPointer + 512);
      entries.push_back({initramfsPointer, fileType, data, size});
    }
    // Add the header (512 bytes) + the size of the file (aligned to 512 bytes)
    // to the initramfsPointer.
    initramfsPointer += (size + 511) / 512 * 512 + 512;
  }
}
} // namespace initramfs
