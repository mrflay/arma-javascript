/*
	Copyright (C) 2013 Simas Toleikis

	A simple unit testing script for JavaScript addon.
	Use 'execVM "\JS\Scripts\RunTests.sqf"' to execute.
*/

private ["_runTest", "_pass", "_fail", "_hint"];

_pass = [];
_fail = [];

// Run a single unit test
_runTest = {

	if (typeName _this != "STRING") exitWith {};

	private ["_result"];

	_result = call compile preprocessFileLineNumbers ("\JS\Tests\" + _this + ".sqf");

	// Save test result
	if (not isNil "_result" && {typeName _result == "BOOL" && {_result}}) then {
		_pass set [count _pass, _this];
	}
	else {
		_fail set [count _fail, _this];
	};
};

#define TEST(NAME) (##NAME## call _runTest)

// Run tests
TEST("NumberInteger");
TEST("NumberFloat");
TEST("NumberNegative");
TEST("NumberZero");
TEST("NumberScientific");
TEST("NumberMinMax");
TEST("NumberInfinity");
TEST("NumberNaN");

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

nil