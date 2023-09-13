#include "TokenizerResults.h"

TokenizedProcedure::TokenizedProcedure(std::string procedureName, std::vector<std::shared_ptr<TokenizedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = std::make_shared<TokenizedStmtList>(stmtVector);
}