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
#include <mykonos/async/completion.h>
#include <mykonos/scheduler.h>

namespace async {
void Completion::await() {
  lock.acquire();
  if (completed) {
    lock.release();
  } else {
    waitingTasks.push(scheduler::block());
    lock.release();
    scheduler::yield();
  }
}
void Completion::signalComplete() {
  lock.acquire();
  completed = true;
  auto nextTask = waitingTasks.pop();
  scheduler::lock();
  while (nextTask != nullptr) {
    scheduler::addTask(nextTask);
    nextTask = waitingTasks.pop();
  }
  scheduler::unlock();
  lock.release();
}
}  // namespace async
