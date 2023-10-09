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

/**
 */
class LineNumberToProcedureNameExtractor : public Extractor {
public:
    // Constructor
    LineNumberToProcedureNameExtractor() {
        this->procedureStatementStorageMap = std::make_shared<map<string, pair<string, string>>>();
    }

    // Override methods to avoid unnecessary processing
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}

    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        string procedureName = procedureNode->getName();
        // Get statements from procedureNode
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        // Insert statements to procedureStatementStorageMap
        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            insertToProcedureStatementStorageMap(procedureName, statementNumber);
            if (statement == statements.back() && (statement->getName() == "if" || statement->getName() == "while")) {
                traverseIfWhile(statement, procedureName);
            }   
        }
    }

    // method to get procedure name from statement number
    string getProcedureNameFromStatementNumber(string statementNumber) {
        for (const auto& [procedureName, values] : *this->procedureStatementStorageMap) {
            if (stoi(statementNumber) >= stoi(values.first) && stoi(statementNumber) <= stoi(values.second)) {
                return procedureName;
            }
        }
        // return "SOMETHING ISN'T WORKING";
        std::cout << "Error: Statement number not found in procedureStatementStorageMap";
        std::cerr << "Error: Statement number not found in procedureStatementStorageMap" << std::endl;
        }

    // Get procedureStatementStorageMap
    std::shared_ptr<map<string, pair<string, string>>> getProcedureStatementStorageMap() {
        return this->procedureStatementStorageMap;
    }
    
private:
    // map to store procedurename to the min and max values of the statement numbers
    std::shared_ptr<map<string, pair<string, string>>> procedureStatementStorageMap;

    // traverse if and while statements
    void traverseIfWhile(shared_ptr<StatementNode> statement, string procedureName) {
        std::vector<std::shared_ptr<StatementNode>> statements;
        if (statement->getName() == "if") {
            shared_ptr<IfNode> ifNode = static_pointer_cast<IfNode>(statement);
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
            statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        }
        else if (statement->getName() == "while") {
            shared_ptr<WhileNode> whileNode = static_pointer_cast<WhileNode>(statement);
            std::vector<std::shared_ptr<StatementNode>> whileStatements = whileNode->getStatements();
            statements.insert(statements.end(), whileStatements.begin(), whileStatements.end());
        }

        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            insertToProcedureStatementStorageMap(procedureName, statementNumber);
            if (statement == statements.back() && (statement->getName() == "if" || statement->getName() == "while")) {
                traverseIfWhile(statement, procedureName);
            }
        }
    }
    
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
};
