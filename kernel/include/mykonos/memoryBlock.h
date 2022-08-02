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
#ifndef _MYKONOS_MEMORY_BLOCK_H
#define _MYKONOS_MEMORY_BLOCK_H

#include <mykonos/spinlock.h>
#include <stddef.h>
#include <stdint.h>

namespace memory {
class BlockMap;
class Block {
 private:
  void* start;
  void* end;
  Block() : start(nullptr), end(nullptr) {}

 public:
  Block(void* start, void* end) : start(start), end(end) {}
  void* getStart() { return start; }
  void* getEnd() { return end; }
  size_t capacity() { return (size_t)((uint8_t*)end - (uint8_t*)start); }
  void* reserve(size_t amount) {
    if (capacity() >= amount) {
      void* reservedMemory = start;
      start = (void*)((uint8_t*)start + amount);
      return reservedMemory;
    }
    return nullptr;
  }
  void addToBeginning(size_t amount) { start = (void*)((uint8_t*)start - amount); }
  void addToEnd(size_t amount) { end = (void*)((uint8_t*)end + amount); }
  friend class BlockMap;
};
#define BLOCKMAP_SIZE 256
class BlockMap {
 private:
  Block blocks[BLOCKMAP_SIZE];
  unsigned numBlocks = 0;
  lock::Spinlock lock;
  void merge();
  void addBlock(Block block, bool acquireLock);

 public:
  void addBlock(Block block) { addBlock(block, true); }
  void* allocate(size_t amount);
  void returnMemory(void* ptr, size_t amount) { addBlock(Block(ptr, (void*)((uint8_t*)ptr + amount))); }
  void reserve(Block block);

  void clear() {
    lock.acquire();
    numBlocks = 0;
    lock.release();
  }
};
}  // namespace memory

#endif