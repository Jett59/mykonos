#Copyright (C) 2021  Jett Thompson
#
    #This program is free software: you can redistribute it and/or modify
    #it under the terms of the GNU General Public License as published by
    #the Free Software Foundation, either version 3 of the License, or
    #(at your option) any later version.
#
    #This program is distributed in the hope that it will be useful,
    #but WITHOUT ANY WARRANTY; without even the implied warranty of
    #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    #GNU General Public License for more details.
#
    #You should have received a copy of the GNU General Public License
    #along with this program.  If not, see <https://www.gnu.org/licenses/>.

export ARCH?=x86_64

export CXX:=$(ARCH)-elf-g++
export AS:=$(ARCH)-elf-gcc
export LD:=$(ARCH)-elf-gcc

export CPPFLAGS:=
export CXXFLAGS:=-std=gnu++17 -nostdinc -nostdlib -O2
export ASFLAGS:=

export LDFLAGS:=-nostdlib -lgcc

all: check-dependencies kernel

.PHONY: check-dependencies
check-dependencies:
	@ARCH=$(ARCH) ./scripts/check-dependencies.sh

.PHONY: kernel
kernel: kernel/Makefile
	@"$(MAKE)" -s -C kernel