private ["_resultNeg", "_resultPos"];

_resultNeg = "123e-5" call JS_fnc_exec;
_resultPos = "123e+5" call JS_fnc_exec;

(not isNil "_resultNeg" && {
	typeName _resultNeg == "SCALAR" && {
		_resultNeg == 0.00123
	}
})
&&
(not isNil "_resultPos" && {
	typeName _resultPos == "SCALAR" && {
		_resultPos == 12300000
	}
})