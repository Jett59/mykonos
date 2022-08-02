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

static __attribute__((constructor)) void initializeRoot() {
  root.type = FileType::DIRECTORY;
}

FileHandle::FileHandle(String path, bool writable) : node(nullptr), writable(writable) {
  FileHandle current(&root, false);
  while (path.len() > 0) {
    while (*path == '/') {
      path = path.subString(1);
    }
    String currentComponent = path.findBefore('/');
    if (currentComponent.len() == 0) {
      break;
    }
    auto nextIndex = current.findChild(currentComponent);
    if (nextIndex == SIZE_MAX) {
      current.close();
      open = false;
      error = FileError::NOT_FOUND;
      return;
    }
    auto next = current.openChild(nextIndex, false);
    current.close();
    current = next;
    path = path.findNext('/');
  }
  node = current.node;
  current.close();
  node->lock.acquire();
  node->openCount++;
  node->lock.release();
  open = true;
  error = FileError::OKAY;
}
FileHandle::FileHandle(FileNode* node, bool writable) : node(node), writable(writable), open(true), error(FileError::OKAY) {
  node->lock.acquire();
  node->openCount++;
  node->lock.release();
}

FileType FileHandle::getType() {
  if (open) {
    node->lock.acquire();
    FileType result = node->type;
    node->lock.release();
    return result;
  } else {
    return FileType::NONE;
  }
}

void FileHandle::close() {
  if (open) {
    node->lock.acquire();
    node->openCount--;
    node->lock.release();
    node = nullptr;
    error = FileError::CLOSED;
  }
}

String FileHandle::childName(size_t index) {
  if (open) {
    node->lock.acquire();
    if (node->type == FileType::DIRECTORY) {
      if (!node->populated) {
        node->fsProvider->populateDirectory(*node);
      }
      if (index < node->children.getSize()) {
        String result = node->children[index]->name;
        node->lock.release();
        return result;
      }
    } else {
      error = FileError::NOT_DIRECTORY;
    }
    node->lock.release();
  }
  return {};
}
FileHandle FileHandle::openChild(size_t index, bool writable) {
  if (open) {
    node->lock.acquire();
    if (node->type == FileType::DIRECTORY) {
      if (!node->populated) {
        node->fsProvider->populateDirectory(*node);
      }
      if (index < node->children.getSize()) {
        auto result = FileHandle(node->children[index], writable);
        node->lock.release();
        return result;
      }
    } else {
      error = FileError::NOT_DIRECTORY;
    }
    node->lock.release();
  }
  return {};
}
size_t FileHandle::findChild(String name) {
  if (open) {
    node->lock.acquire();
    if (node->type == FileType::DIRECTORY) {
      if (!node->populated) {
        node->fsProvider->populateDirectory(*node);
      }
      for (size_t i = 0; i < node->children.getSize(); i++) {
        auto entry = node->children[i];
        if (entry->name == name) {
          node->lock.release();
          return i;
        }
      }
    } else {
      error = FileError::NOT_DIRECTORY;
    }
    node->lock.release();
  }
  return SIZE_MAX;
}

size_t FileHandle::read(size_t offset, size_t length, void* buffer) {
  if (open) {
    node->lock.acquire();
    if (node->type == FileType::FILE) {
      size_t result = node->fsProvider->read(*node, offset, length, buffer);
      node->lock.release();
      return result;
    } else {
      error = FileError::NOT_FILE;
    }
    node->lock.release();
  }
  return 0;
}
size_t FileHandle::write(size_t offset, size_t length, void* buffer) {
  if (open && writable) {
    node->lock.acquire();
    if (node->type == FileType::FILE) {
      size_t result = node->fsProvider->write(*node, offset, length, buffer);
      node->lock.release();
      return result;
    } else {
      error = FileError::NOT_FILE;
    }
    node->lock.release();
  } else if (!writable) {
    error = FileError::READ_ONLY;
  }
  return 0;
}

void FileHandle::mount(FsProvider* fsProvider) {
  if (open) {
    if (writable) {
      node->lock.acquire();
      if (node->type == FileType::DIRECTORY) {
        if (node->children.getSize() == 0 && node->openCount == 1) {
          // If this isn't the unmounted root directory,
          if (node->fsProvider != nullptr) {
            // Then we have to tell it what we are doing.
            node->fsProvider->freeNode(*node);
          }
          node->fsProvider = fsProvider;
          node->populated = false;
          fsProvider->initRoot(*node);
        } else {
          error = FileError::BUSY;
        }
      } else {
        error = FileError::NOT_DIRECTORY;
      }
      node->lock.release();
    } else {
      error = FileError::READ_ONLY;
    }
  }
}
}  // namespace fs
