private "_result";

_result = '"test"' call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "STRING" && {
		_result == "test"
	}
})