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

enum class FileError {
  OKAY,
  CLOSED,
  NOT_FOUND,
  READ_ONLY,
  NOT_FILE,
  NOT_DIRECTORY,
  BUSY
};

struct FileNode;
class FsProvider;

class FileHandle {
public:
  /**
   * @brief open a new FileHandle from a path
   *
   * If the file does not exist, error will be set to FileError::NOT_FOUND.
   * Otherwise, error will be set to FileError::OKAY.
   *
   * @param path the path to the file (unix-style, with '/' to separate the
   * components of the path)
   * @param writable whether it is possible to write to the file
   */
  FileHandle(String path, bool writable);
  /**
   * @brief Construct a new (empty) File Handle object
   *
   * All operations will be ignored as if close() had just been called.
   */
  FileHandle()
      : node(nullptr), writable(false), open(false), error(FileError::CLOSED) {}

  /**
   * @brief Get the file's type
   *
   * @return the type of the file
   */
  FileType getType();
  /**
   * @brief close the file
   *
   * All operations after this point will do nothing and return defaults.
   * error will be set to FileError::CLOSED.
   */
  void close();

  /**
   * @brief read from a file
   *
   * If getType() != FileType::FILE, error will be set to
   * FileError::NOT_FILE.
   *
   * @param offset the offset into the file
   * @param length the length of the buffer
   * @param buffer the buffer to read data into
   * @return the number of bytes read
   */
  size_t read(size_t offset, size_t length, void *buffer);
  /**
   * @brief write to a file
   *
   * If getType() != FileType::FILE, error will be set to
   * FileError::NOT_FILE.
   * If !writable, error will be set to FileError::READ_ONLY.
   *
   * @param offset the offset into the file, where SIZE_MAX means to append
   * @param length the length of the buffer
   * @param buffer the buffer to write to the file
   * @return the number of bytes written
   */
  size_t write(size_t offset, size_t length, void *buffer);

  /**
   * @brief get the name of a child file
   *
   *If getType() != FileType::DIRECTORY, error will be set to
   * FileError::NOT_DIRECTORY.
   *
   * @param index the index of the child
   * @return the name of the child, or empty string if the index is out of
   * range
   */
  String childName(size_t index);
  /**
   * @brief open a child file
   *
   * If getType() != FileType::DIRECTORY, error will be set to
   * FileError::NOT_DIRECTORY.
   *
   * @param index the index of the child file
   * @param writable whether the file will be openned with write permitions
   * @return the freshly openned FileHandle, or an empty FileHandle if the index
   * is out of range
   */
  FileHandle openChild(size_t index, bool writable);
  /**
   * @brief Find the index of the named child
   *
   * If getType() != FileType::DIRECTORY, error will be set to
   * FileError::NOT_DIRECTORY.
   *
   * @param name the name of the child
   * @return the index of the child, or SIZE_MAX if it does not exist or
   * getType() != FileType::DIRECTORY
   */
  size_t findChild(String name);

  /**
   * @brief mount a filesystem on this directory.
   *
   * If getType() != FileType::DIRECTORY, error will be set to
   * FileError::NOT_DIRECTORY.
   * If the number of loaded children is not 0, or the number of open
   * FileHandles on this file is not one, error will be set to FileError::BUSY.
   * If !writable, error will be set to FileError::READ_ONLY.
   *
   * @param fsProvider the file system to mount
   */
  void mount(FsProvider *fsProvider);

  bool isOpen() { return open; }

  FileError getError() { return error; }

  bool okay() { return getError() == FileError::OKAY; }

private:
  FileNode *node;
  bool writable;
  bool open;
  FileError error;

  FileHandle(FileNode *node, bool writable);
};
} // namespace fs

#endif