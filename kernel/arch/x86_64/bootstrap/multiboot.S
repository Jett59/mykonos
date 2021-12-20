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
/*Multiboot header*/

#define MULTIBOOT2_MAGIC 0xE85250D6
#define MULTIBOOT2_ARCHITECTURE 0

.section .multiboot

.align 8
multiboot_header:
.long MULTIBOOT2_MAGIC /*Magic*/
.long MULTIBOOT2_ARCHITECTURE /*Architecture (i386)*/
.long multiboot_header_end - multiboot_header /*Header length*/
.long 0x100000000 - (MULTIBOOT2_MAGIC + MULTIBOOT2_ARCHITECTURE + (multiboot_header_end - multiboot_header)) /*Checksum*/
/*Tags*/

/*End tag*/
.align 8
.short 0 /*Type*/
.short 0 /*Flags*/
.long 8 /*Size*/
multiboot_header_end:
