#include "CallsExtractor.h"
#include "../SimpleProcessor/ProcessedCallStmt.h"

void CallsExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    this->procedureLineNumberExtractor->extract(processedProgram);
    for (auto procedure : processedProgram->getAllProcedures()) {
        procedure->accept(shared_from_this());
    }
}

void CallsExtractor::extract(shared_ptr<ProcessedCallStmt> processedCall) {
    std::string statementNumber = std::to_string(processedCall->getStatementNumber());
    std::string procedureName = this->getProcedureNameFromStatementNumber(statementNumber);
    std::string calledProcedure = processedCall->getProcedureName()->getName();
    this->insertToAbstractionMap(procedureName, calledProcedure);
}

string CallsExtractor::getProcedureNameFromStatementNumber(string lineNumber) {
    return this->procedureLineNumberExtractor->getProcedureNameFromStatementNumber(lineNumber);
}
