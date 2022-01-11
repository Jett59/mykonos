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
#ifndef _APIC_H
#define _APIC_H

#define MAX_LOCAL_APICS 64
#define MAX_IO_APICS 24

#include <stdint.h>

#include <kmalloc.h>

namespace apic {
struct LocalApicDescriptor {
  uint8_t apicId;
};
struct IoApicDescriptor {
  void *physicalAddress;
  uint32_t gsiBase;
};

extern uint32_t *localApicRegisters;

static inline void initLocalApic(void *physicalAddress) {
  localApicRegisters =
      (uint32_t *)memory::mapAddress(physicalAddress, 4096, false);
}
} // namespace apic

#endif