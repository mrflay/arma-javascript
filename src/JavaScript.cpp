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

using namespace JavaScript;

// JavaScript sleep() function
v8::Handle<v8::Value> JavaScript::Sleep(const v8::Arguments &args) {

	// Cannot use sleep/uiSleep in the main thread (with JS_fnc_exec)
	if (std::this_thread::get_id() == Extension::Get().GetMainThreadID()) {
		return v8::ThrowException(v8::String::New("sleep() and uiSleep() can only be used with JS_fnc_spawn"));
	}

	// Process sleep
	if (args.Length() && args[0]->IsNumber()) {

		v8::Isolate* isolate = args.GetIsolate();

		double sleepForValue = args[0]->NumberValue();

		// Let other threads use V8 during sleep
		v8::Unlocker unlocker(isolate);

		if (sleepForValue > 0) {

			uint_fast32 sleepFor = static_cast<uint_fast32>(sleepForValue * 1000 + 0.5);
	
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
		}
	}

	return v8::Undefined();
}
