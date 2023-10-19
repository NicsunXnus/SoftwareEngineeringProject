#include "ProcessedIfStmt.h"
#include "../DesignExtractor/Extractor.h"


bool ProcessedIfStmt::equalsTo(ProcessedIfStmt& rhs) {
  if ((*(this->thenBlock)).equalsTo(*(rhs.thenBlock))) {
    return false;
  }
  if ((*(this->elseBlock)).equalsTo(*(rhs.elseBlock))) {
    return false;
  }
  auto castedThis = static_cast<ProcessedConditionalStmt&>(*this);
  auto castedThat = static_cast<ProcessedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

void ProcessedIfStmt::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedIfStmt::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedIfStmt::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}