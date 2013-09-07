private "_result";

try {
	_result = "Array.apply(null, new Array(1024 * 25)).map(Number.prototype.valueOf, 0);" call JS_fnc_exec;
}
catch {
	if (_exception == "[OOB]") then {
		_result = true;
	}
};

(not isNil "_result" && {
	(typeName _result == "BOOL" && {_result}) || (typeName _result == "ARRAY" && {
		count _result == (1024 * 25)
	})
})