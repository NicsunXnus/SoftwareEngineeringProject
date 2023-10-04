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
        this->procedureStatementStorageMap = std::make_shared<map<string, pair<string, string>>>();
    }

    // Gets the map of the abstraction
    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

    // Method to abstract the extraction of designs to line up with the different abstractions
    void extractAbstractions(shared_ptr<ASTNode> astNode) {
        extractDesigns(astNode);
    }

    // Override methods to save all line numbers of the procedures
    void preProcessProgramNode(shared_ptr<ProgramNode> programNode) override {
        vector<shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
        for (const auto& procedure : procedures) {
            insertToProcedureStatementStorageMap(procedure->getProcedureName(), procedure->getStatementNumber());
        }
    }

protected:
    // map to store procedurename to the min and max values of the statement numbers
    std::shared_ptr<map<string, pair<string, string>>> procedureStatementStorageMap;
    std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap;

    // insert to procedureStatementStorageMap
    void insertToProcedureStatementStorageMap(string procedureName, string statementNumber) {
        // Insert to the map if the procedureName is not found
        if (this->procedureStatementStorageMap->find(procedureName) == this->procedureStatementStorageMap->end()) {
            this->procedureStatementStorageMap->insert({ procedureName, pair<string, string>(to_string(0), to_string(0))});
        }
        // Insert to the first value if the statement number is smaller than the current value
        if (stoi(statementNumber) < stoi(this->procedureStatementStorageMap->at(procedureName).first)) {
            this->procedureStatementStorageMap->at(procedureName).first = statementNumber;
        }
        // Insert to the second value if the statement number is larger than the current value
        if (stoi(statementNumber) > stoi(this->procedureStatementStorageMap->at(procedureName).second)) {
            this->procedureStatementStorageMap->at(procedureName).second = statementNumber;
        }
    }

    // method to get procedure name from statement number
    string getProcedureNameFromStatementNumber(string statementNumber) {
        for (const auto& [procedureName, values] : *this->procedureStatementStorageMap) {
            if (stoi(statementNumber) >= stoi(values.first) && stoi(statementNumber) <= stoi(values.second)) {
                return procedureName;
            }
        }
        return "Error: Could not find procedure name from statement number";
    }

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

};