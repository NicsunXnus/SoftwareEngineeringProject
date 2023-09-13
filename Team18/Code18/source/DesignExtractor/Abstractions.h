#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"

/**
* This class represents the Abstractions component of the Design Extractor.
*/
class Abstractions {
public:
    virtual ~Abstractions() = default;

    virtual void extractAbstraction(shared_ptr<ASTNode> astNode) = 0;

    // Inserts a key and value into the map
    void insertToAbstractionMap(string key, string value, std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap) {
        if (AbstractionStorageMap->find(key) == AbstractionStorageMap->end()) {
            (*AbstractionStorageMap)[key] = vector<string>();
        }
        (*AbstractionStorageMap)[key].push_back(value);
    }

    // Inserts a key and value into the map with the value being at the front of the vector
    void insertToAbstractionMapAddToFront(string key, string value, std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap) {
        if (AbstractionStorageMap->find(key) == AbstractionStorageMap->end()) {
            (*AbstractionStorageMap)[key] = vector<string>();
        }
        (*AbstractionStorageMap)[key].insert((*AbstractionStorageMap)[key].begin(), value);
    }

private:


};

class ParentFollowsAbstraction : public Abstractions {
public:
    void extractAbstraction(shared_ptr<ASTNode> astNode) override {
        auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode);
        auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode);
        auto ifNode = std::dynamic_pointer_cast<IfNode>(astNode);
        auto whileNode = std::dynamic_pointer_cast<WhileNode>(astNode);

        if (!programNode && !procedureNode && !ifNode && !whileNode) {
            return;
        }
        else if (programNode) {
            for (const auto& procedure : programNode->getProcedures()) {
                extractAbstraction(procedure);
            }
        }
        else if (procedureNode || ifNode || whileNode) {
            vector<shared_ptr<StatementNode>> statements;
            int parentStatementNumber = -1;

            if (procedureNode) {
                statements = procedureNode->getStatements();
            }
            else if (ifNode) {
                statements = ifNode->getStatements();
                parentStatementNumber = ifNode->getStatementNumber();
            }
            else if (whileNode) {
                statements = whileNode->getStatements();
                parentStatementNumber = whileNode->getStatementNumber();
            }
            
            // Used for Follows abstraction
            // Extract the line numbers of the statements and insert them into the FollowsStorageMap
            vector<int> equallyNestedStatements = vector<int>();
            for (const auto& statement : statements) {
                extractAbstraction(statement);

                // For Follows abstraction
                equallyNestedStatements.push_back(statement->getStatementNumber());

                // For Parents abstraction
                if (parentStatementNumber != -1) {
                    insertToAbstractionMapAddToFront(to_string(statement->getStatementNumber()), to_string(parentStatementNumber), ParentsStorageMap);
                }

            }
            handleEquallyNestedStatements(equallyNestedStatements);


        }
        else {
            cerr << "Unsupported ASTNode type in ParentsFollowsAbstraction." << endl;
        }
    }

private:
    std::shared_ptr<map<string, vector<string>>> FollowsStorageMap;
    std::shared_ptr<map<string, vector<string>>> ParentsStorageMap;

    // Used for Follows abstraction
    void handleEquallyNestedStatements(vector<int> equallyNestedStatements) {
        //For each element within equallyNestedStatments, add the element and every other element after it to the FollowsStorageMap
        for (int i = 0; i < equallyNestedStatements.size(); i++) {
            for (int j = i + 1; j < equallyNestedStatements.size(); j++) {
                insertToAbstractionMap(to_string(equallyNestedStatements[i]), to_string(equallyNestedStatements[j]), FollowsStorageMap);
            }
        }
    }

};


class UsesAbstraction : public Abstractions {

};

class ModifiesAbstraction : public Abstractions {

};