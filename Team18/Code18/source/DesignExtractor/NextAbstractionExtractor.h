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
    NextAbstractionExtractor() {
        this->ifStorageMap = std::make_shared<map<string, vector<string>>>();
    }

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
        
        // Connect the first statement number to the while statement number
        string firstStatementNumber = to_string(statements.front()->getStatementNumber());
        insertToAbstractionMap(whileStatementNumber, firstStatementNumber);
        
        string lastStatment = traverse(statements);
        insertToAbstractionMap(lastStatment, whileStatementNumber);
    }

    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();

        string ifStatementNumber = to_string(ifNode->getStatementNumber());
        // Get first value of ifStatements
        string firstIfStatementNumber = to_string(ifStatements.front()->getStatementNumber());
        // Get first value of elseStatements
        string firstElseStatementNumber = to_string(elseStatements.front()->getStatementNumber());

        insertToAbstractionMap(ifStatementNumber, firstIfStatementNumber);
        insertToAbstractionMap(ifStatementNumber, firstElseStatementNumber);

        string endIfStatementNumber = traverse(ifStatements);
        string endElseStatementNumber = traverse(elseStatements);

        insertToIfStorageMap(ifStatementNumber, endIfStatementNumber, endElseStatementNumber);
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        extractDesigns(astNode);
        processIfLines();
    }

private:
    std::shared_ptr<map<string, vector<string>>> ifStorageMap;

    string traverse(std::vector<std::shared_ptr<StatementNode>> statements) {
        unordered_set<string> prevStatementNumbers = {};
        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            if (prevStatementNumbers.size() > 0) {
                for (const auto& prevStatement : prevStatementNumbers) {
                    insertToAbstractionMap(prevStatement, statementNumber);
                }
                prevStatementNumbers.clear();
            }
            prevStatementNumbers.insert(statementNumber);
            extractDesigns(statement);
            // if its the last statement, return the statement number to caller
            if (statement == statements.back() && statement->getName() != "if") {
                insertKeyToAbstractionMap(statementNumber);
                return statementNumber;
            }
            
        }
        return "Something went wrong with traverse";
    }


    // Replace the values of the keys for IF statemetns in the abstraction map with the values in the ifStorageMap
    void processIfLines() {
        for (const auto& [key, value] : *ifStorageMap) {
            if (AbstractionStorageMap->find(key) != AbstractionStorageMap->end()) {
                AbstractionStorageMap->at(key) = value;
            }
        }
    }

    void insertKeyToAbstractionMap(string key) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
    }

    void insertToIfStorageMap(string key, string lastIfLineNumber, string lastElseLineNumber) {
        if (this->ifStorageMap->find(key) == this->ifStorageMap->end()) {
            this->ifStorageMap->insert({ key, vector<string>(lastIfLineNumber, lastElseLineNumber) });
        }
    }



};