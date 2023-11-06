#include "ProcessedPrintStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedPrintStmt::equalsTo(ProcessedPrintStmt& rhs) {
  bool varEqual = Token::checkEquality(this->variable, rhs.variable);
  if (!varEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

shared_ptr<Token> ProcessedPrintStmt::getVariable() {
  return this->variable;
}

void ProcessedPrintStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedPrintStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedPrintStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}