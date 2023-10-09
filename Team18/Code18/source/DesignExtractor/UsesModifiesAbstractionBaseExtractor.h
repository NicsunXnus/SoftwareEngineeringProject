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

        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();
            insertIntoMap(statementNumber, whileNode->getStatementNumber(), ifWhileNestedStatementsMap);
            extractDesigns(statement);
        }
    }

    // Overriden method to store statement numbers of the child of if node
    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        preProcessIfNode(ifNode);
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
        std::vector<std::shared_ptr<StatementNode>> statements;

        statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
        statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        
        for (const auto& statement : statements) {
            int statementNumber = statement->getStatementNumber();
            insertIntoMap(statementNumber, ifNode->getStatementNumber(), ifWhileNestedStatementsMap);
            extractDesigns(statement);
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

    // This method checks with the ifWhileNestedStatementsMap to see if any of the keys can be found in the values
    // if it does, add the vector of values to that key's vector of values. This is repeated until no more changes are made
    void preProcessNestedIfWhileStatements() {
        for (auto it = ifWhileNestedStatementsMap->begin(); it != ifWhileNestedStatementsMap->end(); ++it) {
            const string& key = it->first;          // Get the key
            vector<string>& values = it->second;    // Get the vector of values associated with the key
            
            // Check if any of the values can be found in other keys
            for (auto& value : values) {
                // Iterate over the map entries again to look for matches
                for (auto it2 = ifWhileNestedStatementsMap->begin(); it2 != ifWhileNestedStatementsMap->end(); ++it2) {
                    if (it2 != it) {  // Skip the current key-value pair
                        vector<string>& otherValues = it2->second;  // Get the vector of values of the other key
                        
                        // Check if the value is in the other key's vector
                        auto found = find(otherValues.begin(), otherValues.end(), value);
                        
                        if (found != otherValues.end()) {
                            // Add the entire vector of values to the key's values
                            values.insert(values.end(), otherValues.begin(), otherValues.end());
                            // Remove the other key's entry since its values are merged
                            ifWhileNestedStatementsMap->erase(it2);
                            // Restart the iteration to recheck with the newly added values
                            it = ifWhileNestedStatementsMap->begin();
                            break;
                        }
                    }
                }
            }
        }
    }

    // This method checks through the values of AbstractionStorageMap to see if any of the values 
    // can be found as the key in the ifWhileNestedStatementsMap, if it does, add the vector 
    // of values of that key
    void processNestedIfWhileStatements() {
        preProcessNestedIfWhileStatements();
        for (const auto& [key, values] : *this->AbstractionStorageMap) {
            for (const auto& value : values) {
                if (this->ifWhileNestedStatementsMap->find(value) != this->ifWhileNestedStatementsMap->end()) {
                    vector<string> nestedStatements = this->ifWhileNestedStatementsMap->at(value);
                    for (const auto& nestedStatement : nestedStatements) {
                        insertToAbstractionMap(key, nestedStatement);
                    }
                }
            }
        }
        
    }
};