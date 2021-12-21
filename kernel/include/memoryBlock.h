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

   #ifndef _MEMORYBLOCK_H
   #define _MEMORYBLOCK

#include <stddef.h>
#include <stdint.h>

namespace memory {
    class Block {
        private:
      void* start;
      void* end;
      public:
      Block(void* start, void* end) : start(start), end(end) {}
      void* getStart() { return start; }
      void* getEnd() { return end; }
      size_t capacity() { return (size_t)((uint8_t*)end - (uint8_t*)start); }
      void reserve(size_t amount) {
          if (capacity() >= amount) {
            start = (void*)((uint8_t*)start + amount);
          }
      }
          void addToBeginning(size_t amount) {
            start = (void*)((uint8_t*)start - amount);
          }
          void addToEnd(size_t amount) {
            end = (void*)((uint8_t*)end + amount);
          }
    };
    }  // namespace memory

#endif