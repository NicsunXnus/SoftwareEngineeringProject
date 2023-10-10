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
        this->UsesModifiesCallsMap = std::make_shared<map<string, vector<string>>>();
        this->procedureCallLinesMap = std::make_shared<map<string, vector<string>>>();
        this->lineNumberToProcedureNameExtractor = make_shared<LineNumberToProcedureNameExtractor>();
        this->ifWhileNestedStatementsMap = std::make_shared<map<string, vector<string>>>();
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
        // std::vector<string> nestedStatements = vector<string>();

        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            // nestedStatements.push_back(statementNumber);
            extractDesigns(statement);
            
            // insertIntoMap(statementNumber, to_string(whileNode->getStatementNumber()), ifWhileNestedStatementsMap);

            // if any of the nestedStatement values can be found in the AbstractionStorageMap, add the ifNode statement number to the AbstractionStorageMap
            // for (const auto& nestedStatement : nestedStatements) {
            //     for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            //         if (std::find(values.begin(), values.end(), nestedStatement) != values.end()) {
            //             string statementNumber = to_string(whileNode->getStatementNumber());
            //             addStatementNumberAndProcedureName(variable, statementNumber);
            //         }
            //     }
            // }
            
        }
    }

    // Overriden method to store statement numbers of the child of if node
    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        preProcessIfNode(ifNode);
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
        std::vector<std::shared_ptr<StatementNode>> statements;
        // std::vector<string> nestedStatements = vector<string>();

        statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
        statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        
        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            // nestedStatements.push_back(statementNumber);
            extractDesigns(statement);
            insertIntoMap(statementNumber, to_string(ifNode->getStatementNumber()), ifWhileNestedStatementsMap);
            
            // for (const auto& nestedStatement : nestedStatements) {
            //     for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            //         if (std::find(values.begin(), values.end(), nestedStatement) != values.end()) {
            //             string statementNumber = to_string(ifNode->getStatementNumber());
            //             addStatementNumberAndProcedureName(variable, statementNumber);
            //         }
            //     }
            // }
        }
    }

    void handleCall(std::shared_ptr<CallNode> callNode) override {
        string statementNumber = to_string(callNode->getStatementNumber());
        string procedureName = this->lineNumberToProcedureNameExtractor->getProcedureNameFromStatementNumber(statementNumber);
        string procedureCalledName = callNode->getProc()->getName();
        insertToAbstractionMap(procedureName, statementNumber);
        insertIntoMap(procedureCalledName, statementNumber, procedureCallLinesMap);
    }

    void extractAbstractions(shared_ptr<ASTNode> astNode) override {
        // Create CallsAbstractionExtractor to extract the Calls abstraction
        shared_ptr<CallsAbstractionExtractor> callsAbstractionExtractor = make_shared<CallsAbstractionExtractor>();
        callsAbstractionExtractor->extractAbstractions(astNode);
        shared_ptr<map<string, vector<string>>> callsAbstractionMap = callsAbstractionExtractor->getStorageMap();
        createUsesModifiesCallsMap(callsAbstractionMap);

        // Get LineNumberToProcedureNameExtractor from CallsAbstractionExtractor
        setLineNumberToProcedureNameExtractor(callsAbstractionExtractor->getLineNumberToProcedureNameExtractor());

        extractDesigns(astNode);
        processIndirectProcedureCalls();
        processNestedIfWhileStatements();
    }

protected:
    // This map is used to store procedure names as the key and a vector of the procedures names that calls it
    shared_ptr<map<string, vector<string>>> UsesModifiesCallsMap;
    // This map is used to store procedure names as the key and a vector of the statement numbers that calls it
    shared_ptr<map<string, vector<string>>> procedureCallLinesMap;
    // This map is used to storeif/while statement number as the key and nested statement numbers as the value 
    shared_ptr<map<string, vector<string>>> ifWhileNestedStatementsMap;
    // This extractor contains a map and a method to get the procedure name from a statement number
    shared_ptr<LineNumberToProcedureNameExtractor> lineNumberToProcedureNameExtractor; 
    

    virtual void preProcessWhileNode(std::shared_ptr<WhileNode> whileNode) {}
    virtual void preProcessIfNode(std::shared_ptr<IfNode> ifNode) {}

    // Set the LineNumberToProcedureNameExtractor
    void setLineNumberToProcedureNameExtractor(shared_ptr<LineNumberToProcedureNameExtractor> lineNumberToProcedureNameExtractor) {
        this->lineNumberToProcedureNameExtractor = lineNumberToProcedureNameExtractor;
    }


    void insertIntoMap(string key, string statementNumber, shared_ptr<map<string, vector<string>>> map) {
        // Insert to the map if the key is not found
        if (map->find(key) == map->end()) {
            map->insert({ key, vector<string>() });
        }
        // Insert to the vector if the statement number is not found
        if (std::find(map->at(key).begin(), map->at(key).end(), statementNumber) == map->at(key).end()) {
            map->at(key).push_back(statementNumber);
        }
    }
    
    // Create a new map with the values and keys swapped
    void createUsesModifiesCallsMap(shared_ptr<map<string, vector<string>>> callsMap) {
        for (const auto& [key, values] : *callsMap) {
            for (const auto& value : values) {
                insertIntoMap(value, key, UsesModifiesCallsMap);
            }
        }
    }
    
    // Add both statement number and parent procedure to the AbstractionStorageMap
    void addStatementNumberAndProcedureName(string variableName, string statementNumber) {
        string parentProcedure = this->lineNumberToProcedureNameExtractor->getProcedureNameFromStatementNumber(statementNumber);
        insertToAbstractionMap(variableName, statementNumber);
        insertToAbstractionMap(variableName, parentProcedure);
    }

    // for all keys in the AbstractionStorageMap, if a value within its vector can be found 
    // in keys of the UsesModifiesCallsMap, add the vector of key's from the UsesModifiesCallsMap to 
    // the vector of values in the AbstractionStorageMap
    void processIndirectProcedureCalls() {
        for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            std::vector<std::string> procedureNamesToBeAdded = vector<string>();
            for (const auto& value : values) {
                // If the value is a procedure name, add the vector of procedureNames from the UsesModifiesCallsMap
                if (this->UsesModifiesCallsMap->find(value) != this->UsesModifiesCallsMap->end()) {
                    procedureNamesToBeAdded.push_back(value);
                    procedureNamesToBeAdded.insert(procedureNamesToBeAdded.end(), this->UsesModifiesCallsMap->at(value).begin(), this->UsesModifiesCallsMap->at(value).end());
                }
            }
            for (const auto& procedureName : procedureNamesToBeAdded) {
                insertToAbstractionMap(variable, procedureName);   
                if (procedureCallLinesMap->find(procedureName) != procedureCallLinesMap->end()) {
                    for (const auto& statementNumber : procedureCallLinesMap->at(procedureName)) {
                        insertToAbstractionMap(variable, statementNumber);
                    }
                }
                
            }
        }
    }

    // This method checks through the values of AbstractionStorageMap to see if any of the values 
    // can be found as the key in the ifWhileNestedStatementsMap, if it does, add the vector 
    // of values of that key
    void processNestedIfWhileStatements() {
        // for all values in AbstractionStorageMap
        for (const auto& [variable, values] : *this->AbstractionStorageMap) {
            shared_ptr<vector<std::string>> statementNumbersToBeAdded = make_shared<vector<string>>();
            for (const auto& value : values) {
                nestedIfWhileHelper(value, statementNumbersToBeAdded)
            }
            // add statementNumbersToBeAdded to the vector of values in AbstractionStorageMap
            for (const auto& statementNumber : *statementNumbersToBeAdded) {
                insertToAbstractionMap(variable, statementNumber);
            }
        }
    }

    void nestedIfWhileHelper(string childStatementNumber, shared_ptr<vector<std::string>> statementNumbersToBeAdded) {
        if (this->ifWhileNestedStatementsMap->find(childStatementNumber) != this->ifWhileNestedStatementsMap->end()) {
            // get the vector of values of the childStatementNumber
            shared_ptr<vector<std::string>> nestedStatementNumbers = make_shared<vector<string>>(this->ifWhileNestedStatementsMap->at(childStatementNumber));
            for (const auto& nestedStatementNumber : *nestedStatementNumbers) {
                statementNumbersToBeAdded->push_back(nestedStatementNumber);
                // recursively call this method with the nestedStatementNumber as the childStatementNumber
                nestedIfWhileHelper(nestedStatementNumber, statementNumbersToBeAdded);
            }
        } 
    }
};