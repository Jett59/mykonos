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
#ifndef _MBI_H
#define _MBI_H

#include <stdint.h>

#include <acpi/rsdp.h>

namespace multiboot {
struct MbiTag {
  uint32_t type;
  uint32_t size;
};
#define MBI_TAG_MEMORY 6
#define MBI_TAG_FRAME_BUFFER 8
#define MBI_TAG_RSDP_OLD 14
#define MBI_TAG_RSDP_NEW 15

struct MemoryMapTag {
  uint32_t type;
  uint32_t size;
  uint32_t entrySize;
  uint32_t version;
  struct {
    uint64_t base;
    uint64_t length;
    uint32_t type;
    uint32_t reserved;
  } memory[0];
};
static_assert(
    sizeof(MemoryMapTag) == 16,
    "Invalid padding added to MemoryMapTag. Add __attribute__((packed))");

struct __attribute__((packed)) FrameBufferTag {
  uint32_t type;
  uint32_t size;
  uint64_t address;
  uint32_t pitch;
  uint32_t width;
  uint32_t height;
  uint8_t depth;
  // Remaining information omitted
};
static_assert(sizeof(FrameBufferTag) == 29,
              "Invalid padding in FrameBufferTag");

struct OldRsdpTag {
  uint32_t type;
  uint32_t size;
  acpi::RsdpV1 rsdp;
};
static_assert(sizeof(OldRsdpTag) == sizeof(acpi::RsdpV1) + 8,
              "Invalid padding added to OldRsdpTag");
struct NewRsdpTag {
  uint32_t type;
  uint32_t size;
  acpi::RsdpV2 rsdp;
};
static_assert(sizeof(NewRsdpTag) == sizeof(acpi::RsdpV2) + 8,
              "Invalid padding added to NewRsdpTag");

void parseMbi();
class Mbi {
private:
  uint32_t size;
  uint32_t reserved;
  MbiTag firstTag;
  friend void parseMbi();
};
} // namespace multiboot

#endif