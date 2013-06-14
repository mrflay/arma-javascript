/*
	Copyright (C) 2013 Simas Toleikis

	This file is part of "JavaScript for ARMA" project.

	JavaScript for ARMA is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// STL and C++ runtime headers
#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>

// Smart pointers
using std::shared_ptr;
using std::weak_ptr;
using std::unique_ptr;
using std::make_shared;

// Integer types
#include <cstdint>

typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
typedef intmax_t intmax;
typedef uintmax_t uintmax;
typedef intptr_t intptr;
typedef uintptr_t uintptr;
typedef int_fast8_t int_fast8;
typedef uint_fast8_t uint_fast8;
typedef int_fast16_t int_fast16;
typedef uint_fast16_t uint_fast16;
typedef int_fast32_t int_fast32;
typedef uint_fast32_t uint_fast32;

// V8 JavaScript Engine
#include <v8/v8.h>

// Version
#include "Version.h"