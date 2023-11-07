#include "ProcessedStmtList.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedStmtList::checkEquality(shared_ptr<ProcessedStmtList> left, shared_ptr<ProcessedStmtList> right) {
  if (left->statements.size() != right->statements.size()) {
    return false;
  }
  for (int i = 0; i < left->statements.size(); i++) {
    if (ProcessedStmt::checkEquality(left->statements[i], right->statements[i])) {
      return false;
    }
  }
  return true;
}

vector<shared_ptr<ProcessedStmt>> ProcessedStmtList::getStmts() {
  return this->statements;
}

void ProcessedStmtList::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedStmtList::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

unordered_set<string> ProcessedStmtList::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  return extractor->extract(shared_from_this(), prevStatementNumbers, isNotIf);
}



