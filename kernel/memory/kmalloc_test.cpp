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

#include <kmalloc.h>
#include <test.h>

namespace memory {
namespace test {
bool kmallocTest(::test::Logger logger) {
  logger("kmallocTest:\n");
  // Stress test: can it allocate forever?
  void *allocatedMemory[2048];
  bool passed = true;
  int numAllocated = 0;
  for (int i = 0; i < 2048; i++) {
    allocatedMemory[i] = kmalloc(2);
    numAllocated++;
    if (allocatedMemory[i] == nullptr) {
      passed = false;
      break;
    }
  }
  // Clean up
  for (int i = 0; i < numAllocated; i++) {
    kfree(allocatedMemory[i]);
  }
  if (!passed) {
    logger("kmallocTest: Failed stress test\n");
    return false;
  }
  logger("kmallocTest: Passed stress test\n");
  logger("kmallocTest: Succeeded\n");
  return true;
}
bool mapMemoryTest(::test::Logger logger) {
  logger("mapMemoryTest:\n");
  for (int i = 0; i < 4096; i++) {
    void *ptr = mapAddress(nullptr, 1048576);
    if (ptr == nullptr) {
      logger("mapMemoryTest: Stress test failed\n");
      return false;
    } else {
      unmapMemory(ptr, 1048576);
    }
  }
  logger("mapMemoryTest: Succeeded\n");
  return true;
}
ADD_TEST(kmallocTest);
ADD_TEST(mapMemoryTest);
} // namespace test
} // namespace memory

#endif