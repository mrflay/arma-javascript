/*
	Copyright (C) 2013 Simas Toleikis

	Function: JS_fnc_exec

	Description:
		Execute JavaScript code and return the value.

	Parameters:
		_this: STRING - JavaScript code to execute.

	Returns:
		Anything.
*/

call compile ("JavaScript" callExtension _this)