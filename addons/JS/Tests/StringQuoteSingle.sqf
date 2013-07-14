private "_result";

_result = '"t''''est''"' call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "STRING" && {
		_result == "t''est'"
	}
})