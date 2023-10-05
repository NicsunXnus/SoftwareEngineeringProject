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
 * This class is used to extract the Calls abstraction from the AST.
 */
class NextAbstractionExtractor : public AbstractionExtractor {
public:    
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    void handleCall(std::shared_ptr<CallNode> callNode) override {}
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleExpr(std::shared_ptr<ExprNode> exprNode) override {}
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) override {}

    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        traverse(statements);
    }

    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
        string whileStatementNumber = to_string(whileNode->getStatementNumber());
        traverse(statements);

        // Connect the last statement number to the while statement number
        string lastStatementNumber = to_string(statements.back()->getStatementNumber());
        insertToAbstractionMap(lastStatementNumber, whileStatementNumber);
    }

    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();

        traverse(ifStatements);
        traverse(elseStatements);
    }

private:
    void traverse(std::vector<std::shared_ptr<StatementNode>> statements) {
        unordered_set<string> prevStatementNumbers = {};
        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            if (prevStatementNumbers.size() > 0) {
                for (const auto& prevStatement : prevStatementNumbers) {
                    insertToAbstractionMap(prevStatement, statementNumber);
                }
                prevStatementNumbers.clear();
            }
            if (statement->getName() == "if") {
                prevStatementNumbers = getIfElseLastStatementNumbers(statement);
                extractDesigns(statement);
            } else {
                prevStatementNumbers.insert(statementNumber);
                extractDesigns(statement);
            }
            // if its the last statement, 
            if (statement == statements.back()) {
                for (const auto& prevStatement : prevStatementNumbers) {
                    insertKeyToAbstractionMap(prevStatement);
                }
            }
        }
    }

    void insertKeyToAbstractionMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
    }

    unordered_set<string> getIfElseLastStatementNumbers(std::shared_ptr<ASTNode> ifNode) {
        unordered_set<string> prevStatementNumbers = {};
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
        // Get last value of ifStatements
        string lastIfStatementNumber = to_string(ifStatements.back()->getStatementNumber());
        // Get last value of elseStatements
        string lastElseStatementNumber = to_string(elseStatements.back()->getStatementNumber());

        prevStatementNumbers.insert(lastIfStatementNumber);
        prevStatementNumbers.insert(lastElseStatementNumber);

        return prevStatementNumbers;
    }



};