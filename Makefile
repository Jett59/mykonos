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
export CXXFLAGS:=-std=gnu++17 -nostdlib -O2 -g -ffunction-sections -fdata-sections -Wall -Wextra
export ASFLAGS:=

export LDFLAGS:=-nostdlib -lgcc -Wl,--gc-sections

all: check-dependencies kernel

.PHONY: check-dependencies
check-dependencies:
	@ARCH=$(ARCH) ./scripts/check-dependencies.sh

.PHONY: kernel
kernel: kernel/Makefile
	@"$(MAKE)" -s -C kernel
	@mkdir -p build
	@cp kernel/arch/$(ARCH)/build/mykonos build/mykonos
	@objcopy --only-keep-debug build/mykonos build/mykonos.debug
	@strip --strip-all build/mykonos

EFI_FILE_NAME?=BOOTX64.EFI

build/boot.efi:
	@grub-mkimage -O $(ARCH)-efi -p /boot/grub -o $@ normal part_msdos fat iso9660 part_gpt all_video multiboot2

.PHONY: isoimage
isoimage: all build/boot.efi
	@mkdir -p build/isoroot
	@mkdir -p build/isoroot/boot/grub
	@cp build/mykonos build/isoroot/boot/mykonos
	@cp grub/example.cfg build/isoroot/boot/grub/grub.cfg
	@mkdir -p build/isoroot/EFI/BOOT
	@cp build/boot.efi build/isoroot/EFI/BOOT/$(EFI_FILE_NAME)
	@grub-mkrescue -d /usr/lib/grub/i386-pc -o build/mykonos.iso build/isoroot

.PHONY: clean
clean:
	@"$(MAKE)" -s -C kernel clean
	@rm -rf build
