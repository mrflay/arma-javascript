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

#include "Extension.h"
#include "SQF.h"
#include "JavaScript.h"

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

	// TODO: Use a static pre-allocated std::string buffer for output
	std::string sqf = Extension::Get().Run(input);

	// Handle output buffer overflow
	if (sqf.length() > (size_t)max(outputSize - 1, 0)) {

		// TODO: Technically, instead of throwing an exception we can eliminate
		// this limitation: use an internal buffer and do self SQF calls until
		// all the available data is returned to the calling SQF script.
		sqf = SQF::Throw("[OOB]");
	}

	// Workaround for ARMA buffer overflow issue in Debug build of DLL
	#ifdef _DEBUG
		// Disable strcpy_s initial buffer fill with 0xFD
		size_t oldThreshold = _CrtSetDebugFillThreshold(0);
	#endif

	// Pass back the result to ARMA
	strcpy_s(output, outputSize, sqf.c_str());

	// Restore initial buffer fill threshold
	#ifdef _DEBUG
		_CrtSetDebugFillThreshold(oldThreshold);
	#endif
}

// Constructor
Extension::Extension(): isolate(NULL) {

	// Main execution thread ID is used for sleep/uiSleep constrain checks
	mainThreadID = std::this_thread::get_id();

	// Single (default) isolated V8 instance is used.
	// TODO: Consider using separate isolates for each ARMA addon (based on PBO prefix?)
	isolate = v8::Isolate::GetCurrent();

	v8::HandleScope handleScope(isolate);
	v8::PropertyAttribute builtInPropAttr = (v8::PropertyAttribute)(v8::DontDelete | v8::ReadOnly);

	// Create a template for the global object
	v8::Handle<v8::ObjectTemplate> global = v8::ObjectTemplate::New();

	// sleep() function
	global->Set(v8::String::NewSymbol("sleep"), v8::FunctionTemplate::New(JavaScript::Sleep), builtInPropAttr);

	// TODO: Add "global" property as alias for global object
	// TODO: Add JavaScript log() function to log to ARMA RPT file
	// TODO: Detect when ARMA is paused (suspend background scripts and use v8::V8::IdleNotification())

	// Create V8 execution context
	context.Reset(isolate, v8::Context::New(isolate, NULL, global));
}

// Run JavaScript code and return the result as SQF output
std::string Extension::Run(const char* input) {

	bool isSpawn = false;

	// Fast path to process special protocol commands
	if (input[0] == JS_PROTOCOL_COMMAND && input[1] != '\0') {

		// JS_fnc_spawn
		if (input[1] == JS_PROTOCOL_TOKEN_SPAWN) {
			isSpawn = true;
		}
		// JS_fnc_terminate
		else if (input[1] == JS_PROTOCOL_TOKEN_TERMINATE) {
			
			std::string scriptHandle(input + JS_PROTOCOL_LENGTH);

			// TODO: Need a mutex here

			auto it = backgroundScripts.find(scriptHandle);

			// Bad script handle (or the background script is already finished)
			if (it == backgroundScripts.end()) {
				return SQF::False;
			}

			// Signal script termination event
			SetEvent(it->second);

			// TODO: Should we block and wait until the background script is terminated?

			return SQF::True;
		}
		// JS_fnc_done
		else if (input[1] == JS_PROTOCOL_TOKEN_DONE) {
			
			std::string scriptHandle(input + JS_PROTOCOL_LENGTH);

			auto it = backgroundScripts.find(scriptHandle);

			if (it == backgroundScripts.end()) {
				return SQF::True;
			}

			return SQF::False;
		}
		// JS_fnc_version
		else if (input[1] == JS_PROTOCOL_TOKEN_VERSION) {

			// Version information is returned as SQF array
			std::string sqf("[");
	
			// Addon version
			sqf += SQF::String(VERSION_STR);
			sqf += ",";
	
			// JavaScript engine name
			sqf += SQF::String(ENGINE);
			sqf += ",";

			// JavaScript engine version
			sqf += SQF::String(v8::V8::GetVersion());
			sqf += "]";

			return sqf;
		}
		// JS_fnc_init
		else if (input[1] == JS_PROTOCOL_TOKEN_INIT) {
			
			// Initialization is part of Singleton constructor
			return SQF::Nothing;
		}
	}

	v8::Locker locker(isolate); // Critical section
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);
	v8::Context::Scope contextScope(v8::Local<v8::Context>::New(isolate, context));

	v8::Handle<v8::String> source;

	if (isSpawn) {
		source = v8::String::NewFromUtf8(isolate, input + JS_PROTOCOL_LENGTH);
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
				v8::Persistent<v8::Script> backgroundScript = v8::Persistent<v8::Script>::New(isolate, script);

				try {

					// NOTE: We do not need a mutex here because the V8 Locker is active

					std::thread backgroundThread(Extension::Spawn, backgroundScript);
					std::string scriptHandle = GetScriptHandle(backgroundThread.get_id());

					// Store background script handle and termination event
					backgroundScripts[scriptHandle] = CreateEvent(NULL, TRUE, FALSE, NULL);

					backgroundThread.detach();

					return SQF::String(scriptHandle);
				}
				catch (...) {
					return SQF::Nil; // System error
				}
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
			return JavaScript::ToSQF(result);
		}
	}

	return SQF::Nil;
}

// Run JavaScript code in parallel (non-blocking mode)
void Extension::Spawn(v8::Persistent<v8::Script> script) {

	Extension &extension = Extension::Get();

	v8::Locker locker(extension.isolate); // Critical section

	v8::Isolate::Scope isolateScope(extension.isolate);
	v8::HandleScope handleScope(extension.isolate);
	v8::Context::Scope contextScope(v8::Local<v8::Context>::New(extension.isolate, extension.context));

	v8::TryCatch tryCatch;

	// TODO: Catch unhandled JavaScript exceptions and log them to ARMA RPT file
	script->Run();

	std::string scriptHandle = GetScriptHandle(std::this_thread::get_id());

	auto it = extension.backgroundScripts.find(scriptHandle);

	// Clean up
	if (it != extension.backgroundScripts.end()) {

		CloseHandle(it->second);
		extension.backgroundScripts.erase(scriptHandle);
	}	

	script.Dispose();
	script.Clear();
}

// Get V8 JavaScript exception message
std::string Extension::GetException(const v8::TryCatch &tryCatch) const {

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

// Generate script handle for a given thread ID
std::string Extension::GetScriptHandle(const std::thread::id &threadID) {

	std::stringstream ss;

	// SQF representation of the spawned script handle
	ss << JS_PROTOCOL_COMMAND << JS_PROTOCOL_TOKEN_SPAWN;
	ss << threadID;

	return ss.str();
}

// Destructor
Extension::~Extension() {

	// Release V8 execution context handle
	context.Dispose();
	context.Clear();
}