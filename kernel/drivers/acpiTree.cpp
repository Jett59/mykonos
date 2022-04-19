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
#include <mykonos/drivers/acpiTree.h>

namespace drivers {
struct TableDriver {
  acpi::TableType type;
  DeviceTree *(*get)(acpi::TableManager *);
};
static TableDriver tableDrivers[] = {};

void AcpiDeviceTree::load() {
  for (size_t i = 0; i < tables->childCount(); i++) {
    acpi::TableManager *table = (*tables)[i];
    for (auto &tableDriver : tableDrivers) {
        if (tableDriver.type == table->type) {
          appendAndLoad(tableDriver.get(table));
        }
    }
  }
}
} // namespace drivers
