#include "ProcessedReadStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedReadStmt::equalsTo(ProcessedReadStmt& rhs) {
  bool varEqual = Token::checkEquality(this->variable, rhs.variable);
  if (!varEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

shared_ptr<Token> ProcessedReadStmt::getVariable() {
  return this->variable;
}

void ProcessedReadStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedReadStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedReadStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}