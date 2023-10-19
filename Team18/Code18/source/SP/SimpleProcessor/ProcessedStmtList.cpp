#include "ProcessedStmtList.h"
#include "../DesignExtractor/Extractor.h"


bool ProcessedStmtList::equalsTo(ProcessedStmtList& rhs) {
  if (this->statements.size() != rhs.statements.size()) {
    return false;
  }
  for (int i = 0; i < this->statements.size(); i++) {
    if ((*(this->statements[i])).equalsTo(*(rhs.statements[i]))) return false;
  }
  return true;
}

void ProcessedStmtList::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedStmtList::accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

std::unordered_set<std::string> ProcessedStmtList::accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {
  return extractor->extract(shared_from_this(), prevStatementNumbers);
}



