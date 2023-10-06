#include "TokenizedProcedure.h"

TokenizedProcedure::TokenizedProcedure(std::string procedureName, std::vector<std::shared_ptr<TokenizedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = std::make_shared<TokenizedStmtList>(stmtVector);
}

bool TokenizedProcedure::equalsTo(TokenizedProcedure& rhs) {
  if (this->procedureName != rhs.procedureName) {
    return false;
  }
  return (*(this->statementList)).equalsTo(*(rhs.statementList));
}