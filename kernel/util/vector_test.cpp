/*
    Copyright (C) 2022 Jett Thompson

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
#ifdef RUNTIME_TESTS

#include <mykonos/test.h>
#include <mykonos/vector.h>

namespace util {
namespace test {
bool vectorTest(::test::Logger logger) {
  logger("VectorTest:\n");
  logger("VectorTest: Succeeded\n");
  return true;
}
ADD_TEST(vectorTest);
} // namespace test
} // namespace util

#endif