#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"

/*
 * This class is the base class for the Uses and Modifies Abstraction extractors
 * It is used to provide default implementations and common methods for the two extractors
 * Override the relevant methods to implement the different abstractions
 */

class UsesModifiesAbstractionBaseExtractor : public Extractor {
public:
    //Constructor
    UsesModifiesAbstractionBaseExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
        this->procedureStatementStorageMap = std::make_shared<map<string, vector<string>>>();
    }

    // Gets the map of the abstraction
    std::shared_ptr<map<string, vector<string>>> getStorageMap() {
        return this->AbstractionStorageMap;
    }

    // Method to extract the uses abstraction
    void extractAbstractions(shared_ptr<ASTNode> astNode) {
        extractDesigns(astNode);
        reduceProcedureStatementStorageMap();
        addProcedureNames();
    }

    // Overriden method to store procedure names on top of extraction of designs
    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        for (const auto& statement : statements) {
            insertToProcedureStatementStorageMap(procedureNode->getName(), to_string(statement->getStatementNumber()));
            extractDesigns(statement);
        }
    }

    // Overriden method to store variable name (no default implmentation)
    void handleVariable(std::shared_ptr<VariableNode> variableNode) override {
        insertToAbstractionMap(variableNode->getValue(), to_string(variableNode->getStatementNumber()));
    }

    // Overriden method to store statement numbers of the child of while node
    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        preProcessWhileNode(whileNode);
        std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
        std::vector<int> nestedStatements = vector<int>();
        string procedureName = getProcedureName(whileNode->getStatementNumber());

        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();
            insertToProcedureStatementStorageMap(procedureName, to_string(statementNumber));

            // Add the statement number to the vector
            nestedStatements.push_back(statementNumber);

            extractDesigns(statement);
        }
        // if any of the nestedStatement values can be found in the AbstractionStorageMap, add the whileNode statement number to the AbstractionStorageMap
        for (const auto& nestedStatement : nestedStatements) {
            for (const auto& [variable, values] : *this->AbstractionStorageMap) {
                if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                    insertToAbstractionMap(variable, to_string(whileNode->getStatementNumber()));
                }
            }
        }
    }

    // Overriden method to store statement numbers of the child of if node
    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        preProcessIfNode(ifNode);
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
        std::vector<std::shared_ptr<StatementNode>> statements;
        std::vector<int> nestedStatements = vector<int>();
        string procedureName = getProcedureName(ifNode->getStatementNumber());

        statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
        statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        
        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();
            insertToProcedureStatementStorageMap(procedureName, to_string(statementNumber));

            // Add the statement number to the vector
            nestedStatements.push_back(statementNumber);
            
            extractDesigns(statement);
        }
        // if any of the nestedStatement values can be found in the AbstractionStorageMap, add the ifNode statement number to the AbstractionStorageMap
        for (const auto& nestedStatement : nestedStatements) {
            for (const auto& [variable, values] : *this->AbstractionStorageMap) {
                if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                    insertToAbstractionMap(variable, to_string(ifNode->getStatementNumber()));
                }
            }
        }
    }

protected:
    std::shared_ptr<map<string, vector<string>>> AbstractionStorageMap;
    std::shared_ptr<map<string, vector<string>>> procedureStatementStorageMap;

    virtual void preProcessWhileNode(std::shared_ptr<WhileNode> whileNode) {}
    virtual void preProcessIfNode(std::shared_ptr<IfNode> ifNode) {}

    // Inserts a key and value into the map
    void insertToAbstractionMap(string key, string value) {
        if (this->AbstractionStorageMap->find(key) == this->AbstractionStorageMap->end()) {
            this->AbstractionStorageMap->insert({ key, vector<string>() });
        }
        this->AbstractionStorageMap->at(key).push_back(value);
    }

    // insert to procedureStatementStorageMap
    void insertToProcedureStatementStorageMap(string procedureName, string statementNumber) {
        if (this->procedureStatementStorageMap->find(procedureName) == this->procedureStatementStorageMap->end()) {
            this->procedureStatementStorageMap->insert({procedureName, vector<string>()});
        }
        this->procedureStatementStorageMap->at(procedureName).push_back(statementNumber);
    }

     // Method to reduce the procedureStatementStorageMap to two values (min and max)
    void reduceProcedureStatementStorageMap() {
        for (const auto& [procedureName, statementNumbers] : *this->procedureStatementStorageMap) {
            int min = stoi(statementNumbers[0]);
            int max = stoi(statementNumbers[0]);
            for (const auto& statementNumber : statementNumbers) {
                if (stoi(statementNumber) < min) {
                    min = stoi(statementNumber);
                }
                if (stoi(statementNumber) > max) {
                    max = stoi(statementNumber);
                }
            }
            this->procedureStatementStorageMap->at(procedureName).clear();
            this->procedureStatementStorageMap->at(procedureName).push_back(to_string(min));
            this->procedureStatementStorageMap->at(procedureName).push_back(to_string(max));
        }
    }

    // Get the procedure name of a statement number
    string getProcedureName(int statementNumber) {
        for (const auto& [procedureName, statementNumbers] : *this->procedureStatementStorageMap) {
            if (std::find(statementNumbers.begin(), statementNumbers.end(), to_string(statementNumber)) != statementNumbers.end()) {
                return procedureName;
            }
        }
    }

    // Method to add procedure names to the abstraction map from reduced ProcedureStatementStorageMap
    void addProcedureNames() {
        for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            for (const auto& [procedureName, statementNumbers] : *this->procedureStatementStorageMap) {
                if (stoi(values[0]) >= stoi(statementNumbers[0]) && stoi(values[0]) <= stoi(statementNumbers[1])) {
                    this->AbstractionStorageMap->at(variable).push_back(procedureName);
                }
            }
        }        
    }


    
};