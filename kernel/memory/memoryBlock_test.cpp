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
#ifdef RUNTIME_TESTS
// Test for memory::BlockMap

#include <memoryBlock.h>

#include <test.h>

namespace memory {
namespace test {
bool blockMapTest(::test::Logger logger) {
  logger("blockMapTest:\n");
  BlockMap map;
  map.addBlock(Block((void *)0x1000, (void *)0x3000));
  // Simple allocation test
  void *ptr = map.allocate(0x2000);
  if (ptr == nullptr) {
    logger("blockMapTest failed: Allocator not working properly\n");
    return false;
  }
  logger("blockMapTest: simple allocation test passed\n");
  // Free and allocate again
  map.returnMemory(ptr, 0x2000);
  ptr = map.allocate(0x2000);
  if (ptr == nullptr) {
    logger("blockMapTest failed: Allocate/free does not work properly\n");
    return false;
  }
  map.returnMemory(ptr, 0x2000);
  logger("blockMapTest: Allocate/free test passed\n");
  // Stress test (test the merging function)
  for (int i = 0; i < 1000; i++) {
    void *allocatedMemory = map.allocate(0x2000 / (i + 1));
    if (allocatedMemory == nullptr) {
      logger("blockMapTest failed: merge function does not work properly\n");
      return false;
    } else {
      map.returnMemory(allocatedMemory, 0x2000 / (i + 1));
    }
  }
  logger("blockMapTest: Passed stress tester\n");
  // Reserve test
  map.reserve(Block((void *)0x1000, (void *)0x3000)); // Reserve the whole map
  if (map.allocate(1) != nullptr) {
    logger("blockMapTest failed: reserving memory failed");
    return false;
  }
  logger("blockMapTest: Passed reserve test");
  logger("blockMapTest: Succeeded\n");
  return true;
}
ADD_TEST(blockMapTest);
} // namespace test
} // namespace memory

#endif