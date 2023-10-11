#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#include "../AST/ASTNode.h"
#include "AbstractionExtractor.h"

/**
 */
class NextAbstractionExtractor : public AbstractionExtractor {
public:    
    // Constructor
    // NextAbstractionExtractor() {
    //     this->ifStorageMap = std::make_shared<map<string, vector<string>>>();
    // }

    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    void handleCall(std::shared_ptr<CallNode> callNode) override {}
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleExpr(std::shared_ptr<ExprNode> exprNode) override {}
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) override {}
    void handleIf(std::shared_ptr<IfNode> ifNode) override {}
    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {}

    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        traverse(statements);
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        extractDesigns(astNode);
    }

private:
    unordered_set<string> traverse(std::vector<std::shared_ptr<StatementNode>> statements) {
        unordered_set<string> lastStatements;

        for (const auto &statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());

            if (lastStatements.size() > 0) {
                for (const auto &prevStatementNumber : lastStatements) {
                    insertToAbstractionMap(prevStatementNumber, statementNumber);
                }
                lastStatements.clear();
            }

            if (statement->getName() != "if" && statement->getName() != "while") {
                lastStatements.insert(statementNumber);
            }

            if (statement->getName() == "while") {
                string whileStatementNumber = statementNumber; // Store the while statement number
                string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());
                insertToAbstractionMap(whileStatementNumber, firstStatementNumber);

                unordered_set<string> whileStatements;
                
                // Traverse the statements within the while loop
                unordered_set<string> lastInLoopSet = traverse(statement->getStatements());
                
                // Make the last statements of the loop reference back to the while loop
                for (const auto &lastInLoop : lastInLoopSet) {
                    insertToAbstractionMap(lastInLoop, whileStatementNumber);
                }
        
                lastStatements.insert(whileStatementNumber);
            } else if (statement->getName() == "if") {
                string firstStatementNumber = to_string(statement->getStatements().front()->getStatementNumber());
                insertToAbstractionMap(statementNumber, firstStatementNumber);

                string firstStatementNumberElse = to_string(statement->getElseStatements().front()->getStatementNumber());
                insertToAbstractionMap(statementNumber, firstStatementNumberElse);

                unordered_set<string> lastInIfSet = traverse(statement->getStatements());
                unordered_set<string> lastInElseSet = traverse(statement->getElseStatements());

                lastStatements.insert(lastInIfSet.begin(), lastInIfSet.end());
                lastStatements.insert(lastInElseSet.begin(), lastInElseSet.end());
            } else if (statement == statements.back()) {
                insertKeyToAbstractionMap(statementNumber);
                lastStatements.insert(statementNumber);
            }
        }

        return lastStatements;
    }

    void insertKeyToAbstractionMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
    }
};