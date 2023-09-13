#ifndef TOKENIZERRESULTS_H
#define TOKENIZERRESULTS_H

#include <memory>
#include <vector>
#include "../TokenClasses/Token.h"
#include "../TokenClasses/OperatorToken.h"

using namespace std::string_view_literals;

class TokenizedProgram;
class TokenizedProcedure;
class TokenizedStmtList;
class TokenizedStmt;
class TokenizedSemicolonStmt;
class TokenizedConditionalExp;
class TokenizedConditionalStmt;
class TokenizedIfStmt;
class TokenizedWhileStmt;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class TokenizedProgram {
private:
  // no need shared_ptr of vector because copying over pointers is not memory intensive.
  std::vector<std::shared_ptr<TokenizedProcedure>> procedures;
public:
  TokenizedProgram(std::vector<std::shared_ptr<TokenizedProcedure>>  procedures) : procedures{ procedures } {};

  std::vector<std::shared_ptr<TokenizedProcedure>>  getAllProcedures() {
    return this->procedures;
  }
};

/// <summary>
/// Abstraction for a procedure
/// </summary>
class TokenizedProcedure {
private:
  std::string procedureName; // to make sure the string is always on the heap
  std::shared_ptr<TokenizedStmtList> statementList;
public:
  TokenizedProcedure(std::string procedureName, std::shared_ptr<TokenizedStmtList> stmtList)
    : procedureName{ procedureName },
      statementList{ stmtList } {};

  // if provided with a vector of statements instead, create stmtList automatically and assign to field
  TokenizedProcedure(std::string procedureName, std::vector<std::shared_ptr<TokenizedStmt>> stmtVector);

  std::string_view getName() {
    return std::string_view(this->procedureName);
  }

  std::shared_ptr<TokenizedStmtList> getStmts() {
    return this->statementList;
  }
};

/// <summary>
/// Abstraction for a vector of statements.
/// 
/// This intermediary abstraction is needed to allow to aid in tokenising statement lists that are in if-else and while loops.
/// </summary>
class TokenizedStmtList {
private:
  std::vector<std::shared_ptr<TokenizedStmt>> statements;
public:
  // Constructor to fill all existing statements
  TokenizedStmtList(std::vector<std::shared_ptr<TokenizedStmt>> statements) : statements{ statements } {};

  std::vector<std::shared_ptr<TokenizedStmt>> getStmts() {
    return this->statements;
  }
};

// Wrapper for a vector of tokens. Note that semicolons will NO LONGER be stored, since some statements will not end in semicolons

/// <summary>
/// Abstraction for a tokenised statement.
/// 
/// Just a wrapper.
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
};

/// <summary>
/// Regular statement that ends with a semicolon
/// </summary>
class TokenizedSemicolonStmt : public TokenizedStmt {
private:
  std::vector<std::shared_ptr<Token>> contents;
public:
  TokenizedSemicolonStmt(int statementNumber, std::vector<std::shared_ptr<Token>> contents) 
    : TokenizedStmt{ statementNumber }, 
      contents{ contents } {};

  std::vector<std::shared_ptr<Token>> getContents() {
    return this->contents;
  }
};

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
};

#endif
