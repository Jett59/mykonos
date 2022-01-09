/*
    Copyright (C) 2022  Jett Thompson

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
#ifndef _ACPI_RSDT_H
#define _ACPI_RSDT_H

#include <acpi/tables.h>

#include <stddef.h>

namespace acpi {
class RsdtTableManager : public TableManager {
public:
  RsdtTableManager(TableHeader *header);
  virtual ~RsdtTableManager();

private:
  TableManager **children;
  size_t numChildren;

  TableManager *operator[](size_t i) {
    return i < numChildren ? children[i] : nullptr;
  }
  size_t childCount() { return numChildren; }

  TableManager *get(TableType type, int num = 0);
};
} // namespace acpi

#endif