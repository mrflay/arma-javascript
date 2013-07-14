private "_result";

_result = false;

try {
	'throw new Error("my")' call JS_fnc_exec;
}
catch {
	if (_exception == '[line 1] Error: my: "throw new Error("my")"') then {
		_result = true;
	};
};

_result