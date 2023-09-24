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
// TODO: reference additional headers your program requires here
#include <cassert>

inline bool compare_vectors(std::vector<std::string> v1, std::vector<std::string> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    std::vector<std::string> temp1 = v1;
    std::vector<std::string> temp2 = v2;
    std::sort(temp1.begin(), temp1.end());
    std::sort(temp2.begin(), temp2.end());
    return temp1 == temp2;
}
inline bool compare_maps(std::map<std::string, std::vector<std::string>> m1, std::map<std::string, std::vector<std::string>> m2) {
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


inline bool compare_vectors_of_maps(std::vector<std::map<std::string, std::vector<std::string>>> v1, std::vector<std::map<std::string, std::vector<std::string>>> v2) {
    if (v1.size() != v2.size()) {
        return false;
    }
    std::vector<std::map<std::string, std::vector<std::string>>> temp1 = v1;
    std::vector<std::map<std::string, std::vector<std::string>>> temp2 = v2;
    std::sort(temp1.begin(), temp1.end());
    std::sort(temp2.begin(), temp2.end());
    for (size_t i = 0; i < temp1.size(); ++i) {
        if (!compare_maps(temp1[i], temp2[i])) {
            return false;
        }
    }
    return true;
}