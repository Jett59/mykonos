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
#ifndef _MYKONOS_DRIVERS_USB_XHCI_XHCI_REGISTERS_H
#define _MYKONOS_DRIVERS_USB_XHCI_XHCI_REGISTERS_H

#include <mykonos/mmio.h>

#include <stdint.h>

namespace drivers {
namespace xhci {
struct XhciRegisters {
  uint8_t capabilityLength;
  uint8_t reserved0;
  uint16_t version;
  // ...
};
class XhciRegisterAccess {
 public:
  XhciRegisterAccess(XhciRegisters* registers) : registers(registers) {}

  uint8_t getCapabilityLength() {
    return mmio::read(&registers->capabilityLength);
  }
  uint16_t getVersion() { return mmio::read(&registers->version); }

 private:
  XhciRegisters* registers;
};
}  // namespace xhci
}  // namespace drivers

#endif