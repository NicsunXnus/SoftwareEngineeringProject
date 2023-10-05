#ifndef TOKENIZEDCONDSTMT_H
#define TOKENIZEDCONDSTMT_H

#include <memory>
#include <vector>
#include "TokenizedStmt.h"
#include "TokenizedStmtList.h"
#include "../SimpleTokens/Token.h"

using namespace std::string_view_literals;

class TokenizedConditionalStmt;
class TokenizedIfStmt;
class TokenizedWhileStmt;

/// <summary>
/// Conditional Statement containing a relational/Conditional expression
/// </summary>
class TokenizedConditionalStmt : public TokenizedStmt {
private:
  std::vector<std::shared_ptr<Token>> conditionalExp;
public:
  TokenizedConditionalStmt(int statementNumber, std::vector<std::shared_ptr<Token>> conditionalExp)
    : TokenizedStmt{ statementNumber },
      conditionalExp{ conditionalExp } {};
  std::vector<std::shared_ptr<Token>> getConditionalExp() {
    return this->conditionalExp;
  };

  bool equalsTo(TokenizedConditionalStmt& rhs);
};

/// <summary>
/// If statement with 2 "children" statement lists
/// </summary>
class TokenizedIfStmt : public TokenizedConditionalStmt {
private:
  std::shared_ptr<TokenizedStmtList> thenBlock;
  std::shared_ptr<TokenizedStmtList> elseBlock;
public:
  TokenizedIfStmt(int statementNumber, std::vector<std::shared_ptr<Token>> conditionalExp,
    std::shared_ptr<TokenizedStmtList> thenBlock,
    std::shared_ptr<TokenizedStmtList> elseBlock)
    : TokenizedConditionalStmt{ statementNumber, conditionalExp },
      thenBlock{ thenBlock },
      elseBlock{ elseBlock } {};

  std::shared_ptr<TokenizedStmtList> getThenBlock() {
    return this->thenBlock;
  }
  std::shared_ptr<TokenizedStmtList> getElseBlock() {
    return this->elseBlock;
  }

  bool equalsTo(TokenizedIfStmt& rhs);
};

/// <summary>
/// While statement with 1 "child" statement list
/// </summary>
class TokenizedWhileStmt : public TokenizedConditionalStmt {
private:
  std::shared_ptr<TokenizedStmtList> whileBlock;
public:
  TokenizedWhileStmt(int statementNumber, std::vector<std::shared_ptr<Token>> conditionalExp,
    std::shared_ptr<TokenizedStmtList> whileBlock)
    : TokenizedConditionalStmt{ statementNumber, conditionalExp },
      whileBlock{ whileBlock } {};

  std::shared_ptr<TokenizedStmtList> getWhileBlock() {
    return this->whileBlock;
  }

  bool equalsTo(TokenizedWhileStmt& rhs);
};

#endif
