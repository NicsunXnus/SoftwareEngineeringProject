#include "TokenizedStmtList.h"

bool TokenizedStmtList::equalsTo(TokenizedStmtList& rhs) {
  if (this->statements.size() != rhs.statements.size()) {
    return false;
  }
  for (int i = 0; i < this->statements.size(); i++) {
    if ((*(this->statements[i])).equalsTo(*(rhs.statements[i]))) return false;
  }
  return true;
}