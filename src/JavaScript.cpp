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

// JavaScript sleep() function
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

				std::string scriptHandle = SQF::ScriptHandle(std::this_thread::get_id());
				auto it = extension.backgroundScripts.find(scriptHandle);

				if (it != extension.backgroundScripts.end()) {
					terminationEvent = it->second;
				}
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
