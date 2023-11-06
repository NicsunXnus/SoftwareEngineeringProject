#include "ModifiesExtractor.h"

void ModifiesExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile) {
  string statementNumber = to_string(processedWhile->getStatementNumber());

  processedWhile->getWhileBlock()->accept(shared_from_this(), statementNumber);
}

void ModifiesExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf) {
  string statementNumber = to_string(processedIf->getStatementNumber());

  processedIf->getThenBlock()->accept(shared_from_this(), statementNumber);
  processedIf->getElseBlock()->accept(shared_from_this(), statementNumber);
}

void ModifiesExtractor::extract(shared_ptr<ProcessedAssignStmt> processedAssign) {
    string statementNumber = to_string(processedAssign->getStatementNumber());
    string assignedVariable = processedAssign->getLeft()->getName();

    this->addStatementNumberAndProcedureName(assignedVariable, statementNumber);
}

void ModifiesExtractor::extract(shared_ptr<ProcessedReadStmt> processedRead) {
    string statementNumber = to_string(processedRead->getStatementNumber());
    string variableName = processedRead->getVariable()->getName();
    this->addStatementNumberAndProcedureName(variableName, statementNumber);
}