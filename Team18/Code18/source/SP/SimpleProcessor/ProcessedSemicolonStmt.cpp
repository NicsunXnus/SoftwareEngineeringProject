#include "ProcessedSemicolonStmt.h"

bool ProcessedSemicolonStmt::equalsTo(ProcessedSemicolonStmt& rhs) {
  auto castedThis = static_cast<ProcessedStmt&>(*this);
  auto castedThat = static_cast<ProcessedStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}