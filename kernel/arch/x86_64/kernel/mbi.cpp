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
   #include <mbi.h>

extern "C" {
extern multiboot::Mbi* mbiPointer;
}

   namespace multiboot {
   static void parseMbiTag(MbiTag* tag);
   void parseMbi() {
       MbiTag* tag = &mbiPointer->firstTag;
       while (tag->type != 0) {
         parseMbiTag(tag);
         tag = (MbiTag*)((uint8_t*)tag + (tag->size + 7) / 8 * 8);
       }
   }
   static void parseMbiTag(MbiTag* tag) {
       // TODO
   }
   }
