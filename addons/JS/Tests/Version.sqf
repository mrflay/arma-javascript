private "_result";

_result = call JS_fnc_version;

(not isNil "_result" && {
	typeName _result == "ARRAY" && {
		count _result == 3
	}
})