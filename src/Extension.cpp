// Copyright (C) 2013 Simas Toleikis

#include "Extension.h"
#include "SQF.h"

// Windows API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// DLL entry point
BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved) {

	switch (fdwReason) {

		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH: {

			// TODO:

			break;
		}
	}

	return TRUE;
}

// Handle Real Virtuality callExtension API calls
void __stdcall RVExtension(char* output, int outputSize, const char* input) {

	std::string sqf = Extension::Get().Run(input);

	// Handle output buffer overflow
	if (sqf.length() > (outputSize - 1)) {

		sqf = SQF::Throw("[OOB]");
	}

	strcpy_s(output, outputSize, sqf.c_str());
}

// Constructor
Extension::Extension() {

	// Initialize JavaScript context and global object
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope handleScope;

	// Create a template for the global object
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

	// TODO: Setup global object

	// Create and enter the V8 execution context
	v8Context.Reset(isolate, v8::Context::New(isolate, NULL, global));
	v8Context->Enter();
}

// Run JavaScript code and return the result as SQF output
std::string Extension::Run(const char* input) {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope handleScope;

	// TODO: Add parallel V8_fnc_spawn support

	v8::Handle<v8::String> source = v8::String::NewFromUtf8(isolate, input);

	if (!source.IsEmpty()) {

		v8::TryCatch tryCatch;
		v8::Handle<v8::Script> script = v8::Script::Compile(source);
		v8::Handle<v8::Value> result;

		// Execute JavaScript code
		if (!script.IsEmpty()) {
			result = script->Run();
		}

		// Process unhandled script exceptions
		if (tryCatch.HasCaught()) {

			// Use SQF exception handling to report JavaScript errors
			return SQF::Throw(GetException(tryCatch));
		}
		else if (!result.IsEmpty()) {

			// Return JavaScript result as serialized SQF
			return SQF::Serialize(result);
		}
	}

	return SQF::Nil;
}

// Get V8 JavaScript exception message
std::string Extension::GetException(const v8::TryCatch &tryCatch) const {

	v8::HandleScope handleScope;

	v8::String::Utf8Value exception(tryCatch.Exception());
	v8::Handle<v8::Message> message = tryCatch.Message();
	std::string exceptionMessage;

	// Simple exception message
	if (message.IsEmpty()) {

		if (*exception) {
			exceptionMessage = std::string(*exception);
		}
	}
	// Detailed exception message
	else {

		v8::String::Utf8Value scriptSourceLine(message->GetSourceLine());
		int scriptLineNumber = message->GetLineNumber();

		// Build detailed message
		std::stringstream ss;

		ss << "[line ";
		ss << scriptLineNumber;
		ss << "] ";

		if (*exception) {
			ss << *exception;
		}

		if (*scriptSourceLine) {
			ss << ": \"";
			ss << *scriptSourceLine;
			ss << "\"";
		}

		exceptionMessage = ss.str();
	}

	return exceptionMessage;
}

// Destructor
Extension::~Extension() {

	// Release V8 context
	v8Context->Exit();
	v8Context.Dispose();
}