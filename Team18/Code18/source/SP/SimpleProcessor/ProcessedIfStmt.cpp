#include "ProcessedIfStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedIfStmt::equalsTo(ProcessedIfStmt& rhs) {
  if (!(ProcessedStmtList::checkEquality(this->thenBlock, rhs.thenBlock))) {
    return false;
  }
  if (!(ProcessedStmtList::checkEquality(this->elseBlock, rhs.elseBlock))) {
    return false;
  }
  auto castedThis = static_cast<ProcessedConditionalStmt&>(*this);
  auto castedThat = static_cast<ProcessedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

shared_ptr<ProcessedStmtList> ProcessedIfStmt::getThenBlock() {
  return this->thenBlock;
}
shared_ptr<ProcessedStmtList> ProcessedIfStmt::getElseBlock() {
  return this->elseBlock;
}

void ProcessedIfStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedIfStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedIfStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}