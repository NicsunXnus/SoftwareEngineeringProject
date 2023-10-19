#include "ProcedureLineNumberExtractor.h"

using namespace std;

void ProcedureLineNumberExtractor::insertToProcedureStatementStorageMap(string procedureName, string statementNumber) {
    // Insert to the map if the procedureName is not found
    if (this->procedureStatementStorageMap->find(procedureName) == this->procedureStatementStorageMap->end()) {
        this->procedureStatementStorageMap->insert({ procedureName, pair<string, string>("", "")});
    }
    // Insert to the first value if the statement number is smaller than the current value or if it doesn't exist
    if (this->procedureStatementStorageMap->at(procedureName).first == "" || stoi(statementNumber) < stoi(this->procedureStatementStorageMap->at(procedureName).first)) {
        this->procedureStatementStorageMap->at(procedureName).first = statementNumber;
    }
    // Insert to the second value if the statement number is larger than the current value
    if (this->procedureStatementStorageMap->at(procedureName).second == "" || stoi(statementNumber) > stoi(this->procedureStatementStorageMap->at(procedureName).second)) {
        this->procedureStatementStorageMap->at(procedureName).second = statementNumber;
    }
}

string ProcedureLineNumberExtractor::getProcedureNameFromStatementNumber(string statementNumber) {
    for (const auto& [procedureName, values] : *this->procedureStatementStorageMap) {
        if (stoi(statementNumber) >= stoi(values.first) && stoi(statementNumber) <= stoi(values.second)) {
            return procedureName;
        }
    }
    std::cout << "Error: Statement number not found in procedureStatementStorageMap";
}

void ProcedureLineNumberExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    std::vector<std::shared_ptr<ProcessedProcedure>> procedures = processedProgram->getAllProcedures();
    for (std::shared_ptr<ProcessedProcedure> procedure : procedures) {
        procedure->accept(shared_from_this());
    }
}

void ProcedureLineNumberExtractor::extract(shared_ptr<ProcessedProcedure> processedProcedure) {
    std::string_view procName = processedProcedure->getProcName();
    std::string procedureName(procName);
    std::shared_ptr<ProcessedStmtList> statementList = processedProcedure->getStmts();
    statementList->accept(shared_from_this(), procedureName);
}

void ProcedureLineNumberExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList, string procedureName) {
    for (auto& stmt : processedStmtList->getStmts()) {
        this->insertToProcedureStatementStorageMap(procedureName, std::to_string(stmt->getStatementNumber()));
        stmt->accept(shared_from_this(), procedureName);
    }
}

void ProcedureLineNumberExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile, string procedureName) {
    std::string stmtNumber = std::to_string(processedWhile->getStatementNumber());
    this->insertToProcedureStatementStorageMap(procedureName, stmtNumber);

    processedWhile->getWhileBlock()->accept(shared_from_this(), procedureName);
}

void ProcedureLineNumberExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf, string procedureName) {
    std::string stmtNumber = std::to_string(processedIf->getStatementNumber());
    this->insertToProcedureStatementStorageMap(procedureName, stmtNumber);
    
    processedIf->getThenBlock()->accept(shared_from_this(), procedureName);
    processedIf->getElseBlock()->accept(shared_from_this(), procedureName);
}