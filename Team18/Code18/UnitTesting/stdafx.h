// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cassert>
#include "DesignExtractor/Entity.h"

using namespace std;

inline bool compare_vectors(vector<string> v1, vector<string> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    vector<string> temp1 = v1;
    vector<string> temp2 = v2;
    sort(temp1.begin(), temp1.end());
    sort(temp2.begin(), temp2.end());
    return temp1 == temp2;
}

inline bool compare_maps(StringMap m1, StringMap m2) {
    if (m1.size() != m2.size()) {
        return false;
    }
    for (auto const& [key, value] : m1) {
        if (m2.find(key) == m2.end()) {
            return false;
        }
        if (!compare_vectors(value, m2.at(key))) {
            return false;
        }
    }
    return true;
}

inline bool compare_vectors_of_maps(vector<StringMap> v1, vector<StringMap> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    vector<StringMap> temp1 = v1;
    vector<StringMap> temp2 = v2;
    sort(temp1.begin(), temp1.end());
    sort(temp2.begin(), temp2.end());
    for (size_t i = 0; i < temp1.size(); ++i) {
        if (!compare_maps(temp1[i], temp2[i])) {
            return false;
        }
    }
    return true;
}
