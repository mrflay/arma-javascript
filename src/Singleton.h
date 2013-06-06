// Copyright (C) 2013 Simas Toleikis

#pragma once

// Simple singleton implementation
template<class T>
class Singleton {

public:

	// Get singleton instance
	static inline T& Get() {
		static T instance;
		return instance;
	}

};