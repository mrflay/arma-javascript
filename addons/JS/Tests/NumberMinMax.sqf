private ["_resultMax", "_resultMin"];

_resultMax = "Number.MAX_VALUE" call JS_fnc_exec;
_resultMin = "-Number.MAX_VALUE" call JS_fnc_exec;

(not isNil "_resultMax" && {typeName _resultMax == "SCALAR" && {_resultMax > 0}})
&&
(not isNil "_resultMin" && {typeName _resultMin == "SCALAR" && {_resultMin < 0}})