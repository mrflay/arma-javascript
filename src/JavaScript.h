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

#include "Common.h"

// JavaScript bindings
class JavaScript {

public:

	// sleep()
	static void Sleep(const v8::FunctionCallbackInfo<v8::Value>& args);

};