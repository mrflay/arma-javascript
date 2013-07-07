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
class SQF {

public:

	// Escape quotation (") characters in SQF output strings
	static std::string Escape(const char* input);
	static std::string Escape(const std::string &input);

	// Generate SQF "throw" statement
	static std::string Throw(const char* message);
	static std::string Throw(const std::string &message);

	// Serialize V8 JavaScript value to SQF value/statement
	static std::string Serialize(const v8::Handle<v8::Value> value);

	// Generate JS_fnc_version SQF output
	static std::string Version();

	// Generate SQF script handle for a given thread ID
	static std::string ScriptHandle(const std::thread::id &threadID);

	// SQF "Void" data type value
	static const char* Nil;

	// SQF "Nothing" data type value
	static const char* Nothing;

	// SQF "Boolean" data type values
	static const char* True;
	static const char* False;
};