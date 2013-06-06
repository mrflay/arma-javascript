// Copyright (C) 2013 Simas Toleikis

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

protected:

	// Get V8 JavaScript exception message
	std::string GetException(const v8::TryCatch &tryCatch) const;

private:

	// V8 execution context
	v8::Persistent<v8::Context> v8Context;

};