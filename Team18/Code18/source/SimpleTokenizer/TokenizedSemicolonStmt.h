#ifndef TOKENIZEDSEMICOLONSTMT_H
#define TOKENIZEDSEMICOLONSTMT_H

#include <memory>
#include <vector>

#include "../SimpleTokens/Token.h"
#include "TokenizedStmt.h"

using namespace std::string_view_literals;

class TokenizedSemicolonStmt;

/// <summary>
/// Regular statement that ends with a semicolon
/// </summary>
class TokenizedSemicolonStmt : public TokenizedStmt {
 private:
  std::vector<std::shared_ptr<Token>> contents;

 public:
  TokenizedSemicolonStmt(int statementNumber,
                         std::vector<std::shared_ptr<Token>> contents)
      : TokenizedStmt{statementNumber}, contents{contents} {};

  std::vector<std::shared_ptr<Token>> getContents() { return this->contents; }

  bool equalsTo(TokenizedSemicolonStmt& rhs);
};
#endif
