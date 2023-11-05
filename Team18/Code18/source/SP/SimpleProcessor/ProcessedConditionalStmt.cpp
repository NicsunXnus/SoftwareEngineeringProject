#include "ProcessedConditionalStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedConditionalStmt::equalsTo(ProcessedConditionalStmt& rhs) {
  bool condEqual = (this->condExp)->isIdentical(rhs.condExp, true);
  if (!condEqual) {
    return false;
  }
  shared_ptr<ProcessedStmt> castedThis = static_cast<shared_ptr<ProcessedStmt>>(this);
  shared_ptr<ProcessedStmt> castedThat = static_cast<shared_ptr<ProcessedStmt>>(make_shared<ProcessedConditionalStmt>(rhs));
  return ProcessedStmt::checkEquality(castedThis, castedThat);
}

shared_ptr<Node> ProcessedConditionalStmt::getConditionalExp() {
  return this->condExp;
};