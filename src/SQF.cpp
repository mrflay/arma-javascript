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

#include "SQF.h"

using namespace SQF;

// Escape quotation (") characters in SQF output strings (for C string)
std::string SQF::Escape(const char* input) {

	return SQF::Escape(std::string(input));
}

// Escape quotation (") characters in SQF output strings (for STL string)
std::string SQF::Escape(const std::string &input) {

	std::string result(input);

	// TODO: Optimize and improve the code below

	size_t pos = 0;
	while ((pos = result.find('"', pos)) != std::string::npos) {

		result.insert(pos, 1, '"');
		pos += 2;
	}

	return result;
}

// Generate SQF "throw" statement from C string
std::string SQF::Throw(const char* message) {

	return SQF::Throw(std::string(message));
}

// Generate SQF "throw" statement from STL string
std::string SQF::Throw(const std::string &message) {

	std::string sqf("throw \"");
	sqf += SQF::Escape(message);
	sqf += "\"";

	return sqf;
}

// Serialize V8 JavaScript value to SQF value/statement
std::string SQF::Serialize(const v8::Handle<v8::Value> value) {

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

			sqf += SQF::Serialize(arrayItem);

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
			
			// NOTE: This will use .toString() for objects/arrays
			std::string sqf(*valueString);

			// Numbers and booleans are serialized directly, any other value
			// will be serialized as SQF string.
			if (!value->IsNumber() && !value->IsBoolean()) {
				sqf = '"' + SQF::Escape(sqf) + '"';
			}

			return sqf;
		}
	}

	return SQF::Nil;
}

// Generate JS_fnc_version SQF output
std::string SQF::Version() {

	// Version information is returned as SQF array
	std::stringstream ss;
	std::string sqf("[\"");
	
	// Addon version
	sqf += SQF::Escape(VERSION_STR);
	sqf += "\",\"";
	
	// JavaScript engine name
	sqf += SQF::Escape(ENGINE);
	sqf += "\",\"";

	// JavaScript engine version
	sqf += SQF::Escape(v8::V8::GetVersion());
	sqf += "\"]";

	return sqf;
}