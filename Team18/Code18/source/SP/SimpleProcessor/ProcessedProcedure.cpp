#include "ProcessedProcedure.h"

ProcessedProcedure::ProcessedProcedure(std::string procedureName, std::vector<std::shared_ptr<ProcessedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = std::make_shared<ProcessedStmtList>(stmtVector);
}

bool ProcessedProcedure::checkEquality(shared_ptr<ProcessedProcedure> left, shared_ptr<ProcessedProcedure> right) {
  if (left->procedureName != right->procedureName) {
    return false;
  }
  return ProcessedStmtList::checkEquality(left->statementList, right->statementList);
}

void ProcessedProcedure::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}