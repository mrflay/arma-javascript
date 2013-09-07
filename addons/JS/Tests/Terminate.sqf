private ["_handle", "_result"];

_handle = "sleep(10)" call JS_fnc_spawn;
sleep(0.5);
_result = _handle call JS_fnc_terminate;

waitUntil {
	_handle call JS_fnc_done
};

(not isNil "_result" && {
	typeName _result == "BOOL" && {
		_result
	}
})