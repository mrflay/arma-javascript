private "_result";

_result = '(new Array(1024 * 100)).join("x")' call JS_fnc_exec; // ~100 KB

(not isNil "_result" && {
	typeName _result == "STRING" && {
		count (toArray _result) == (1024 * 100)
	}
})