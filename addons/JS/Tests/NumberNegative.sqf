private ["_resultInteger", "_resultFloat"];

_resultInteger = "-123" call JS_fnc_exec;
_resultFloat = "-1.23" call JS_fnc_exec;

(not isNil "_resultInteger" && {typeName _resultInteger == "SCALAR" && {_resultInteger == -123 && {_resultInteger < 0}}})
&&
(not isNil "_resultFloat" && {typeName _resultFloat == "SCALAR" && {_resultFloat == -1.23 && {_resultFloat < 0}}})