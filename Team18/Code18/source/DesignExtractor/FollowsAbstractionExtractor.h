#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "ParentsFollowsAbstractionBaseExtractor.h"

/**
 * This class is used to extract the Follows abstraction from the AST.
 */
class FollowsAbstractionExtractor : public ParentsFollowsAbstractionBaseExtractor {
public:
    // Overriden method to handle the different types of nodes, its use is found in the ParentsFollowsAbstractionBaseExtractor base class
    void handleNode(shared_ptr<ProcedureNode> procedureNode, shared_ptr<IfNode> ifNode, shared_ptr<WhileNode> whileNode) override {
        vector<shared_ptr<StatementNode>> statements;
        if (ifNode) {
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            vector<int> equallyNestedStatementsIf = vector<int>();
            vector<int> equallyNestedStatementsElse = vector<int>();

            extractAndConvertToStatementNumbers(ifStatements, equallyNestedStatementsIf);
            extractAndConvertToStatementNumbers(elseStatements, equallyNestedStatementsElse);

            handleEquallyNestedStatements(equallyNestedStatementsIf);
            handleEquallyNestedStatements(equallyNestedStatementsElse);
            return;
        }
        else if (procedureNode) {
            statements = procedureNode->getStatements();
        }
        else if (whileNode) {
            statements = whileNode->getStatements();
        }
        vector<int> equallyNestedStatements = vector<int>();
        extractAndConvertToStatementNumbers(statements, equallyNestedStatements);
        handleEquallyNestedStatements(equallyNestedStatements);
    }

private:
    // Method to convert a vector of statements to a vector of statement numbers
    void extractAndConvertToStatementNumbers(vector<shared_ptr<StatementNode>> statements, vector<int> &statementNumbers) {
        for (const auto& statement : statements) {
            extractDesigns(statement);
            statementNumbers.push_back(statement->getStatementNumber());
        }
    }

    // Method to use the nested statments to build the FollowsStorageMap
    void handleEquallyNestedStatements(vector<int> equallyNestedStatements) {
        //For each element within equallyNestedStatments, add the element and every other element after it to the FollowsStorageMap
        for (int i = 0; i < equallyNestedStatements.size(); i++) {
            for (int j = i + 1; j < equallyNestedStatements.size(); j++) {
                insertToAbstractionMap(to_string(equallyNestedStatements[i]), to_string(equallyNestedStatements[j]));
            }
        }
    }
};
