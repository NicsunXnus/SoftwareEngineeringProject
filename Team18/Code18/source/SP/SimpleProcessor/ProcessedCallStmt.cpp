#include "ProcessedCallStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedCallStmt::equalsTo(ProcessedCallStmt& rhs) {
  bool procEqual = Token::checkEquality(this->procedureName, rhs.procedureName);
  if (!procEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

shared_ptr<Token> ProcessedCallStmt::getProcedureName() {
  return this->procedureName;
}

void ProcessedCallStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedCallStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedCallStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}