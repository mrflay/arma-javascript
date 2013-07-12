private "_result";

_result = "[123, [true, null]]" call JS_fnc_exec;

(not isNil "_result" && {
	typeName _result == "ARRAY" && {
		count _result == 2 && {
			_result select 0 == 123 && {
				typeName (_result select 1) == "ARRAY" && {
					count (_result select 1) == 2 && {
						(_result select 1) select 0 && {
							isNil {(_result select 1) select 1}
						}
					}
				}
			}
		}
	}
})