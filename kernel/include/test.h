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
#ifndef _TEST_H
#define _TEST_H

namespace test {
typedef void (*Logger)(const char *);
typedef bool (*TestFunction)(Logger);

bool runTests(Logger logger);
} // namespace test

#define ADD_TEST(FUNCTION_NAME)                                                \
  ::test::TestFunction testTable_##FUNCTION_NAME                               \
      __attribute__((section(".test_array"))) = &FUNCTION_NAME

#endif