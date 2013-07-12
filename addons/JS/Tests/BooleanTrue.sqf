private "_result";

_result = "true" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "BOOL" && {
		_result
	}
})