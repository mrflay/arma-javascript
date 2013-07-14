private "_result";

"_JS_SleepTest = true; sleep(2); _JS_SleepTest = undefined;" call JS_fnc_spawn;
sleep(1);
_result = "this._JS_SleepTest" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "BOOL" && {
		_result
	}
})