// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// Headers for CppUnitTest
#include <algorithm>
#include <cassert>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "CppUnitTest.h"

using namespace std;

inline bool compare_pairs(pair<string, string> p1, pair<string, string> p2) {
  return p1.first == p2.first && p1.second == p2.second;
}

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

inline bool compare_sets(unordered_set<string> s1, unordered_set<string> s2) {
  if (s1.size() != s2.size()) {
    return false;
  }
  for (const string& element : s1) {
    if (s2.find(element) == s2.end()) {
      return false;
    }
  }
  return true;
}

inline bool compare_vector_maps(unordered_map<string, vector<string>> m1,
                                unordered_map<string, vector<string>> m2) {
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

inline bool compare_maps(unordered_map<string, unordered_set<string>> m1,
                         unordered_map<string, unordered_set<string>> m2) {
  if (m1.size() != m2.size()) {
    return false;
  }
  for (auto const& [key, value] : m1) {
    if (m2.find(key) == m2.end()) {
      return false;
    }
    if (!compare_sets(value, m2.at(key))) {
      return false;
    }
  }
  return true;
}

inline bool compare_vectors_of_maps(
    vector<unordered_map<string, vector<string>>> v1,
    vector<unordered_map<string, vector<string>>> v2) {
  /* if (v1.size() != v2.size()) {
     return false;
   }
   vector<unordered_map<string, vector<string>>> temp1 = v1;
   vector<unordered_map<string, vector<string>>> temp2 = v2;
   sort(temp1.begin(), temp1.end());
   sort(temp2.begin(), temp2.end());
   for (size_t i = 0; i < temp1.size(); ++i) {
     if (!compare_vector_maps(temp1[i], temp2[i])) {
       return false;
     }
   }
   return true;*/

  if (v1.size() != v2.size()) {
    return false;
  }

  for (std::size_t i = 0; i < v1.size(); ++i) {
    const auto& map1 = v1[i];
    const auto& map2 = v2[i];

    if (map1.size() != map2.size()) {
      return false;
    }

    for (const auto& entry : map1) {
      const auto& key = entry.first;
      const auto& value1 = entry.second;
      auto it = map2.find(key);
      if (it == map2.end()) {
        return false;
      }
      const auto& value2 = it->second;
      if (value1 != value2) {
        return false;
      }
    }
  }
}
