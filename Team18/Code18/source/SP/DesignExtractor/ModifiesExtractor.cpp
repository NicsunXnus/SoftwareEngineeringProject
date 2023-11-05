#include "ModifiesExtractor.h"

void ModifiesExtractor::extract(std::shared_ptr<ProcessedWhileStmt> processedWhile) {
  string statementNumber = std::to_string(processedWhile->getStatementNumber());

  processedWhile->getWhileBlock()->accept(shared_from_this(), statementNumber);
}

void ModifiesExtractor::extract(std::shared_ptr<ProcessedIfStmt> processedIf) {
  string statementNumber = std::to_string(processedIf->getStatementNumber());

  processedIf->getThenBlock()->accept(shared_from_this(), statementNumber);
  processedIf->getElseBlock()->accept(shared_from_this(), statementNumber);
}

void ModifiesExtractor::extract(std::shared_ptr<ProcessedAssignStmt> processedAssign) {
    string statementNumber = std::to_string(processedAssign->getStatementNumber());
    string assignedVariable = processedAssign->getLeft()->getName();

    this->addStatementNumberAndProcedureName(assignedVariable, statementNumber);
}

void ModifiesExtractor::extract(std::shared_ptr<ProcessedReadStmt> processedRead) {
    string statementNumber = std::to_string(processedRead->getStatementNumber());
    string variableName = processedRead->getVariable()->getName();
    this->addStatementNumberAndProcedureName(variableName, statementNumber);
}