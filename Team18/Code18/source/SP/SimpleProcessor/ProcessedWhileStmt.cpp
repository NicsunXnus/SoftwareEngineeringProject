#include "ProcessedWhileStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedWhileStmt::equalsTo(ProcessedWhileStmt& rhs) {
  if ((*(this->whileBlock)).equalsTo(*(rhs.whileBlock))) {
    return false;
  }
  auto castedThis = static_cast<ProcessedConditionalStmt&>(*this);
  auto castedThat = static_cast<ProcessedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

void ProcessedWhileStmt::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedWhileStmt::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedWhileStmt::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}