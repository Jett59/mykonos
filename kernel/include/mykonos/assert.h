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
#ifndef _MYKONOS_ASSERT_H
#define _MYKONOS_ASSERT_H

#include <mykonos/kpanic.h>

// TODO: Move this to a better place
#define STRINGIFY(X) #X

#ifndef DEBUG
#define assert(EXPRESSION, MESSAGE)                                                               \
  [&] {                                                                                           \
    auto result = (EXPRESSION);                                                                   \
    if (!result) {                                                                                \
      kpanic(STRINGIFY(MESSAGE) "\nAssertion failed: " STRINGIFY(EXPRESSION) " (file " STRINGIFY( \
          __FILE__) ", line " STRINGIFY(__LINE__) ")");                                           \
    }                                                                                             \
    return result;                                                                                \
  }()
#else
#define ASSERT(EXPRESSION, MESSAGE) (EXPRESSION) /* Not checked */
#endif

#endif