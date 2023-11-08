#include "CallsExtractor.h"
#include "../SimpleProcessor/ProcessedCallStmt.h"

void CallsExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    this->procedureLineNumberExtractor->extract(processedProgram);
    for (auto procedure : processedProgram->getAllProcedures()) {
        procedure->accept(shared_from_this());
    }
}

void CallsExtractor::extract(shared_ptr<ProcessedCallStmt> processedCall) {
    string statementNumber = to_string(processedCall->getStatementNumber());
    string procedureName = this->getProcedureNameFromStatementNumber(statementNumber);
    string calledProcedure = processedCall->getProcedureName()->getName();
    this->insertToAbstractionMap(procedureName, calledProcedure);
}

string CallsExtractor::getProcedureNameFromStatementNumber(string lineNumber) {
    return this->procedureLineNumberExtractor->getProcedureNameFromStatementNumber(lineNumber);
}
