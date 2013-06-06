/*
	Copyright (C) 2013 Simas Toleikis

	Function: JS_fnc_spawn

	Description:
		Execute JavaScript code in parallel (non-blocking mode).

	Parameters:
		_this: STRING - JavaScript code to execute in parallel.

	Returns:
		Nothing.
*/

call compile ("JavaScript" callExtension ("__S" + _this))