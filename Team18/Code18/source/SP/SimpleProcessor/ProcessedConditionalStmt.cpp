#include "ProcessedConditionalStmt.h"

bool ProcessedConditionalStmt::equalsTo(ProcessedConditionalStmt& rhs) {
  bool condEqual = (this->condExp)->isIdentical(rhs.condExp, true);
  if (!condEqual) {
    return false;
  }
  auto castedThis = static_cast<ProcessedStmt&>(*this);
  auto castedThat = static_cast<ProcessedStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}
