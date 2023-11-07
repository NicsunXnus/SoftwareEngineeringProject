#include "UsesModifiesExtractor.h"

void UsesModifiesExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) {
    this->callsExtractor->extractAbstractions(processedProgram);
    createCallsProcedureParentProcedureMap(this->callsExtractor->getStorageStarMap());
    extract(processedProgram);
    processIndirectProcedureCalls();
    processNestedIfWhileStatements();
}

string UsesModifiesExtractor::getProcedureNameFromStatementNumber(string lineNumber) {
    return this->callsExtractor->getProcedureNameFromStatementNumber(lineNumber);
}

void UsesModifiesExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    vector<shared_ptr<ProcessedProcedure>> procedures = processedProgram->getAllProcedures();
    for (shared_ptr<ProcessedProcedure> procedure : procedures) {
        procedure->accept(shared_from_this());
    }
}

void UsesModifiesExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList, string parentConditionalStatementNumber) {
    for (auto& stmt : processedStmtList->getStmts()) {
        string currentStatementNumber = to_string(stmt->getStatementNumber());
        this->insertToIfWhileNestedStatementsMap(currentStatementNumber, parentConditionalStatementNumber);
        stmt->accept(shared_from_this());
    }
}

void UsesModifiesExtractor::extract(shared_ptr<ProcessedCallStmt> processedCallStmt) {
    string statementNumber = to_string(processedCallStmt->getStatementNumber());
    string procedureCalledName = processedCallStmt->getProcedureName()->getName();
    this->insertIntoProcedureCallLinesMap(procedureCalledName, statementNumber);
}

void UsesModifiesExtractor::processIndirectProcedureCalls() {
    for (const auto& [variable, values] : *this->AbstractionStorageMap) {
        vector<string> procedureNamesToBeAdded = vector<string>();
        for (const auto& value : values) {
            // If the value is a procedure name, add the vector of procedureNames from the callsProcedureParentProcedureMap
            if (this->callsProcedureParentProcedureMap->find(value) != this->callsProcedureParentProcedureMap->end()) {
                procedureNamesToBeAdded.push_back(value);
                procedureNamesToBeAdded.insert(procedureNamesToBeAdded.end(), this->callsProcedureParentProcedureMap->at(value).begin(), this->callsProcedureParentProcedureMap->at(value).end());
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

void UsesModifiesExtractor::addStatementNumberAndProcedureName(string variableName, string statementNumber) {
    string parentProcedure = this->callsExtractor->getProcedureNameFromStatementNumber(statementNumber);
    insertToAbstractionMap(variableName, statementNumber);
    insertToAbstractionMap(variableName, parentProcedure);
}

void UsesModifiesExtractor::createCallsProcedureParentProcedureMap(shared_ptr<map<string, unordered_set<string>>> callsMap) {
    for (const auto& [key, values] : *callsMap) {
        for (const auto& value : values) {
            insertIntoMap(value, key, callsProcedureParentProcedureMap);
        }
    }
}

void UsesModifiesExtractor::processNestedIfWhileStatements() {
    // for all values in AbstractionStorageMap
    for (const auto& [variable, values] : *this->AbstractionStorageMap) {
        shared_ptr<unordered_set<string>> statementNumbersToBeAdded = make_shared<unordered_set<string>>();
        for (const auto& value : values) {
            nestedIfWhileHelper(value, statementNumbersToBeAdded);
        }
        // add statementNumbersToBeAdded to the vector of values in AbstractionStorageMap
        for (const auto& statementNumber : *statementNumbersToBeAdded) {
            insertToAbstractionMap(variable, statementNumber);
        }
    }
}

void UsesModifiesExtractor::nestedIfWhileHelper(string childStatementNumber, shared_ptr<unordered_set<string>> statementNumbersToBeAdded) {
        if (this->ifWhileNestedStatementsMap->find(childStatementNumber) != this->ifWhileNestedStatementsMap->end()) {
            // get the vector of values of the childStatementNumber
            shared_ptr<unordered_set<string>> nestedStatementNumbers = make_shared<unordered_set<string>>(this->ifWhileNestedStatementsMap->at(childStatementNumber));
            for (const auto& nestedStatementNumber : *nestedStatementNumbers) {
                // add the nestedStatementNumber to the vector of values in the AbstractionStorageMap
                statementNumbersToBeAdded->insert(nestedStatementNumber);
                // recursively call this method with the nestedStatementNumber as the childStatementNumber
                nestedIfWhileHelper(nestedStatementNumber, statementNumbersToBeAdded);
            }
        } 
    }