// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include "CppUnitTest.h"
// TODO: reference additional headers your program requires here
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <cassert>
#include "DesignExtractor/Entity.h"

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