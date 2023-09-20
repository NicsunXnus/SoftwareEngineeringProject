#include "TokenizerResults.h"

TokenizedProcedure::TokenizedProcedure(std::string procedureName, std::vector<std::shared_ptr<TokenizedStmt>> stmtVector) {
  this->procedureName = procedureName;
  this->statementList = std::make_shared<TokenizedStmtList>(stmtVector);
}

bool TokenizedProgram::equalsTo(TokenizedProgram& rhs) {
  if (this->procedures.size() != rhs.procedures.size()) {
    return false;
  }
  for (int i = 0; i < this->procedures.size(); i++) {
    if ((*(this->procedures[i])).equalsTo(*(rhs.procedures[i]))) return false;
  }
  return true;
}

bool TokenizedProcedure::equalsTo(TokenizedProcedure& rhs) {
  if (this->procedureName != rhs.procedureName) {
    return false;
  }
  return (*(this->statementList)).equalsTo(*(rhs.statementList));
}

bool TokenizedStmtList::equalsTo(TokenizedStmtList& rhs) {
  if (this->statements.size() != rhs.statements.size()) {
    return false;
  }
  for (int i = 0; i < this->statements.size(); i++) {
    if ((*(this->statements[i])).equalsTo(*(rhs.statements[i]))) return false;
  }
  return true;
}

bool TokenizedStmt::equalsTo(TokenizedStmt& rhs) {
  if (this->statementNumber != rhs.statementNumber) {
    return false;
  }
  return true;
}

bool TokenizedSemicolonStmt::equalsTo(TokenizedSemicolonStmt& rhs) {
  if (this->contents.size() != rhs.contents.size()) {
    return false;
  }
  for (int i = 0; i < this->contents.size(); i++) {
    if (!Token::checkEquality(this->contents[i], rhs.contents[i])) return false;
  }
  auto castedThis = static_cast<TokenizedStmt&>(*this);
  auto castedThat = static_cast<TokenizedStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

bool TokenizedConditionalStmt::equalsTo(TokenizedConditionalStmt& rhs) {
  if (this->conditionalExp.size() != rhs.conditionalExp.size()) {
    return false;
  }
  for (int i = 0; i < this->conditionalExp.size(); i++) {
    if (!Token::checkEquality(this->conditionalExp[i], rhs.conditionalExp[i])) return false;
  }
  auto castedThis = static_cast<TokenizedStmt&>(*this);
  auto castedThat = static_cast<TokenizedStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

bool TokenizedIfStmt::equalsTo(TokenizedIfStmt& rhs) {
  if ((*(this->thenBlock)).equalsTo(*(rhs.thenBlock))) {
    return false;
  }
  if ((*(this->elseBlock)).equalsTo(*(rhs.elseBlock))) {
    return false;
  }
  auto castedThis = static_cast<TokenizedConditionalStmt&>(*this);
  auto castedThat = static_cast<TokenizedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}

bool TokenizedWhileStmt::equalsTo(TokenizedWhileStmt& rhs) {
  if ((*(this->whileBlock)).equalsTo(*(rhs.whileBlock))) {
    return false;
  }
  auto castedThis = static_cast<TokenizedConditionalStmt&>(*this);
  auto castedThat = static_cast<TokenizedConditionalStmt&>(rhs);
  return castedThis.equalsTo(castedThat);
}