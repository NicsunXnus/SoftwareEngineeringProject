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

/**
 * This class is used to extract the Calls abstraction from the AST.
 */
class CallsAbstractionExtractor : public Extractor {
public:
    // Constructor
    CallsAbstractionExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, unordered_set<string>>>();
    }

    // Method to get the Calls abstraction map
    shared_ptr<map<string, unordered_set<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

    // Override methods to avoid unnecessary processing
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    
    void handleCall(std::shared_ptr<CallNode> callNode) override {

    }

    



private:
    shared_ptr<map<string, unordered_set<string>>> AbstractionStorageMap;

    // Inserts a key and value into the map
    void insertToMap(string key, string value) {
        if (AbstractionStorageMap->find(key) == AbstractionStorageMap->end()) {
            AbstractionStorageMap->insert({ key, unordered_set<string>() });
        }
        AbstractionStorageMap->at(key).insert(value);
    }
};
