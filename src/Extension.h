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
class Extension: public Singleton<::Extension> {

public:

	Extension();
	~Extension();

	// Run JavaScript code and return the result as SQF output
	std::string Run(const char* input);

	// V8 isolate and execution context
	v8::Isolate* isolate;
	v8::Persistent<v8::Context> context;

protected:

	// Run JavaScript code in parallel/background (non-blocking mode)
	static void Spawn(v8::Persistent<v8::Script> script);

	// Get V8 JavaScript exception message
	std::string GetException(const v8::TryCatch &tryCatch) const;

	// Generate script handle for a given thread ID
	static std::string GetScriptHandle(const std::thread::id &threadID);

private:



	// Main thread ID
	std::thread::id mainThreadID;

	// Active background scripts (script handle => termination event)
	std::unordered_map<std::string, HANDLE> backgroundScripts;
	std::mutex backgroundScriptsMutex;

	// Friends
	friend class JavaScript;
	friend class SQF;
};