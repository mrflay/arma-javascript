private "_result";

_result = false;

try {
	loadFile("\JS\Tests\ExceptionSyntax.js") call JS_fnc_exec;
}
catch {
	if (_exception == '[line 2] SyntaxError: Unexpected Identifier: "an error"') then {
		_result = true;
	}
};

_result