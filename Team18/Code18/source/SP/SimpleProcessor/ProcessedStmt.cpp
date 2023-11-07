#include "ProcessedStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedStmt::checkEquality(shared_ptr<ProcessedStmt> left, shared_ptr<ProcessedStmt> right) {
  return left->statementNumber == right->statementNumber;
}

int ProcessedStmt::getStatementNumber() {
  return this->statementNumber;
}

string ProcessedStmt::getStatementNumberInString() {
  return to_string(this->statementNumber);
}