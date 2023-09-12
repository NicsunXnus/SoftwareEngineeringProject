#include "TokenizerResults.h"

bool TokenizedProgram::containsProcedure(std::string prodName) {
  return this->procedures.count(prodName) == 1;
}

bool TokenizedProgram::addProcedure(std::string prodName, TokenizedProcedure prod) {
  if (this->containsProcedure(prodName)) {
    return false;
  }
  this->procedures[prodName] = prod;
  return true;
}

TokenizedProcedure TokenizedProgram::getProcedure(std::string prodName) {
  return this->procedures[prodName];
}

TokenizedProcedure::TokenizedProcedure(std::vector<TokenizedStmt> stmtVector) {
  TokenizedStmtList stmtList = TokenizedStmtList(stmtVector);
  this->statementList = stmtList;
}

bool TokenizedRegularStmt::addContent(std::shared_ptr<Token> content) {
  this->contents.push_back(content);
}