private "_result";

_result = "[123, true, null]" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "ARRAY" && {
		count _result == 3 && {
			_result select 0 == 123 && {
				_result select 1 && {
					isNil {_result select 2}
				}
			}
		}
	}
})