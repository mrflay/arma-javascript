/*
	Copyright (C) 2013 Simas Toleikis

	Function: JS_fnc_done

	Description:
		Check if a spawned JavaScript script is done (finished).

	Parameters:
		_this: STRING - JavaScript script handle.

	Returns:
		BOOL - If the script is done or not.
*/

#include "\JS\API.hpp"

call compile ("JavaScript" callExtension (JS_PROTOCOL_COMMAND_DONE + _this))