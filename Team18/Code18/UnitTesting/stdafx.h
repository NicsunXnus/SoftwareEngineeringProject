// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"

// TODO: reference additional headers your program requires here
#include <cassert>

#include "DesignExtractor/Entity.h"
#include <string>
#include <vector>

inline bool vectorEqualityWrapper(const vector<string>& vec1, const vector<string>& vec2) {
	if (vec1.size() != vec2.size()) {
		return false;
	}
	for (size_t i = 0; i < vec1.size(); ++i) {
		if (vec1[i] != vec2[i]) {
			return false;
		}
	}
	return true;
}

inline bool stringMapEqualityWrapper(const StringMap& map1, const StringMap& map2) {
	if (map1.size() != map2.size()) {
		return false;
	}
	for (const auto& entry1 : map1) {
		const string& key = entry1.first;
		if (map2.find(key) == map2.end()) {
			return false;
		}
		const vector<string>& vec1 = entry1.second;
		const vector<string>& vec2 = map2.at(key);
		if (vec1 != vec2) {
			return false;
		}
	}
	return true;
}