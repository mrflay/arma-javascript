/*
	Copyright (C) 2013 Simas Toleikis

	Function: (private) JS_fnc_init

	Description:
		Private function used to initialize JavaScript addon.

	Parameters:
		None.

	Returns:
		Nothing.
*/

#include "\JS\API.hpp"

// Load the DLL extension (and initialize JS addon)
"JavaScript" callExtension JS_PROTOCOL_COMMAND_INIT;

nil