private "_result";

_result = "1.23" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "SCALAR" && {
		_result == 1.23
	}
})