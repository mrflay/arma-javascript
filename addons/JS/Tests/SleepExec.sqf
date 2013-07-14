private "_result";

_result = false;

try {
	"sleep(0)" call JS_fnc_exec;
}
catch {
	_result = true;
};

_result