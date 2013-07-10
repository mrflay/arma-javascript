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

#include "JavaScript.h"
#include "Extension.h"
#include "SQF.h"

// Special JavaScript number values
#define JAVASCRIPT_NAN "NaN"
#define JAVASCRIPT_POSITIVE_INFINITY "Infinity"
#define JAVASCRIPT_NEGATIVE_INFINITY "-Infinity"

// Serialize/convert V8 JavaScript value to SQF value
std::string JavaScript::ToSQF(const v8::Handle<v8::Value> value) {

	// JavaScript null and undefined are matched to SQF nil  
	if (value->IsNull() || value->IsUndefined()) {
		return SQF::Nil;
	}

	// We cannot use toString for array serialization
	if (value->IsArray()) {

		v8::Handle<v8::Array> valueArray = v8::Handle<v8::Array>::Cast(value);
		std::string sqf("[");

		// Serialize array
		uint_fast32 valueArrayLength = valueArray->Length();
		for (uint_fast32 i = 0; i < valueArrayLength; i++) {

			v8::Handle<v8::Value> arrayItem = valueArray->Get(i);

			sqf += JavaScript::ToSQF(arrayItem);

			if (i < (valueArrayLength - 1)) {
				sqf += ",";
			}
		}

		sqf += "]";

		return sqf;
	}
	// Any other value will use V8 Unicode (UTF-8) string conversion
	else {

		v8::String::Utf8Value valueString(value);

		if (*valueString) {
			
			// NOTE: This will use .toString() for objects
			std::string sqf(*valueString);

			// Handle special Number values
			if (value->IsNumber()) {
				
				// NaN is represented in SQF as nil
				if (sqf.compare(JAVASCRIPT_NAN) == 0) {
					sqf = SQF::Nil;
				}
				// Positive infinity
				else if (sqf.compare(JAVASCRIPT_POSITIVE_INFINITY) == 0) {
					sqf = SQF::InfinityPositive;
				}
				// Negative infinity
				else if (sqf.compare(JAVASCRIPT_NEGATIVE_INFINITY) == 0) {
					sqf = SQF::InfinityNegative;
				}
			}
			// Any other value than Number or Boolean is serialized as SQF string literal
			else if (!value->IsBoolean()) {
				sqf = SQF::String(sqf);
			}

			return sqf;
		}
	}

	return SQF::Nil;
}

// Global sleep() function
void JavaScript::Sleep(const v8::FunctionCallbackInfo<v8::Value>& args) {

	// NOTE: IsExecutionTerminating() doesn't seem to work when TerminateExecution()
	// is invoked by V8 thread ID. We are using a local TLS flag as a workaround.
	__declspec(thread) static bool terminateExecution = false;

	if (terminateExecution) {
		return;
	}

	v8::Isolate* isolate = args.GetIsolate();
	Extension &extension = Extension::Get();

	// Cannot use sleep/uiSleep in the main thread (with JS_fnc_exec)
	if (std::this_thread::get_id() == extension.mainThreadID) {
		
		v8::HandleScope handleScope(isolate);
		v8::ThrowException(v8::String::New("sleep() can only be used with a background script (JS_fnc_spawn)"));

		return;
	}

	// Process sleep
	if (args.Length() && args[0]->IsNumber()) {

		double sleepForValue = max(args[0]->NumberValue(), 0);

		{
			// Let other threads use V8 during sleep
			isolate->Exit();
			v8::Unlocker unlocker(isolate);

			DWORD sleepFor = static_cast<DWORD>(sleepForValue * 1000 + 0.5);
			__declspec(thread) static HANDLE terminationEvent = NULL;

			// Cache termination event handle locally
			if (terminationEvent == NULL) {

				std::string scriptHandle = Extension::GetScriptHandle(std::this_thread::get_id());
				
				extension.backgroundScriptsMutex.lock();

				auto it = extension.backgroundScripts.find(scriptHandle);

				if (it != extension.backgroundScripts.end()) {
					terminationEvent = it->second;
				}

				extension.backgroundScriptsMutex.unlock();
			}

			if (terminationEvent != NULL) {

				DWORD sleepResult = WaitForSingleObject(terminationEvent, sleepFor);

				// Termination event signal
				if (sleepResult == WAIT_OBJECT_0) {
					terminateExecution = true;
				}
			}
		}

		isolate->Enter();

		// Try to terminate V8 thread/script
		if (terminateExecution) {
			v8::V8::TerminateExecution(v8::V8::GetCurrentThreadId());
		}
	}
}
