/*
	Copyright (C) 2013 Simas Toleikis

	A simple unit testing script for JavaScript addon.
	Use execVM "\JS\Tests\_Run.sqf" to run all tests.
*/

#define TEST(NAME) (##NAME## call _runTest)

hintSilent "Running @JS addon tests...";

// Run all tests in parallel, non-blocking mode
[] spawn {

	sleep 0.1;

	private ["_runTest", "_pass", "_fail", "_hint"];

	_pass = [];
	_fail = [];

	// Run a single unit test
	_runTest = {

		if (typeName _this != "STRING") exitWith {};

		private ["_result"];

		// Execute unit test file
		try {
			_result = call compile preprocessFileLineNumbers format ["\JS\Tests\%1.sqf", _this];
		}
		catch {
			_result = false;
		};

		// Log test result
		if (not isNil "_result" && {typeName _result == "BOOL" && {_result}}) then {
			_pass set [count _pass, _this];
		}
		else {
			_fail set [count _fail, _this];
		};
	};

	// Run tests
	TEST("Number");
	TEST("NumberFloat");
	TEST("NumberNegative");
	TEST("NumberZero");
	TEST("NumberScientific");
	TEST("NumberMinMax");
	TEST("NumberInfinity");
	TEST("NumberNaN");
	TEST("BooleanTrue");
	TEST("BooleanFalse");
	TEST("BooleanStatement");
	TEST("Array");
	TEST("ArrayEmpty");
	TEST("ArrayNested");
	TEST("ArrayLarge");
	TEST("String");
	TEST("StringUTF8");
	TEST("StringQuoteSingle");
	TEST("StringQuoteDouble");
	TEST("StringQuoteUTF8");
	TEST("StringLarge");
	TEST("Object");
	TEST("ExceptionSyntax");
	TEST("ExceptionUser");
	TEST("Null");
	TEST("Undefined");
	TEST("Version");
	TEST("Spawn");
	TEST("Done");
	TEST("Terminate");
	TEST("Sleep");
	TEST("SleepExec");

	// All tests pass
	if (count _fail == 0) then {
		_hint = format["All @JS addon tests <t color='#00ff00'>PASSED</t>!<br />(%1 total)", count _pass];
	}
	// Show failed tests
	else {

		_hint = format["Some @JS addon tests <t color='#ff0000'>FAILED</t>!<br />(%1 total)<br /><br />", count _fail];

		{
			_hint = (_hint + _x + "<br />");
		}
		forEach _fail;
	};

	// Show test results as hint
	hint parseText _hint;
};

nil