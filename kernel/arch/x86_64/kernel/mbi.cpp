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
#include <mykonos/acpi/rsdp.h>
#include <mykonos/frameBuffer.h>
#include <mykonos/initramfs.h>
#include <mykonos/mbi.h>
#include <mykonos/pageConstants.h>
#include <mykonos/physicalMemory.h>
#include <mykonos/string.h>

extern "C" {
extern multiboot::Mbi* mbiPointer;

extern void* kernelPhysicalAddress[0];
extern void* kernelPhysicalEnd[0];
}

namespace initramfs {
Initramfs initramfs;
}

namespace memory {
BlockMap physicalMemory;
}

namespace acpi {
RsdpV2 rsdp;
}

namespace multiboot {
static void parseMbiTag(uint32_t type, MbiTag* tag);
void parseMbi() {
  MbiTag* tag = &mbiPointer->firstTag;
  while (tag->type != 0) {
    parseMbiTag(tag->type, tag);
    tag = (MbiTag*)((uint8_t*)tag + (tag->size + 7) / 8 * 8);
  }
  // Now that the module and memory map tags are parsed, we should reserve the
  // initramfs from the physical memory map.
  auto& initramfs = initramfs::initramfs;
  if (initramfs.size > 0) {
    memory::physicalMemory.reserve(
        {(void*)PAGE_ALIGN_DOWN((size_t)initramfs.pointer),
         (void*)PAGE_ALIGN_UP((size_t)initramfs.pointer + initramfs.size)});
  }
}
static void parseModuleTag(ModuleTag* tag);
static void parseMemoryMapTag(MemoryMapTag* memoryMap);
static void parseFrameBufferTag(FrameBufferTag* tag);
static void parseOldRsdpTag(OldRsdpTag* tag);
static void parseNewRsdpTag(NewRsdpTag* tag);
static void parseMbiTag(uint32_t type, MbiTag* tag) {
  switch (type) {
    case MBI_TAG_MODULE: {
      parseModuleTag((ModuleTag*)tag);
      break;
    }
    case MBI_TAG_MEMORY: {
      parseMemoryMapTag((MemoryMapTag*)tag);
      break;
    }
    case MBI_TAG_FRAME_BUFFER: {
      parseFrameBufferTag((FrameBufferTag*)tag);
      break;
    }
    case MBI_TAG_RSDP_OLD: {
      parseOldRsdpTag((OldRsdpTag*)tag);
      break;
    }
    case MBI_TAG_RSDP_NEW: {
      parseNewRsdpTag((NewRsdpTag*)tag);
      break;
    }
    default:
      break;
  }
}
static void parseModuleTag(ModuleTag* tag) {
  initramfs::initramfs = {(void*)(size_t)tag->moduleStart,
                          tag->moduleEnd - tag->moduleStart};
}
static void parseMemoryMapTag(MemoryMapTag* memoryMap) {
  uint32_t numEntries =
      (memoryMap->size - sizeof(MemoryMapTag)) / memoryMap->entrySize;
  for (uint32_t i = 0; i < numEntries; i++) {
    if (memoryMap->memory[i].type == 1) {
      void* entryBase = (void*)PAGE_ALIGN_UP(memoryMap->memory[i].base);
      size_t entrySize = PAGE_ALIGN_DOWN(memoryMap->memory[i].length);
      void* entryEnd = (void*)((uint8_t*)entryBase + entrySize);
      memory::physicalMemory.addBlock(memory::Block(entryBase, entryEnd));
    }
  }
  memory::physicalMemory.reserve(
      memory::Block((void*)PAGE_ALIGN_DOWN((size_t)kernelPhysicalAddress),
                    (void*)PAGE_ALIGN_UP((size_t)kernelPhysicalEnd)));
  memory::physicalMemory.reserve(memory::Block((void*)0x0, (void*)0x100000));
}
static void parseFrameBufferTag(FrameBufferTag* tag) {
  display::frameBuffer.pointer = (uint8_t*)tag->address;
  display::frameBuffer.width = tag->width;
  display::frameBuffer.height = tag->height;
  display::frameBuffer.depth = tag->depth;
  display::frameBuffer.pitch = tag->pitch;
}
static void parseOldRsdpTag(OldRsdpTag* tag) {
  memcpy(&acpi::rsdp, &tag->rsdp, sizeof(acpi::RsdpV1));
}
static void parseNewRsdpTag(NewRsdpTag* tag) {
  memcpy(&acpi::rsdp, &tag->rsdp, sizeof(acpi::RsdpV2));
}
}  // namespace multiboot
