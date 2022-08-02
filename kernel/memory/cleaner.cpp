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
#include <mykonos/cleaner.h>
#include <mykonos/queue.h>
#include <mykonos/thread.h>

namespace cleaner {
struct CleanerObject {
  void* object;
  void (*handler)(void*);
};
static util::Queue<CleanerObject> queue;

void addObject(void* object, void (*handler)(void*)) {
  queue.push({object, handler});
}

static void cleanerThread(void*) {
  while (true) {
    auto object = queue.pop();
    object.handler(object.object);
  }
}

void init() {
  thread::create(cleanerThread, nullptr, PRIORITY_LOW);
}
}  // namespace cleaner
