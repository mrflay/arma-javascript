private "_result";

_result = "new Object()" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "STRING" && {
		_result == "[object Object]"
	}
})