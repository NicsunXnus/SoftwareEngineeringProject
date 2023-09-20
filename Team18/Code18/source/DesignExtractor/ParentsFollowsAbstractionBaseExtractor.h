#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"


class ParentsFollowsAbstractionBaseExtractor : public Extractor {
public:
    //Constructor
    ParentsFollowsAbstractionBaseExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
    }

    virtual void handleNode(shared_ptr<ProcedureNode> procedureNode, shared_ptr<IfNode> ifNode, shared_ptr<WhileNode> whileNode) = 0;

    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

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

private:
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


};