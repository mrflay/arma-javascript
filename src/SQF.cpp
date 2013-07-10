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

// SQF quote characters
#define SQF_QUOTE_SINGLE '\''
#define SQF_QUOTE_DOUBLE '"'
#define SQF_QUOTES "'\""

// SQF plain data type values
const char* SQF::Nil = "nil";
const char* SQF::Nothing = "";
const char* SQF::True = "true";
const char* SQF::False = "false";
const char* SQF::InfinityPositive = "1e39";
const char* SQF::InfinityNegative = "-1e39";

// Generate SQF string literal
std::string SQF::String(const std::string &input) {

	std::string sqf;

	// Simple heuristics to keep memory (re)allocations to a minimum
	// Extra 10% is reserved for quote escape characters
	sqf.reserve(2 + (size_t)(input.size() * 1.1));

	// Quick check for at least one quote in the string
	size_t pos = input.find_first_of(SQF_QUOTES);

	// Fast path for strings without any quotes
	if (pos == std::string::npos) {
		sqf += SQF_QUOTE_DOUBLE;
		sqf += input;
		sqf += SQF_QUOTE_DOUBLE;
	}
	else {

		char enclosureQuote = SQF_QUOTE_DOUBLE;
		if (input[pos] == SQF_QUOTE_DOUBLE) {
			enclosureQuote = SQF_QUOTE_SINGLE;
		}

		sqf += enclosureQuote;

		// Sequence until (and including) the first quote doesn't need any escaping
		sqf.append(input, 0, pos + 1);

		// Escape any extra enclosure quotes
		size_t lastPos;
		do {

			lastPos = pos + 1;

			pos = input.find(enclosureQuote, lastPos);

			if (pos != std::string::npos) {

				sqf.append(input, lastPos, (pos - lastPos) + 1);
				sqf += enclosureQuote; // SQF double quote escaping
			}
			else {
				sqf.append(input, lastPos, pos);
			}
		}
		while (pos != std::string::npos);

		sqf += enclosureQuote;
	}

	return sqf;
}

// Generate SQF "throw ..." statement
std::string SQF::Throw(const std::string &message) {

	std::string sqf("throw ");

	sqf += SQF::String(message);

	return sqf;
}