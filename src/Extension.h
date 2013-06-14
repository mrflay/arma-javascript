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
#include "Singleton.h"

// Real Virtuality extension API exports
extern "C"
{
	__declspec(dllexport) void __stdcall RVExtension(char* output, int outputSize, const char* input);
}

// Main Extension component
class Extension: public Singleton<Extension> {

public:

	Extension();
	~Extension();

	// Run JavaScript code and return the result as SQF output
	std::string Run(const char* input);

	// Get main extension thread ID
	inline const std::thread::id& GetMainThreadID() const { return mainThreadID; }

protected:

	// Run JavaScript code in parallel (non-blocking mode)
	static void Spawn(v8::Persistent<v8::Script> script);

	// Get V8 JavaScript exception message
	std::string GetException(const v8::TryCatch &tryCatch) const;

private:

	// V8 execution context
	v8::Persistent<v8::Context> context;

	// Main thread ID
	std::thread::id mainThreadID;

};