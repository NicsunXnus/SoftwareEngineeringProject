#include "ProcessedPrintStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedPrintStmt::equalsTo(ProcessedPrintStmt& rhs) {
  bool varEqual = Token::checkEquality(this->variable, rhs.variable);
  if (!varEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}


void ProcessedPrintStmt::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedPrintStmt::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedPrintStmt::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}