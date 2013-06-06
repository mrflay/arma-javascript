// Copyright (C) 2013 Simas Toleikis

#include "Extension.h"
#include "SQF.h"

// Windows API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

// All JavaScript extension protocol commands are 3 bytes in length
#define JS_PROTOCOL_CMD_LENGTH 3

// Extension protocol commands
#define JS_PROTOCOL_INIT "__I"
#define JS_PROTOCOL_SPAWN "__S"
#define JS_PROTOCOL_TERMINATE "__T"
#define JS_PROTOCOL_VERSION "__V"

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
	if (sqf.length() > (size_t)max(outputSize - 1, 0)) {

		// TODO: Technically, instead of throwing an exception we can eliminate
		// this limitation: use an internal buffer and do self SQF calls until
		// all the available data is returned to the calling SQF script.
		sqf = SQF::Throw("[OOB]");
	}

	strcpy_s(output, outputSize, sqf.c_str());
}

// Constructor
Extension::Extension() {

	// Single (default) isolated V8 instance is used
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::HandleScope handleScope(isolate);

	// Create a template for the global object
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

	// TODO: Add JavaScript sleep() function to be used with JS_fnc_spawn
	// TODO: Add JavaScript log() function to log to ARMA RPT file
	// TODO: Add "global" property as alias for global object
	// TODO: Detect when ARMA is paused (suspend background scripts and use v8::V8::IdleNotification())

	// Create V8 execution context
	context = v8::Persistent<v8::Context>::New(isolate, v8::Context::New(isolate, NULL, global));
}

// Run JavaScript code and return the result as SQF output
std::string Extension::Run(const char* input) {

	bool isSpawn = false;

	// Process special protocol commands
	if (input[0] == '_') {

		// JS_fnc_spawn
		if (strncmp(input, JS_PROTOCOL_SPAWN, JS_PROTOCOL_CMD_LENGTH) == 0) {
			isSpawn = true;
		}
		// JS_fnc_terminate
		else if (strncmp(input, JS_PROTOCOL_TERMINATE, JS_PROTOCOL_CMD_LENGTH) == 0) {
			// TODO: Not implemented yet
		}
		// JS_fnc_version
		else if (strcmp(input, JS_PROTOCOL_VERSION) == 0) {
			return SQF::Version();
		}
		// JS_fnc_init 
		else if (strcmp(input, JS_PROTOCOL_INIT) == 0) {
			return SQF::Nothing;
		}
	}

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::Locker locker(isolate); // Critical section
	v8::Isolate::Scope isolateScope(isolate);
	v8::Context::Scope contextScope(context);
	v8::HandleScope handleScope(isolate);

	// TODO: Add JS_fnc_terminate support

	v8::Handle<v8::String> source;

	if (isSpawn) {

		// Strip protocol command
		// TODO: Can we do this better performance-wise?

		std::string sourceStr(input);
		sourceStr.erase(0 , JS_PROTOCOL_CMD_LENGTH);

		source = v8::String::NewFromUtf8(isolate, sourceStr.c_str());
	}
	else {
		source = v8::String::NewFromUtf8(isolate, input);
	}

	if (!source.IsEmpty()) {

		v8::TryCatch tryCatch;

		v8::Handle<v8::Script> script = v8::Script::Compile(source);
		v8::Handle<v8::Value> result;

		// Execute JavaScript code
		if (!script.IsEmpty()) {

			// Parallel JS_fnc_spawn support
			if (isSpawn) {

				// NOTE: The persistent V8 Script handle will be released by the background thread
				v8::Persistent<v8::Script> pScript = v8::Persistent<v8::Script>::New(isolate, script);

				// TODO: Exception handling
				std::thread bgThread(Extension::Spawn, pScript);

				bgThread.detach();
			}
			// JS_fnc_exec
			else {
				result = script->Run();
			}
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

// Run JavaScript code in parallel (non-blocking mode)
void Extension::Spawn(v8::Persistent<v8::Script> script) {

	Extension &extension = Extension::Get();

	v8::Isolate* isolate = extension.context->GetIsolate();
	v8::Locker locker(isolate); // Critical section
	v8::Isolate::Scope isolateScope(isolate);
	v8::Context::Scope contextScope(extension.context);
	v8::HandleScope handleScope(isolate);

	// TODO: Catch unhandled JavaScript exceptions and log them to ARMA RPT file
	script->Run();

	script.Dispose();
	script.Clear();
}

// Get V8 JavaScript exception message
std::string Extension::GetException(const v8::TryCatch &tryCatch) const {

	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope handleScope(isolate);

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

	// Release V8 execution context handle
	context.Dispose();
	context.Clear();
}