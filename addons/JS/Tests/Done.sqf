private ["_handle", "_result1", "_result2"];

_handle = "sleep(0.5)" call JS_fnc_spawn;
_result1 = _handle call JS_fnc_done;
sleep(1);
_result2 = _handle call JS_fnc_done;

(not isNil "_result1" && {
	typeName _result1 == "BOOL" && {
		not _result1
	}
})
&&
(not isNil "_result2" && {
	typeName _result2 == "BOOL" && {
		_result2
	}
})