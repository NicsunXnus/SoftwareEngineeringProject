#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "ParentsFollowsAbstractionBaseExtractor.h"

class FollowsAbstractionExtractor : public ParentsFollowsAbstractionBaseExtractor {
public:
    void handleNode(shared_ptr<ProcedureNode> procedureNode, shared_ptr<IfNode> ifNode, shared_ptr<WhileNode> whileNode) override {
        vector<shared_ptr<StatementNode>> statements;
        if (ifNode) {
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            // Process "if" statements
            for (const auto& statement : ifStatements) {
                extractDesigns(statement);
            }
            // Process "else" statements separately
            for (const auto& statement : elseStatements) {
                extractDesigns(statement);
            }
            handleEquallyNestedStatements(ifStatements);
            handleEquallyNestedStatements(elseStatements);
            return;
        }
        else if (procedureNode) {
            statements = procedureNode->getStatements();
        }
        else if (whileNode) {
            statements = whileNode->getStatements();
        }
        // Extract the line numbers of the statements and insert them into the FollowsStorageMap
        vector<int> equallyNestedStatements = vector<int>();
        for (const auto& statement : statements) {
            extractDesigns(statement);
            equallyNestedStatements.push_back(statement->getStatementNumber());
        }
        handleEquallyNestedStatements(equallyNestedStatements);
    }

private:
    void handleEquallyNestedStatements(vector<int> equallyNestedStatements) {
        //For each element within equallyNestedStatments, add the element and every other element after it to the FollowsStorageMap
        for (int i = 0; i < equallyNestedStatements.size(); i++) {
            for (int j = i + 1; j < equallyNestedStatements.size(); j++) {
                insertToMap(to_string(equallyNestedStatements[i]), to_string(equallyNestedStatements[j]));
            }
        }
    }
};
