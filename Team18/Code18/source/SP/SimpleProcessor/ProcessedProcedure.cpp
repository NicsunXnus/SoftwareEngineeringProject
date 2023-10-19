#include "ProcessedProcedure.h"
#include "../DesignExtractor/Extractor.h"

ProcessedProcedure::ProcessedProcedure(std::string procedureName, std::vector<std::shared_ptr<ProcessedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = std::make_shared<ProcessedStmtList>(stmtVector);
}

bool ProcessedProcedure::equalsTo(ProcessedProcedure& rhs) {
  if (this->procedureName != rhs.procedureName) {
    return false;
  }
  return (*(this->statementList)).equalsTo(*(rhs.statementList));
}

void ProcessedProcedure::accept(std::shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}