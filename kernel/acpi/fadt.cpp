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
#include <mykonos/acpi/fadt.h>
#include <mykonos/kmalloc.h>
#include <mykonos/kout.h>

namespace acpi {
FadtTableManager::FadtTableManager(TableHeader* header) : TableManager(TableType::FADT) {
  Fadt* fadt = (Fadt*)header;
#ifdef __x86_64__
  // Find if there is a "8042" controller using the boot architecture flags.
  // Qemu appears to not set this flag dispite being version 3, so we exclude version 3 as well.
  if (header->revision > 3) {
    hasPs2Controller = (fadt->bootArchitectureFlags & (1 << 1)) != 0;
  } else {
    hasPs2Controller = true;
  }
  if (hasPs2Controller) {
    kout::print("Found PS/2 controller\n");
  }
#endif
  memory::unmapMemory((void*)fadt, header->length);
}
}  // namespace acpi
