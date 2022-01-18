/*
    Copyright (C) 2022  Jett Thompson

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

#include <mykonos/string.h>
#include <mykonos/test.h>

namespace test {
bool memcmpTest(::test::Logger logger) {
  logger("memcmpTest:\n");
  char buffer1[] = "Hello, World!";
  char buffer2[] = "Hello, There!";
  char buffer3[] = "Test";
  char buffer4[] = "Best";
  if (memcmp(buffer1, buffer1, sizeof(buffer1)) != 0) {
    logger("memcmpTest: equality test failed\n");
    return false;
  }
  if (memcmp(buffer2, buffer2, sizeof(buffer2)) != 0) {
    logger("memcmpTest: equality test failed\n");
    return false;
  }
  if (memcmp(buffer3, buffer3, strlen(buffer3)) != 0) {
    logger("memcmpTest: Equality test failed on small array\n");
    return false;
  }
  if (memcmp(buffer4, buffer4, strlen(buffer4)) != 0) {
    logger("memcmpTest: Equality test failed on small array\n");
    return false;
  }
  logger("memcmpTest: Passed equality test\n");
  if (memcmp(buffer1, buffer2, sizeof(buffer1)) == 0) {
    logger("memcmpTest: Failed inequality test\n");
    return false;
  }
  if (memcmp(buffer2, buffer1, sizeof(buffer2)) == 0) {
    logger("memcmpTest: Failed inequality test\n");
    return false;
  }
  if (memcmp(buffer3, buffer4, strlen(buffer3)) == 0) {
    logger("memcmpTest: Inequality test failed on small array\n");
    return false;
  }
  if (memcmp(buffer4, buffer3, strlen(buffer4) == 0)) {
    logger("memcmpTest: Inequality test failed on small array\n");
    return false;
  }
  logger("memcmpTest: Passed inequality test\n");
  logger("memcmpTest: Succeeded\n");
  return true;
}
ADD_TEST(memcmpTest);
} // namespace test

#endif