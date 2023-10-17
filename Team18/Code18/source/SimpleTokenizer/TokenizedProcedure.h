#ifndef TOKENIZEDPROCEDURE
#define TOKENIZEDPROCEDURE

#include <memory>
#include <string>
#include <vector>

#include "TokenizedStmtList.h"

using namespace std::string_view_literals;

class TokenizedProcedure;

/// <summary>
/// Abstraction for a procedure
/// </summary>
class TokenizedProcedure {
 private:
  std::string procedureName;  // to make sure the string is always on the heap
  std::shared_ptr<TokenizedStmtList> statementList;

 public:
  TokenizedProcedure(std::string procedureName,
                     std::shared_ptr<TokenizedStmtList> stmtList)
      : procedureName{procedureName}, statementList{stmtList} {};

  // if provided with a vector of statements instead, create stmtList
  // automatically and assign to field
  TokenizedProcedure(std::string procedureName,
                     std::vector<std::shared_ptr<TokenizedStmt>> stmtVector);

  std::string_view getName() { return std::string_view(this->procedureName); }

  std::shared_ptr<TokenizedStmtList> getStmts() { return this->statementList; }

  bool equalsTo(TokenizedProcedure& rhs);
};

#endif
