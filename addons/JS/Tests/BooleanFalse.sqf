private "_result";

_result = "false" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "BOOL" && {
		not _result
	}
})