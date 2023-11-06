#include "ProcessedSemicolonStmt.h"

bool ProcessedSemicolonStmt::equalsTo(ProcessedSemicolonStmt& rhs) {
  shared_ptr<ProcessedStmt> castedThis = static_cast<shared_ptr<ProcessedStmt>>(this);
  shared_ptr<ProcessedStmt> castedThat = static_cast<shared_ptr<ProcessedStmt>>(make_shared<ProcessedSemicolonStmt>(rhs));
  return ProcessedStmt::checkEquality(castedThis, castedThat);
}