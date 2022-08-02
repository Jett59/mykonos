/*
    Copyright (C) 2022  Jett Thompson

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

#include <mykonos/task/taskQueue.h>

#include <mykonos/test.h>

namespace task {
namespace test {
bool taskQueueTest(::test::Logger logger) {
  logger("taskQueueTest:\n");
  ControlBlock a, b, c;
  ControlBlock* aPointer = &a;
  ControlBlock* bPointer = &b;
  ControlBlock* cPointer = &c;
  Queue queue;
  queue.push(aPointer);
  queue.push(bPointer);
  queue.push(cPointer);
  if (queue.pop() != aPointer) {
    logger("taskQueueTest failed: Popping failed on first element\n");
    return false;
  }
  if (queue.pop() != bPointer) {
    logger("taskQueueTest failed: Popping failed on second element\n");
    return false;
  }
  if (queue.pop() != cPointer) {
    logger("taskQueueTest failed: Popping failed on third element\n");
    return false;
  }
  if (queue.pop() != nullptr) {
    logger("taskQueueTest failed: Popping non-existant element not null\n");
    return false;
  }
  logger("taskQueueTest: Succeeded\n");
  return true;
}
ADD_TEST(taskQueueTest);
}  // namespace test
}  // namespace task

#endif