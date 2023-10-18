#include "ProcessedReadStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedReadStmt::equalsTo(ProcessedReadStmt& rhs) {
  bool varEqual = Token::checkEquality(this->variable, rhs.variable);
  if (!varEqual) return false;
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

void ProcessedReadStmt::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedReadStmt::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedReadStmt::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}