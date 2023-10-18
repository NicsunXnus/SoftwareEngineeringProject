#include "TokenizedConditionalStmt.h"

bool TokenizedConditionalStmt::equalsTo(TokenizedConditionalStmt& rhs) {
  if (this->conditionalExp.size() != rhs.conditionalExp.size()) {
    return false;
  }
  for (int i = 0; i < this->conditionalExp.size(); i++) {
    if (!Token::checkEquality(this->conditionalExp[i], rhs.conditionalExp[i]))
      return false;
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