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
#ifndef _MYKONOS_FS_H
#define _MYKONOS_FS_H

#include <mykonos/string.h>
#include <stddef.h>

namespace fs {
enum class FileType { NONE, FILE, DIRECTORY };

struct FileNode;

class FileHandle {
public:
  FileHandle(String path, bool writable);
  FileHandle();

  FileType getType();
  void close();
  void remove();

  size_t read(size_t offset, size_t length, void *buffer);
  size_t write(size_t offset, size_t length, void *buffer);

  String childName(size_t index);
  FileHandle openChild(size_t index, bool writable);
  // SIZE_MAX if child does not exist
  size_t findChild(String name);

  // handle == nullptr if there was an error while openning
  bool operator==(nullptr_t) { return !open; }
  bool operator!=(nullptr_t) { return open; }

private:
  FileNode *node;
  bool writable;
  bool open;

  FileHandle(FileNode *node, bool writable);
};
} // namespace fs

#endif