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
#include <memoryBlock.h>

namespace memory {
void BlockMap::merge() {
  Block temp[numBlocks];
  for (unsigned i = 0; i < numBlocks; i++) {
    temp[i] = blocks[i];
  }
  unsigned originalNumBlocks = numBlocks;
  numBlocks = 0;
  for (unsigned i = 0; i < originalNumBlocks; i++) {
    if (temp[i].capacity() == 0) {
      continue; // Ignore empty block
    }
    [&] { // Use lambda for multi level continue
      for (unsigned j = 0; j < numBlocks; j++) {
        if (blocks[j].start == temp[i].end) {
          blocks[j].start = temp[i].start;
          return; // Multi level continue
        }
        if (blocks[j].end == temp[i].start) {
          blocks[j].end = temp[i].end;
          return; // Multi level continue
        }
      }
      // Could not combine, just append
      blocks[numBlocks++] = temp[i];
    }();
  }
}
void BlockMap::addBlock(Block block) {
  if (block.capacity() > 0) {
    if (numBlocks < BLOCKMAP_SIZE) {
      blocks[numBlocks++] = block;
    } else {
      merge();
      if (numBlocks < BLOCKMAP_SIZE) {
        blocks[numBlocks++] = block;
      }
    }
  }
}
void *BlockMap::allocate(size_t amount) {
  for (unsigned i = 0; i < numBlocks; i++) {
    if (blocks[i].capacity() >= amount) {
      return blocks[i].reserve(amount);
    }
  }
  return nullptr; // Not enough room in the block map
}
void BlockMap::reserve(Block blockToRemove) {
  size_t blockToRemoveStart = (size_t)blockToRemove.getStart();
  size_t blockToRemoveEnd = (size_t)blockToRemove.getEnd();
  merge();
  for (unsigned i = 0; i < numBlocks; i++) {
    Block &currentBlock = blocks[i];
    size_t currentBlockStart = (size_t)currentBlock.getStart();
    size_t currentBlockEnd = (size_t)currentBlock.getEnd();
    if (currentBlockStart >= blockToRemoveEnd) {
      continue;
    } else if (currentBlockEnd <= blockToRemoveStart) {
      continue;
    } else if (currentBlockStart >= blockToRemoveStart &&
               currentBlockEnd <=
                   blockToRemoveEnd) { // currentBlock is inside blockToRemove
      currentBlock = Block();
    } else { // There is some intersection between the blocks
      if (currentBlockStart >= blockToRemoveStart) {
        currentBlock.start = blockToRemove.end;
      } else if (currentBlockEnd <= blockToRemoveEnd) {
        currentBlock.end = blockToRemove.start;
      } else { // blockToRemove is entirely inside currentBlock
        currentBlock = Block();
        addBlock(Block((void *)currentBlockStart, (void *)blockToRemoveStart));
        addBlock(Block((void *)blockToRemoveEnd, (void *)currentBlockEnd));
      }
    }
  }
}
} // namespace memory
