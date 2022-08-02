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
  Vector<int> vector;
  vector.push_back(17);
  vector.push_back(23);
  if (vector.pop_back() != 23) {
    logger("VectorTest: Failed on popping 23\n");
    return false;
  }
  if (vector.pop_back() != 17) {
    logger("VectorTest: Failed on popping 17\n");
    return false;
  }
  vector.push_back(27);
  vector.push_back(144);
  vector.push_back(123);
  if (vector[0] != 27) {
    logger("VectorTest: Random access failed on 0\n");
    return false;
  }
  if (vector[1] != 144) {
    logger("VectorTest: Random access failed on 1\n");
    return false;
  }
  if (vector[2] != 123) {
    logger("VectorTest: Random access failed on 2\n");
    return false;
  }
  vector[1] = 1700;
  if (vector[1] != 1700) {
    logger("VectorTest: Random access assignment failed\n");
    return false;
  }
  vector.pop_back();
  vector.pop_back();
  vector.pop_back();
  for (int i = 0; i < 1000; i++) {
    vector.push_back(i);
  }
  for (int i = 0; i < 1000; i++) {
    if (vector[i] != i) {
      logger("VectorTest: Random access failed during stress testing\n");
      return false;
    }
  }
  logger("VectorTest: Succeeded\n");
  return true;
}
ADD_TEST(vectorTest);
}  // namespace test
}  // namespace util

#endif