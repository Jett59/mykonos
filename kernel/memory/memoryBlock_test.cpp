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

namespace memory {
    namespace test {
    bool blockMapTest() {
        BlockMap map;
        map.addBlock(Block(nullptr, (void*)0x2000));
        // Simple allocation test
        if (map.allocate(0x2000) == nullptr) {
            return false;
        }
        // Free and allocate again
        map.returnMemory((void*)nullptr, 0x2000);
        if (map.allocate(0x2000) == nullptr) {
            return false;
        }
        // Stress test (test the merging function)
        for (int i = 0; i < 1000; i ++) {
          void* allocatedMemory = map.allocate(0x2000 / (i + 1));
          if (allocatedMemory == nullptr) {
            return false;
          }else {
            map.returnMemory(allocatedMemory, 0x2000 / (i + 1));
          }
        }
        return true;
    }
    } // namespace test
} // namespace memory
#endif