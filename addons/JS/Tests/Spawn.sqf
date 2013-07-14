private "_handle";

_handle = "true" call JS_fnc_spawn;

(not isNil "_handle" && {
	typeName _handle == "STRING"
})