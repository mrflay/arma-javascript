private "_result";

_result = '"tėst".substr(0, 2)' call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "STRING" && {
		_result == "tė"
	}
})