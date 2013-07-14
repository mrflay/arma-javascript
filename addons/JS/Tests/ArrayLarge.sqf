private "_result";

_result = "Array.apply(null, new Array(1024 * 25)).map(Number.prototype.valueOf, 0);" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "ARRAY" && {
		count _result == (1024 * 25)
	}
})