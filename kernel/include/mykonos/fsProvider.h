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
#ifndef _MYKONOS_FS_PROVIDER_H
#define _MYKONOS_FS_PROVIDER_H

#include <mykonos/fs.h>
#include <mykonos/mutex.h>
#include <mykonos/string.h>
#include <mykonos/vector.h>
#include <stddef.h>

namespace fs {
class FsProvider;

struct FileNode {
  String name;
  FileType type;
  size_t openCount = 0;
  util::Vector<FileNode> children;
  void *node; // Identifies the file (fs-specific)
  lock::Mutex lock;
  FsProvider *fsProvider;
  bool populated = false;
};
class FsProvider {
public:
  virtual size_t read(FileNode &node, size_t offset, size_t length,
                      void *buffer) {
    (void)node;
    (void)offset;
    (void)length;
    (void)buffer;
    return 0;
  }
  virtual size_t write(FileNode &node, size_t offset, size_t length,
                       void *buffer) {
    (void)node;
    (void)offset;
    (void)length;
    (void)buffer;
    return 0;
  }
  virtual void populateDirectory(FileNode &directory) { (void)directory; }
};
} // namespace fs

#endif