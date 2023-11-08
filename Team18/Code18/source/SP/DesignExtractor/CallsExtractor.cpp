#include "CallsExtractor.h"

void CallsExtractor::extract(shared_ptr<ProcessedCallStmt> processedCall) {
    string statementNumber = to_string(processedCall->getStatementNumber());
    string procedureName = this->getProcedureNameFromStatementNumber(statementNumber);
    string calledProcedure = processedCall->getProcedureName()->getName();
    this->insertToAbstractionMap(procedureName, calledProcedure);
}

string CallsExtractor::getProcedureNameFromStatementNumber(string lineNumber) {
    return this->procedureLineNumberExtractor->getProcedureNameFromStatementNumber(lineNumber);
}
