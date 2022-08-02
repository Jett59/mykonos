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
#include <mykonos/cpuidCheck.h>

#include <cpuid.h>
#include <mykonos/kout.h>

#define CPUID_8000_0001_EDX_RDTSCP (1 << 27)

namespace cpuid {
bool checkCpuidFlags() {
  unsigned eax = 0, ebx = 0, ecx = 0, edx = 0;
  // Standard features (fn0x0000_0001)
  __get_cpuid(0x00000001, &eax, &ebx, &ecx, &edx);
  // Extended features (fn0x8000_0001)
  __get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx);
  if ((edx & CPUID_8000_0001_EDX_RDTSCP) == 0) {
    kout::print("Your CPU does not support rdtscp\n");
    return false;
  }
  return true;
}
}  // namespace cpuid
