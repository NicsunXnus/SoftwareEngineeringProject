#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>
#include <utility>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"


class AbstractionExtractor : public Extractor {
public:
    AbstractionExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
    }

    // Gets the map of the abstraction
    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

    // Method to abstract the extraction of designs to line up with the different abstractions
    virtual void extractAbstractions(shared_ptr<ASTNode> astNode) {
        extractDesigns(astNode);
    }

    

protected:
    std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap;

    
    // Inserts a key and value into the map
    void insertToAbstractionMap(string key, string value) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
        // Insert only if the value is not found in the vector
        if (std::find(this->AbstractionStorageMap->at(key).begin(), this->AbstractionStorageMap->at(key).end(), value) == this->AbstractionStorageMap->at(key).end()) {
            this->AbstractionStorageMap->at(key).push_back(value);
        }
    }

    // Inserts a key into the map
    void insertToAbstractionMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
    }

};