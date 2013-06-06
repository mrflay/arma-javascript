// Copyright (C) 2013 Simas Toleikis

#pragma once

#include "Common.h"

// SQF output and conversion methods
namespace SQF {

	// Escape quotation (") characters in SQF output strings
	std::string Escape(const char* input);
	std::string Escape(const std::string &input);

	// Generate SQF "throw" statement
	std::string Throw(const char* message);
	std::string Throw(const std::string &message);

	// Serialize V8 JavaScript value to SQF value/statement
	std::string Serialize(const v8::Handle<v8::Value> value);

	// SQF "nil" value (Void data type)
	static const char* Nil = "nil";
}