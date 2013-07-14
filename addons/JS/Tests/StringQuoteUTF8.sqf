private "_result";

_result = '"t''ėst".substr(0, 3)' call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "STRING" && {
		_result == "t'ė"
	}
})