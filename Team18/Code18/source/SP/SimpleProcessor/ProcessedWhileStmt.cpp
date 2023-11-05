#include "ProcessedWhileStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedWhileStmt::equalsTo(ProcessedWhileStmt& rhs) {
  if (!(ProcessedStmtList::checkEquality(this->whileBlock, rhs.whileBlock))) {
    return false;
  }
  auto castedThis = static_cast<ProcessedConditionalStmt&>(*this);
  auto castedThat = static_cast<ProcessedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

shared_ptr<ProcessedStmtList> ProcessedWhileStmt::getWhileBlock() {
  return this->whileBlock;
}

void ProcessedWhileStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedWhileStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedWhileStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}