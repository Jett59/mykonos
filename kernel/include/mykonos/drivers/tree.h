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
#ifndef _MYKONOS_DRIVERS_TREE_H
#define _MYKONOS_DRIVERS_TREE_H

namespace drivers {
enum class DeviceType { ACPI, PCIE };

void loadRootDevice();

class DeviceTree {
public:
  class Iterator {
  private:
    DeviceTree *ptr;

  public:
    Iterator(DeviceTree *tree) : ptr(tree) {}

    Iterator operator++(int) {
      Iterator result = *this;
      ptr = ptr->next;
      return result;
    }
    Iterator operator++() {
      ptr = ptr->next;
      return *this;
    }

    DeviceTree &operator*() { return *ptr; }

    bool operator==(Iterator &other) { return this->ptr == other.ptr; }
    bool operator!=(Iterator &other) { return !(*this == other); }

    operator bool() { return ptr != nullptr; }
  };

  DeviceTree(DeviceType type) : type(type) {}
  virtual ~DeviceTree() {
    for (auto &child : *this) {
      delete &child;
    }
  }

  DeviceType getType() { return type; }

  Iterator begin() { return Iterator(firstChild); }
  Iterator end() { return Iterator(nullptr); }

protected:
  void appendAndLoad(DeviceTree *child);

  virtual void load() = 0;

private:
  const DeviceType type;

  DeviceTree *firstChild = nullptr;
  DeviceTree *lastChild = nullptr;

  DeviceTree *next = nullptr;

  friend void loadRootDevice();
};

void setRootDevice(DeviceTree *device);
} // namespace drivers

#endif