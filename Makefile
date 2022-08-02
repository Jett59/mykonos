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

export CXX:=$(ARCH)-mykonos-g++
export AS:=$(ARCH)-mykonos-gcc
export LD:=$(ARCH)-mykonos-gcc

export CPPFLAGS:=
export CXXFLAGS:=-std=gnu++17 -nostdlib -O2 -g -ffunction-sections -fdata-sections -Wall -Wextra
export ASFLAGS:=

export LDFLAGS:=-nostdlib -Wl,--gc-sections

# Import options from the .config file
-include .config

all: check-dependencies isoimage

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

.PHONY: initramfs
initramfs:
	@mkdir -p build/initramfs
	@cp -RT config/ build/initramfs/
	@echo build build/initramfs.img
	@tar -C build/initramfs -cf build/initramfs.img .

EFI_FILE_NAME?=BOOTX64.EFI

build/boot.efi:
	@grub-mkimage -O $(ARCH)-efi -p /boot/grub -o $@ normal part_msdos fat iso9660 part_gpt all_video multiboot2

.PHONY: isoimage
isoimage: kernel initramfs build/boot.efi
	@mkdir -p build/isoroot
	@mkdir -p build/isoroot/boot/grub
	@cp build/mykonos build/isoroot/boot/mykonos
	@cp build/initramfs.img build/isoroot/boot/initramfs.img
	@cp grub/example.cfg build/isoroot/boot/grub/grub.cfg
	@mkdir -p build/isoroot/EFI/BOOT
	@cp build/boot.efi build/isoroot/EFI/BOOT/$(EFI_FILE_NAME)
	@grub-mkrescue -d /usr/lib/grub/i386-pc -o build/mykonos.iso build/isoroot

.PHONY: clean
clean:
	@"$(MAKE)" -s -C kernel clean
	@rm -rf build
