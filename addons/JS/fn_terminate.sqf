/*
	Copyright (C) 2013 Simas Toleikis

	Function: JS_fnc_terminate

	Description:
		Terminate (abort) a spawned JavaScript script.

	Parameters:
		_this: STRING - JavaScript script handle.

	Returns:
		Nothing.
*/

#include "\JS\API.hpp"

call compile ("JavaScript" callExtension (JS_PROTOCOL_COMMAND_TERMINATE + _this))