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
#include <mykonos/fs.h>
#include <mykonos/fsProvider.h>

namespace fs {
static FileNode root;

FileHandle::FileHandle(String path, bool writable)
    : node(nullptr), writable(writable) {
  FileHandle current(&root, false);
  while (path.len() > 0) {
    while (*path == '/') {
      path = path.subString(1);
    }
    String currentComponent = path.findBefore('/');
    if (currentComponent == "") {
      break;
    }
    auto nextIndex = current.findChild(currentComponent);
    if (nextIndex == SIZE_MAX) {
      current.close();
      open = false;
      return;
    }
    auto next = current.openChild(nextIndex);
    current.close();
    current = next;
    path = path.findNext('/');
  }
  node = current.node;
  current.close();
  node->openCount++;
  open = true;
}
} // namespace fs
