/*
    Copyright (C) 2021  Jett Thompson

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
#ifndef _MYKONOS_CPU_H
#define _MYKONOS_CPU_H

#include <stdint.h>

namespace cpu {
// Most of these functions map to processor instructions. Check the official
// documentation for details of each one.
static inline uint64_t rdmsr(uint32_t address) {
  uint32_t msrLow, msrHigh;
  __asm__ volatile("rdmsr"
                   : "=a"(msrLow), "=d"(msrHigh)
                   : "c"(address)
                   : "memory");
  return msrLow | ((uint64_t)msrHigh << 32);
}
static inline void wrmsr(uint64_t value, uint32_t address) {
  __asm__ volatile("wrmsr"
                   :
                   : "a"((uint32_t)value), "d"((uint32_t)(value >> 32)),
                     "c"(address)
                   : "memory");
}

// Get the CPU number from the TSC auxiliary data
static inline unsigned getCpuNumber() {
  unsigned cpuNumber;
  __asm__ volatile("rdtscp" : "=c"(cpuNumber) : : "rdx", "rax", "memory");
  return cpuNumber;
}
// Used to initialize the TSC auxiliary data with the CPU number
static inline void setCpuNumber(unsigned cpuNumber) {
  wrmsr(cpuNumber, 0xc0000103);  // rdtscp processor id
}

// Read the rflags register using pushfq
static inline uint64_t getFlags() {
  uint64_t result;
  __asm__ volatile(
      "pushfq;"
      "popq %0"
      : "=g"(result)
      :
      : "memory");
  return result;
}
// Set rflags using popfq
static inline void setFlags(uint64_t flags) {
  __asm__ volatile(
      "pushq %0;"
      "popfq"
      :
      : "g"(flags)
      : "memory", "cc");
}
// Get the value of the interrupt flag
static inline bool localIrqState() {
  return (getFlags() & (1 << 9)) != 0;
}
// cli and sti instructions
static inline void enableLocalIrqs() {
  __asm__ volatile("sti" : : : "memory");
}
static inline void disableLocalIrqs() {
  __asm__ volatile("cli" : : : "memory");
}

static inline void mfence() {
  __asm__ volatile("mfence" : : : "memory");
}
// Pause instruction
static inline void relax() {
  __asm__ volatile("pause");
}

// Hault forever ignoring IRQs
[[noreturn]] static inline void hault() {
  disableLocalIrqs();
  while (true) {
    __asm__ volatile("hlt");
  }
}
// Hault forever with IRQs enabled
[[noreturn]] static inline void haultWithIrqs() {
  enableLocalIrqs();
  while (true) {
    __asm__("hlt");
  }
}
}  // namespace cpu

#endif