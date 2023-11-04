#include "ProcessedProcedure.h"
#include "../DesignExtractor/Extractor.h"

ProcessedProcedure::ProcessedProcedure(string procedureName, vector<shared_ptr<ProcessedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = make_shared<ProcessedStmtList>(stmtVector);
}

bool ProcessedProcedure::checkEquality(shared_ptr<ProcessedProcedure> left, shared_ptr<ProcessedProcedure> right) {
  if (left->procedureName != right->procedureName) {
    return false;
  }
  return ProcessedStmtList::checkEquality(left->statementList, right->statementList);
}

void ProcessedProcedure::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

string_view ProcessedProcedure::getProcName() {
  return string_view(this->procedureName);
}

shared_ptr<ProcessedStmtList> ProcessedProcedure::getStmts() {
  return this->statementList;
}
