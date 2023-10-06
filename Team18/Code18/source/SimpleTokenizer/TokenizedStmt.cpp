#include "TokenizedStmt.h"

bool TokenizedStmt::equalsTo(TokenizedStmt& rhs) {
  if (this->statementNumber != rhs.statementNumber) {
    return false;
  }
  return true;
}