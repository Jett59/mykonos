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
#ifndef _MYKONOS_INITRAMFS_H
#define _MYKONOS_INITRAMFS_H

#include <mykonos/fsProvider.h>
#include <mykonos/string.h>
#include <mykonos/vector.h>
#include <stddef.h>

namespace initramfs {
struct Initramfs {
  void *pointer;
  size_t size;
};
extern Initramfs initramfs;

struct InitramfsEntry {
  String name;
  fs::FileType type;
  void *data;
  size_t size;
};

class InitramfsFsProvider : public fs::FsProvider {
public:
  InitramfsFsProvider();

  virtual size_t read(fs::FileNode &node, size_t offset, size_t length,
                      void *buffer);
  virtual void populateDirectory(fs::FileNode &directory);
  virtual void freeNode(fs::FileNode &directory);
  virtual void initRoot(fs::FileNode &root);

private:
  util::Vector<InitramfsEntry> entries;
};
} // namespace initramfs

#endif