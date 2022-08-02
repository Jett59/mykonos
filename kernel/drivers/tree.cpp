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
#include <mykonos/drivers/tree.h>
#include <mykonos/thread.h>

namespace drivers {
void DeviceTree::appendAndLoad(DeviceTree* child) {
  if (firstChild == nullptr) {
    firstChild = lastChild = child;
  } else {
    lastChild->next = child;
    lastChild = child;
  }
  thread::create(
      [](void* childPointer) {
        auto child = (DeviceTree*)childPointer;
        child->loadAndWait();
        thread::destroy();
      },
      (void*)child);
}

void DeviceTree::loadAndWait() {
  load();
  for (auto& child : *this) {
    child.initializationCompletion.await();
  }
  initializationCompletion.signalComplete();
}

static DeviceTree* rootDevice;

void setRootDevice(DeviceTree* device) {
  rootDevice = device;
}
void loadRootDevice() {
  rootDevice->loadAndWait();
}
}  // namespace drivers
