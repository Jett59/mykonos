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
#include <mykonos/test.h>

extern "C" {
extern test::TestFunction __test_array_start[0];
extern test::TestFunction __test_array_end[0];
}

namespace test {
bool runTests(Logger logger) {
#ifdef RUNTIME_TESTS
  logger("Running tests...\n");
  unsigned passed = 0, failed = 0;
  for (TestFunction *testFunction = __test_array_start;
       testFunction != __test_array_end; testFunction++) {
    if ((*testFunction)(logger)) {
      passed++;
    } else {
      failed++;
    }
  }
  if (failed == 0) {
    logger("All tests passed\n");
    return true;
  } else {
    logger("There were test failures\n");
    return false;
  }
#else
  logger("Skipping tests: not a debug build\n");
  return true;
#endif
}
} // namespace test
