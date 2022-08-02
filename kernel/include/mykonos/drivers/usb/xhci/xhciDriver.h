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
#ifndef _MYKONOS_DRIVERS_USB_XHCI_XHCI_DRIVER_H
#define _MYKONOS_DRIVERS_USB_XHCI_XHCI_DRIVER_H

#include <mykonos/drivers/tree.h>
#include <mykonos/drivers/usb/xhci/xhciRegisters.h>

namespace drivers {
namespace xhci {
class XhciDriver : public DeviceTree {
 public:
  XhciDriver(XhciRegisterAccess registers)
      : DeviceTree(DeviceType::XHCI), registers(registers) {}

  void load();

 private:
  XhciRegisterAccess registers;
};
}  // namespace xhci
}  // namespace drivers

#endif