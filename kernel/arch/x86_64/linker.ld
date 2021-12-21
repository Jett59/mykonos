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

SECTIONS {
    . = 0x200000 + SIZEOF_HEADERS;
    kernelPhysicalAddress = .;
    .multiboot : {
        KEEP(*(.multiboot));
    }
    .bootstrap : {
        *(.bootstrap);
    }
    .bootstrap-rodata : {
        *(.bootstrap-rodata);
    }
    .text : {
        *(.text*);
    }
    .rodata : {
        *(.rodata*);
    }
    .data : {
        *(.data*);
    }
    .bss : {
        *(.bss*);
        *(common*);
    }
    .init_array : {
        __init_array_start = .;
        KEEP(*(.init_array));
        KEEP(*(.ctors));
        __init_array_end = .;
    }
    .fini_array : {
        __fini_array_start = ABSOLUTE(.);
        KEEP(*(.fini_array));
        KEEP(*(.dtors));
        __fini_array_end = ABSOLUTE(.);
    }
    .bootstrap-bss : {
        *(.bootstrap-bss);
    }
    /DISCARD/ : {
        *(.comment)
        *(.note)
    }
    kernelPhysicalEnd = .;
}
