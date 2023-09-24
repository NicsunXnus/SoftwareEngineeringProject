#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"

/**
 * This class is the base class for the Parents and Follows Abstraction extractors
 * It is used to provide default implementations and common methods for the two extractors
 * Override the relevant methods to implement the different abstractions
 */
class ParentsFollowsAbstractionBaseExtractor : public Extractor {
public:
    //Constructor
    ParentsFollowsAbstractionBaseExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
    }

    // Method to handle the different types of nodes
    virtual void handleNode(shared_ptr<ProcedureNode> procedureNode, shared_ptr<IfNode> ifNode, shared_ptr<WhileNode> whileNode) = 0;

    // Gets the map of the abstraction
    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

    // Method to abstract the extraction of designs to line up with the different abstractions
    void extractAbstractions(shared_ptr<ASTNode> astNode) {
        extractDesigns(astNode);
    }

    // Overriden method to extract the necessary designs 
    void extractDesigns(shared_ptr<ASTNode> astNode) override {
        auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode);
        auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode);
        auto ifNode = std::dynamic_pointer_cast<IfNode>(astNode);
        auto whileNode = std::dynamic_pointer_cast<WhileNode>(astNode);

        if (!programNode && !procedureNode && !ifNode && !whileNode) {
            return;
        }
        else if (programNode) {
            for (const auto& procedure : programNode->getProcedures()) {
                extractDesigns(procedure);
            }
        }
        else if (procedureNode || ifNode || whileNode) {
            handleNode(procedureNode, ifNode, whileNode);
        }
        else {
            cerr << "Unsupported ASTNode type in ParentsFollowsAbstraction." << endl;
        }
    }

protected:
    std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap;

    // Inserts a key and value into the map
    void insertToMap(string key, string value) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
        this->AbstractionStorageMap->at(key).push_back(value);
    }

    // Inserts a key and value into the map with the value being at the front of the vector
    void insertToMapFront(string key, string value) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
        this->AbstractionStorageMap->at(key).insert(this->AbstractionStorageMap->at(key).begin(), value);
    }

    // Get the value of a key in the map
    vector<string> getValueFromMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            return vector<string>();
        }
        return this->AbstractionStorageMap->at(key);
    }
};