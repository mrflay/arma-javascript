private ["_resultPos", "_resultNeg"];

_resultPos = "Number.POSITIVE_INFINITY" call JS_fnc_exec;
_resultNeg = "Number.NEGATIVE_INFINITY" call JS_fnc_exec;

(not isNil "_resultPos" && {
	typeName _resultPos == "SCALAR" && {
		str(_resultPos) == "1.#INF"
	}
})
&&
(not isNil "_resultNeg" && {
	typeName _resultNeg == "SCALAR" && {
		str(_resultNeg) == "-1.#INF"
	}
})