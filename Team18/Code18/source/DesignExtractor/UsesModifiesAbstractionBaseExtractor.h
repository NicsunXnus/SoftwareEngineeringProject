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
    // Constructor
    UsesModifiesAbstractionBaseExtractor() {
        this->AbstractionStorageMap = std::make_shared<map<string, vector<string>>>();
        this->procedureVariableStorageMap = std::make_shared<map<string, vector<shared_ptr<map<string, vector<string>>>>>>();
    }

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

    // void handleCall(std::shared_ptr<CallNode> callNode) override {
    //     string statementNumber = to_string(callNode->getStatementNumber());
    //     string procedureCalledName = callNode->getProc()->getName();
    //     insertIntoUsesModifiesCallsMap(procedureCalledName, statementNumber);
    // }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        // Create CallsAbstractionExtractor to extract the Calls abstraction
        shared_ptr<CallsAbstractionExtractor> callsAbstractionExtractor = make_shared<CallsAbstractionExtractor>();
        callsAbstractionExtractor->extractAbstractions(astNode);
        shared_ptr<map<string, vector<string>>> callsAbstractionMap = callsAbstractionExtractor->getStorageMap();
        shared_ptr<map<string, vector<string>>> usesModifiesCallsMap = createUsesModifiesCallsMap(callsAbstractionMap);
        setUsesModifiesCallsMap(usesModifiesCallsMap);
        extractDesigns(astNode);
        processIndirectProcedureCalls(usesModifiesCallsMap);
    }


protected:
    shared_ptr<map<string, vector<string>>> UsesModifiesCallsMap;
    shared_ptr<map<string, vector<shared_ptr<map<string, vector<string>>>>>> procedureVariableStorageMap;
    
    virtual void preProcessWhileNode(std::shared_ptr<WhileNode> whileNode) {}
    virtual void preProcessIfNode(std::shared_ptr<IfNode> ifNode) {}

    void setUsesModifiesCallsMap(shared_ptr<map<string, vector<string>>> usesModifiesCallsMap) {
        this->UsesModifiesCallsMap = usesModifiesCallsMap;
    }

    void insertIntoUsesModifiesCallsMap(string procedureName, string statementNumber) {
        if (this->UsesModifiesCallsMap->find(procedureName) == this->UsesModifiesCallsMap->end()) {
            this->UsesModifiesCallsMap->insert({ procedureName, vector<string>() });
        }
        // insert only if the statement number is not found in the vector
        if (std::find(this->UsesModifiesCallsMap->at(procedureName).begin(), this->UsesModifiesCallsMap->at(procedureName).end(), statementNumber) == this->UsesModifiesCallsMap->at(procedureName).end()) {
            this->UsesModifiesCallsMap->at(procedureName).push_back(statementNumber);
        }
    }
    
    // Create a new map with the values and keys swapped
    shared_ptr<map<string, vector<string>>> createUsesModifiesCallsMap(shared_ptr<map<string, vector<string>>> callsMap) {
        shared_ptr<map<string, vector<string>>> newUsesModifiesCallsMap = make_shared<map<string, vector<string>>>();
        // Loop through all values in the vector in the callsMap, add the value as a key and the key as a value to the newUsesModifiesCallsMap
        for (const auto& [key, values] : *callsMap) {
            for (const auto& value : values) {
                if (newUsesModifiesCallsMap->find(value) == newUsesModifiesCallsMap->end()) {
                    newUsesModifiesCallsMap->insert({ value, vector<string>() });
                }
                newUsesModifiesCallsMap->at(value).push_back(key);
            }
        }
        return newUsesModifiesCallsMap;
    }

    
    // Add both statement number and parent procedure to the AbstractionStorageMap
    void addStatementNumberAndProcedureName(string variableName, string statementNumber) {
        string parentProcedure = getProcedureNameFromStatementNumber(statementNumber);
        insertToAbstractionMap(variableName, statementNumber);
        insertToAbstractionMap(variableName, parentProcedure);
        insertToProcedureVariableStorageMap(parentProcedure, variableName, statementNumber);
    }

    void insertToProcedureVariableStorageMap(string parentProcedure, string variableName, string statementNumber) {
        if (this->procedureVariableStorageMap->find(parentProcedure) == this->procedureVariableStorageMap->end()) {
            this->procedureVariableStorageMap->insert({ parentProcedure, vector<shared_ptr<map<string, vector<string>>>>() });
        }
        // Insert new map if the variableName is not found in the procedure
        if (std::find_if(this->procedureVariableStorageMap->at(parentProcedure).begin(), this->procedureVariableStorageMap->at(parentProcedure).end(), [variableName](shared_ptr<map<string, vector<string>>> map) { return map->find(variableName) != map->end(); }) == this->procedureVariableStorageMap->at(parentProcedure).end()) {
            shared_ptr<map<string, vector<string>>> newMap = make_shared<map<string, vector<string>>>();
            newMap->insert({ variableName, vector<string>() });
            this->procedureVariableStorageMap->at(parentProcedure).push_back(newMap);
        }
        this->procedureVariableStorageMap->at(parentProcedure).back()->at(variableName).push_back(statementNumber);
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
            for (const auto& procedureName : procedureNames) {
                insertToAbstractionMap(variable, procedureName);
                // Add the statement numbers of all variables in the procedure to the vector of values
                for (const auto& [variableName, statementNumbers] : *this->procedureVariableStorageMap->at(procedureName).back()) {
                    for (const auto& statementNumber : statementNumbers) {
                        insertToAbstractionMap(variableName, statementNumber);
                    }
                }
            }
        }
    }
};