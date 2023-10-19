#include "ProcessedCallStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedCallStmt::equalsTo(ProcessedCallStmt& rhs) {
  bool procEqual = Token::checkEquality(this->procedureName, rhs.procedureName);
  if (!procEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}


void ProcessedCallStmt::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedCallStmt::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedCallStmt::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}