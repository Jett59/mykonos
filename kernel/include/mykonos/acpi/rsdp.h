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
#ifndef _MYKONOS_ACPI_RSDP_H
#define _MYKONOS_ACPI_RSDP_H

#include <stdint.h>

namespace acpi {
struct RsdpV1 {
  char signature[8];  // "RSD PTR "
  uint8_t checksum;
  char oemId[6];
  uint8_t revision;
  uint32_t rsdtAddress;
};
static_assert(sizeof(RsdpV1) == 20, "RsdpV1 must be 20 bytes");
struct __attribute__((packed)) RsdpV2 {
  char signature[8];  // "RSD PTR "
  uint8_t checksum;
  char oemId[6];
  uint8_t revision;
  uint32_t rsdtAddress;

  uint32_t length;
  uint64_t xsdtAddress;
  uint8_t extendedChecksum;
  uint8_t reserved[3];

  bool doChecksum() {
    unsigned char* rsdpBytes = (unsigned char*)this;
    unsigned char sum = 0;
    for (unsigned i = 0; i < (revision >= 2 ? sizeof(RsdpV2) : sizeof(RsdpV1)); i++) {
      sum += rsdpBytes[i];
    }
    return sum == 0;
  }
};
static_assert(sizeof(RsdpV2) == 36, "RsdpV2 must be 36 bytes");

// V1 structure can fit in v2, so just use v2 structure
extern RsdpV2 rsdp;
}  // namespace acpi

#endif