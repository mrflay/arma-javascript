/*
	Copyright (C) 2013 Simas Toleikis

	This file is part of "JavaScript for ARMA" project.

	JavaScript for ARMA is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#define TITLE "JavaScript for ARMA"
#define DESCRIPTION "JavaScript extension for ARMA"
#define ENGINE "V8"
#define VERSION_MAJOR 0
#define VERSION_MINOR 5

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef NDEBUG
	#define VERSION_STR TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR)
#else
	#define VERSION_STR TOSTRING(VERSION_MAJOR) "." TOSTRING(VERSION_MINOR) "-dev"
#endif
