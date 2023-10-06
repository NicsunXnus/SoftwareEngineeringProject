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

class WithExtractor : public Extractor {
public:
    // Constructor
    WithExtractor() {
        this->callProcNameMap = std::make_shared<map<string, unordered_set<string>>>();
        this->readVarNameMap = std::make_shared<map<string, unordered_set<string>>>();
        this->printVarNameMap = std::make_shared<map<string, unordered_set<string>>>();
    }

    void handleCall(std::shared_ptr<CallNode> callNode) override {
        string statementNumber = to_string(callNode->getStatementNumber());
        string procedureCalledName = callNode->getProc()->getName();
        insertToMap(statementNumber, procedureCalledName, this->callProcNameMap);
    }

    void handleRead(std::shared_ptr<ReadNode> readNode) override {
        string statementNumber = to_string(readNode->getStatementNumber());
        string variableName = readNode->getVar()->getValue();
        insertToMap(statementNumber, variableName, this->readVarNameMap);
    }

    void handlePrint(std::shared_ptr<PrintNode> printNode) override {
        string statementNumber = to_string(printNode->getStatementNumber());
        string variableName = printNode->getVar()->getValue();
        insertToMap(statementNumber, variableName, this->printVarNameMap);
    }

    // Get map of callProcNameMap
    std::shared_ptr<map<string, unordered_set<string>>> getCallProcNameMap() {
        return this->callProcNameMap;
    }

    // Get map of readVarNameMap
    std::shared_ptr<map<string, unordered_set<string>>> getReadVarNameMap() {
        return this->readVarNameMap;
    }

    // Get map of printVarNameMap
    std::shared_ptr<map<string, unordered_set<string>>> getPrintVarNameMap() {
        return this->printVarNameMap;
    }

private:
    std::shared_ptr<map<string, unordered_set<string>>> callProcNameMap;
    std::shared_ptr<map<string, unordered_set<string>>> readVarNameMap;
    std::shared_ptr<map<string, unordered_set<string>>> printVarNameMap;

    // Method to insert to the any map
    void insertToMap(string key, string value, std::shared_ptr<map<string, unordered_set<string>>> map) {
        if (map->find(key) == map->end()) {
            map->insert({ key, unordered_set<string>() });
        }
        map->at(key).insert(value);
    }

};