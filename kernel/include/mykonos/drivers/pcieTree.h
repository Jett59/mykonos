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
#ifndef _MYKONOS_DRIVERS_PCIE_TREE_H
#define _MYKONOS_DRIVERS_PCIE_TREE_H

#include <mykonos/acpi/mcfg.h>
#include <mykonos/drivers/tree.h>
#include <stdint.h>

namespace drivers {
class PcieDeviceTree : public DeviceTree {
 public:
  PcieDeviceTree(acpi::McfgTableManager* mcfg) : DeviceTree(DeviceType::PCIE), mcfg(mcfg) {}

 protected:
  virtual void load();

 private:
  acpi::McfgTableManager* mcfg;
};
}  // namespace drivers

#endif