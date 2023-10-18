#ifndef TOKENIZEDSTMTLIST_H
#define TOKENIZEDSTMTLIST_H

#include <memory>
#include <vector>

#include "TokenizedStmt.h"

class TokenizedStmtList;

/// <summary>
/// Abstraction for a vector of statements.
///
/// This intermediary abstraction is needed to allow to aid in tokenising
/// statement lists that are in if-else and while loops.
/// </summary>
class TokenizedStmtList {
 private:
  std::vector<std::shared_ptr<TokenizedStmt>> statements;

 public:
  // Constructor to fill all existing statements
  TokenizedStmtList(std::vector<std::shared_ptr<TokenizedStmt>> statements)
      : statements{statements} {};

  std::vector<std::shared_ptr<TokenizedStmt>> getStmts() {
    return this->statements;
  }

  bool equalsTo(TokenizedStmtList& rhs);
};

#endif
