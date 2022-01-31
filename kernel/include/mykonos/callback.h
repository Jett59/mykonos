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
#ifndef _MYKONOS_CALLBACK_H
#define _MYKONOS_CALLBACK_H

namespace callback {
// Note: Don't use with void return type.
template <typename Result, typename... ParameterTypes> class Callback {
private:
  bool run = false;
  virtual Result invoke(ParameterTypes...);

public:
  Result operator()(ParameterTypes... args) {
    Result res = invoke(args...);
    run = true;
    return res;
  }
  bool hasRun() { return run; }
};

template <typename LambdaType, typename Result, typename... ParameterTypes>
class Lambda : public Callback<Result, ParameterTypes...> {
private:
  LambdaType lambda;

public:
  Lambda<LambdaType, Result, ParameterTypes...>(LambdaType lambda)
      : lambda(lambda) {}

  virtual Result invoke(ParameterTypes... args) { return lambda(args...); }
};
} // namespace callback

#endif