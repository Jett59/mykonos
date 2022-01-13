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

ENTRY(_start);

kernelVirtualOffset = 0xFFFF800000000000;

kernelHeapSize = 512m;

SECTIONS {
    . = 0x200000 + SIZEOF_HEADERS;
    kernelPhysicalAddress = .;
    kernelVirtualAddress = . + kernelVirtualOffset;
    .multiboot : {
        KEEP(*(.multiboot));
    }
    .bootstrap : {
        *(.bootstrap);
    }
    .bootstrap-rodata : {
        *(.bootstrap-rodata);
    }
    .bootstrap-bss : {
        *(.bootstrap-bss);
    }
    . += kernelVirtualOffset;
    .text : AT(ADDR(.text) - kernelVirtualOffset) {
        *(.text*);
        KEEP(*(.smp-trampoline));
    }
    .rodata : AT(ADDR(.rodata) - kernelVirtualOffset) {
        *(.rodata*);
    }
    .data : AT(ADDR(.data) - kernelVirtualOffset) {
        *(.data*);
    }
    .test_array : AT(ADDR(.test_array) - kernelVirtualOffset) {
        __test_array_start = .;
        KEEP(*(.test_array));
        __test_array_end = .;
    }
    .init_array : AT(ADDR(.init_array) - kernelVirtualOffset) {
        __init_array_start = .;
        KEEP(*(.init_array));
        KEEP(*(.ctors));
        __init_array_end = .;
    }
    .fini_array : AT(ADDR(.fini_array) - kernelVirtualOffset) {
        __fini_array_start = ABSOLUTE(.);
        KEEP(*(.fini_array));
        KEEP(*(.dtors));
        __fini_array_end = ABSOLUTE(.);
    }
    .bss : AT(ADDR(.bss) - kernelVirtualOffset) {
        *(.bss*);
        *(common*);
    }
    kernelVirtualEnd = .;
    kernelPhysicalEnd = . - kernelVirtualOffset;
    kernelHeapStart = ALIGN(2m) + 8m;
    kernelHeapEnd = kernelHeapStart + kernelHeapSize;
    /DISCARD/ : {
        *(.comment)
        *(.note)
    }
}
