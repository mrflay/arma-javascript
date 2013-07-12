private "_result";

_result = "0" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "SCALAR" && {
		_result == 0
	}
})