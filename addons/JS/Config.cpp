// Copyright (C) 2013 Simas Toleikis

class CfgPatches
{
	class JS
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 1.0;
		requiredAddons[] = {};
	};
};

// Public functions library
class CfgFunctions
{
	class JS
	{
		class JavaScript
		{
			class init
			{
				scope = 0;
				description = "Private function used to initialize JavaScript addon.";
				file = "\JS\fn_init.sqf";
				forcedStart = 1;
			};
			class exec
			{
				description = "Execute JavaScript code and return the value.";
				file = "\JS\fn_exec.sqf";
				headerType = -1;
			};
			class spawn
			{
				description = "Execute JavaScript code in parallel (non-blocking mode).";
				file = "\JS\fn_spawn.sqf";
				headerType = -1;
			};
			class terminate
			{
				description = "Terminate (abort) a spawned JavaScript script.";
				file = "\JS\fn_terminate.sqf";
				headerType = -1;
			};
			class done
			{
				description = "Check if a spawned JavaScript script is done (finished).";
				file = "\JS\fn_done.sqf";
				headerType = -1;
			};
			class version
			{
				description = "Get addon and JavaScript engine version information.";
				file = "\JS\fn_version.sqf";
				headerType = -1;
			};
		};
	};
};