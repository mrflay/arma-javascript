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

// SQF output and conversion methods
namespace SQF {

	// Escape quotation (") characters in SQF output strings
	std::string Escape(const char* input);
	std::string Escape(const std::string &input);

	// Generate SQF "throw" statement
	std::string Throw(const char* message);
	std::string Throw(const std::string &message);

	// Serialize V8 JavaScript value to SQF value/statement
	std::string Serialize(const v8::Handle<v8::Value> value);

	// Generate JS_fnc_version SQF output
	std::string Version();

	// SQF "Void" data type value
	static const char* Nil = "nil";

	// SQF "Nothing" data type value
	static const char* Nothing = "";
}