#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"


class AbstractionExtractor : public Extractor {
public:
    ~AbstractionExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
    }

    // Gets the map of the abstraction
    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }
protected:
    std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap;

    // Inserts a key and value into the map
    void insertToAbstractionMap(string key, string value) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
        this->AbstractionStorageMap->at(key).push_back(value);
    }

};