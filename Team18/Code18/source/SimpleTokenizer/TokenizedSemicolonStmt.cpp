#include "TokenizedSemicolonStmt.h"

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