#ifndef ADAPTER_H
#define ADAPTER_H

#pragma once
#include<stdio.h>
#include <string>
#include <vector>
#include <map> 
#include <unordered_set>

using namespace std;

static shared_ptr<map<string, unordered_set<string>>> convertVectorToUnorderedSet(shared_ptr<map<string, vector<string>>> stringMap) const {
    shared_ptr<map<string, unordered_set<string>>> resultSet = make_shared<map<string, unordered_set<string>>>();
    for (const auto& pair : *stringMap) {
        const string& key = pair.first;
        const vector<string>& values = pair.second;

        // Create an unordered_set and add all values from the vector to it.
        unordered_set<string> valueSet(values.begin(), values.end());

        // Insert the key-value pair into the result map.
        resultSet->insert(make_pair(key, valueSet));
    }
    return resultSet;
}

// Used to convert the full ParentsStar/FollowsStar maps into the Parents/Follows maps
static shared_ptr<map<string, unordered_set<string>>> convertParentsFollowsStarToParentsFollows(shared_ptr<map<string, vector<string>>> stringMap) const {
    shared_ptr<map<string, unordered_set<string>>> resultSet = make_shared<map<string, unordered_set<string>>>();
    for (const auto& pair : *stringMap) {
        const string& key = pair.first;
        const vector<string>& values = pair.second;
        unordered_set<string> valueSet(values.begin(), values.begin() + 1);
        resultSet->insert(make_pair(key, valueSet));
    }
    return resultSet;
}
#endif