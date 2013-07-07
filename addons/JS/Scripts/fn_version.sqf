/*
	Copyright (C) 2013 Simas Toleikis

	Function: JS_fnc_version

	Description:
		Get addon and JavaScript engine version information.

	Parameters:
		None.

	Returns:
		ARRAY - Combined version information array:
			select 0: STRING - Addon version.
			select 1: STRING - JavaScript engine name (e.g "V8").
			select 2: STRING - JavaScript engine version.
*/

#include "\JS\API.hpp"

call compile ("JavaScript" callExtension JS_PROTOCOL_COMMAND_VERSION)