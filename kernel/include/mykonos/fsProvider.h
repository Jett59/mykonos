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
  /**
   * @brief read from a file
   *
   * @param node the file to read from
   * @param offset the offset into the file
   * @param length the length of the buffer
   * @param buffer the buffer to read into
   * @return the number of bytes read
   */
  virtual size_t read(FileNode &node, size_t offset, size_t length,
                      void *buffer) {
    (void)node;
    (void)offset;
    (void)length;
    (void)buffer;
    return 0;
  }
  /**
   * @brief write to a file
   *
   * @param node the file to write to
   * @param offset the offset into the file, where SIZE_MAX means to append
   * @param length the length of the buffer
   * @param buffer the buffer to write to the file
   * @return the number of bytes written
   */
  virtual size_t write(FileNode &node, size_t offset, size_t length,
                       void *buffer) {
    (void)node;
    (void)offset;
    (void)length;
    (void)buffer;
    return 0;
  }
  /**
   * @brief populate a directory's children
   *
   * Each fchild must have:
   * 1) node.fsProvider = this
   * 2) type = the file's type
   * 3) name = the name of the file
   *
   * The child directories need not be populated.
   *
   * @param directory the directory to populate
   */
  virtual void populateDirectory(FileNode &directory) { (void)directory; }
};
} // namespace fs

#endif