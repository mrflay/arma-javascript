// Copyright (C) 2013 Simas Toleikis

#pragma once

#define TITLE "JavaScript for ARMA"
#define DESCRIPTION "JavaScript extension for ARMA"
#define ENGINE "V8"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef NDEBUG
	#define VERSION_STR TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR)
#else
	#define VERSION_STR TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "-dev"
#endif
