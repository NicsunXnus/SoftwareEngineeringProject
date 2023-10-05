#ifndef TOKENIZEDSTMT_H
#define TOKENIZEDSTMT_H

#include <memory>
#include <vector>

class TokenizedStmt;

/// <summary>
/// Abstraction for a tokenised statement.
/// 
/// Wrapper for a vector of tokens. Note that semicolons will NO LONGER be stored, since some statements will not end in semicolons
/// 
/// This can be one of 3 types:
/// - Regular Statement that ends with a semicolon
/// - Conditional Statement (if)
/// - Conditional Statement (while)
/// </summary>
class TokenizedStmt {
private:
  const int statementNumber;
public:
  TokenizedStmt(int statementNumber) : statementNumber(statementNumber) {};
  int getStatementNumber() {
    return this->statementNumber;
  }
  virtual ~TokenizedStmt() {};
  bool equalsTo(TokenizedStmt& rhs);
};

#endif
