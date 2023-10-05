#pragma once
#include<stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"
#include "AbstractionExtractor.h"
#include "CallsAbstractionExtractor.h"

/*
 * This class is the base class for the Uses and Modifies Abstraction extractors
 * It is used to provide default implementations and common methods for the two extractors
 * Override the relevant methods to implement the different abstractions
 */

class UsesModifiesAbstractionBaseExtractor : public AbstractionExtractor {
public:
    // Overriden method to store variable name (no default implmentation)
    void handleVariable(std::shared_ptr<VariableNode> variableNode) override {
        string variableName = variableNode->getValue();
        string statementNumber = to_string(variableNode->getStatementNumber());
        addStatementNumberAndProcedureName(variableName, statementNumber);
    }

    // Overriden method to store statement numbers of the child of while node
    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        preProcessWhileNode(whileNode);
        std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
        std::vector<int> nestedStatements = vector<int>();

        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();

            // Add the statement number to the vector
            nestedStatements.push_back(statementNumber);

            extractDesigns(statement);
        }
        // if any of the nestedStatement values can be found in the AbstractionStorageMap, add the whileNode statement number to the AbstractionStorageMap
        for (const auto& nestedStatement : nestedStatements) {
            for (const auto& [variable, values] : *this->AbstractionStorageMap) {
                if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                    string statementNumber = to_string(whileNode->getStatementNumber());
                    addStatementNumberAndProcedureName(variable, statementNumber);
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

        statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
        statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        
        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();
            
            // Add the statement number to the vector
            nestedStatements.push_back(statementNumber);
            
            extractDesigns(statement);
        }
        // if any of the nestedStatement values can be found in the AbstractionStorageMap, add the ifNode statement number to the AbstractionStorageMap
        for (const auto& nestedStatement : nestedStatements) {
            for (const auto& [variable, values] : *this->AbstractionStorageMap) {
                if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                    string statementNumber = to_string(ifNode->getStatementNumber());
                    addStatementNumberAndProcedureName(variable, statementNumber);
                }
            }
        }
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        // Create CallsAbstractionExtractor to extract the Calls abstraction
        shared_ptr<CallsAbstractionExtractor> callsAbstractionExtractor = make_shared<CallsAbstractionExtractor>();
        callsAbstractionExtractor->extractAbstractions(astNode);
        shared_ptr<map<string, vector<string>>> callsAbstractionMap = callsAbstractionExtractor->getStorageMap();

        extractDesigns(astNode);
        processIndirectProcedureCalls(callsAbstractionMap);
    }


protected:
    virtual void preProcessWhileNode(std::shared_ptr<WhileNode> whileNode) {}
    virtual void preProcessIfNode(std::shared_ptr<IfNode> ifNode) {}
    
    // Add both statement number and parent procedure to the AbstractionStorageMap
    void addStatementNumberAndProcedureName(string variableName, string statementNumber) {
        string parentProcedure = getProcedureNameFromStatementNumber(statementNumber);
        insertToAbstractionMap(variableName, statementNumber);
        insertToAbstractionMap(variableName, parentProcedure);
    }
    
    // for all keys in the AbstractionStorageMap, if a value within its vector can be found 
    // in the callsAbstractionMap, add the vector of values from the callsAbstractionMap to 
    // the vector of values in the AbstractionStorageMap
    void processIndirectProcedureCalls(shared_ptr<map<string, vector<string>>> callsAbstractionMap) {
        for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            std::vector<std::string> procedureNames = vector<string>();
            for (const auto& value : values) {
                if (callsAbstractionMap->find(value) != callsAbstractionMap->end()) {
                    // Add to the vector of procedureNames 
                    procedureNames.insert(procedureNames.end(), callsAbstractionMap->at(value).begin(), callsAbstractionMap->at(value).end());
                }
            }
            // Add procedureNames to values
            this->AbstractionStorageMap->at(variable).insert(this->AbstractionStorageMap->at(variable).end(), procedureNames.begin(), procedureNames.end());
        }
    }
};