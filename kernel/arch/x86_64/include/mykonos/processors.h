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
#ifndef _MYKONOS_PROCESSORS_H
#define _MYKONOS_PROCESSORS_H

#define PROCESSOR_CALLBACK_INTERRUPT 0xfe

#include <mykonos/callback.h>

namespace processors {
// Run the specified callback on CPU cpuNumber. The function returns when the
// callback returns
void runOn(unsigned cpuNumber, callback::Callback<bool> &&callback);

// Internal function. Called when the CPU receives a request to execute a
// callback
void receiveCall();
} // namespace processors

#endif