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
#include <frameBuffer.h>
#include <mbi.h>
#include <pageConstants.h>
#include <physicalMemory.h>

extern "C" {
extern multiboot::Mbi *mbiPointer;
}

namespace memory {
BlockMap physicalMemory;
}

namespace multiboot {
static void parseMbiTag(uint32_t type, MbiTag *tag);
void parseMbi() {
  MbiTag *tag = &mbiPointer->firstTag;
  while (tag->type != 0) {
    parseMbiTag(tag->type, tag);
    tag = (MbiTag *)((uint8_t *)tag + (tag->size + 7) / 8 * 8);
  }
}
static void parseMemoryMapTag(MemoryMapTag *memoryMap);
static void parseFrameBufferTag(FrameBufferTag *tag);
static void parseMbiTag(uint32_t type, MbiTag *tag) {
  switch (type) {
  case MBI_TAG_MEMORY: {
    parseMemoryMapTag((MemoryMapTag *)tag);
    break;
  }
  case MBI_TAG_FRAME_BUFFER: {
    parseFrameBufferTag((FrameBufferTag *)tag);
    break;
  }
  default:
    break;
  }
}
static void parseMemoryMapTag(MemoryMapTag *memoryMap) {
  uint32_t numEntries =
      (memoryMap->size - sizeof(MemoryMapTag)) / memoryMap->entrySize;
  for (uint32_t i = 0; i < numEntries; i++) {
    if (memoryMap->memory[i].type == 1) {
      void *entryBase = (void *)PAGE_ALIGN_UP(memoryMap->memory[i].base);
      size_t entrySize = PAGE_ALIGN_DOWN(memoryMap->memory[i].length);
      void *entryEnd = (void *)((uint8_t *)entryBase + entrySize);
      memory::physicalMemory.addBlock(memory::Block(entryBase, entryEnd));
    }
  }
}
static void parseFrameBufferTag(FrameBufferTag *tag) {
  display::frameBuffer.pointer = (uint8_t *)tag->address;
  display::frameBuffer.width = tag->width;
  display::frameBuffer.height = tag->height;
  display::frameBuffer.depth = tag->depth;
  display::frameBuffer.pitch = tag->pitch;
}
} // namespace multiboot
