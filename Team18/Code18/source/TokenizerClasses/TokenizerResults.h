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

  bool operator==(TokenizedProgram& rhs) {
    if (this->procedures.size() != rhs.procedures.size()) {
      return false;
    }
    for (int i = 0; i < this->procedures.size(); i++) {
      if (*(this->procedures[i]) != *(rhs.procedures[i])) return false;
    }
    return true;
  }

  bool operator!=(TokenizedProgram& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedProcedure& rhs) {
    if (this->procedureName != rhs.procedureName) {
      return false;
    }
    return *(this->statementList) == *(rhs.statementList);
  }

  bool operator!=(TokenizedProcedure& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedStmtList& rhs) {
    if (this->statements.size() != rhs.statements.size()) {
      return false;
    }
    for (int i = 0; i < this->statements.size(); i++) {
      if (*(this->statements[i]) != *(rhs.statements[i])) return false;
    }
    return true;
  }

  bool operator!=(TokenizedStmtList& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedStmt& rhs) {
    if (this->statementNumber != rhs.statementNumber) {
      return false;
    }
    return true;
  }

  bool operator!=(TokenizedStmt& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedSemicolonStmt& rhs) {
    if (this->contents.size() != rhs.contents.size()) {
      return false;
    }
    for (int i = 0; i < this->contents.size(); i++) {
      if (!Token::checkEquality(this->contents[i], rhs.contents[i])) return false;
    }
    return static_cast<TokenizedStmt&>(*this) == rhs;
  }

  bool operator!=(TokenizedSemicolonStmt& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedConditionalStmt& rhs) {
    if (*(this->conditionalExp) != *(rhs.conditionalExp)) {
      return false;
    }
    return static_cast<TokenizedStmt&>(*this) == rhs;
  }

  bool operator!=(TokenizedConditionalStmt& rhs) {
    return !(*this == rhs);
  }
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

  bool operator==(TokenizedIfStmt& rhs) {
    if (*(this->thenBlock) != *(rhs.thenBlock)) {
      return false;
    }
    if (*(this->elseBlock) != *(rhs.elseBlock)) {
      return false;
    }
    return static_cast<TokenizedConditionalStmt&>(*this) == rhs;
  }

  bool operator!=(TokenizedIfStmt& rhs) {
    return !(*this == rhs);
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

  bool operator==(TokenizedWhileStmt& rhs) {
    if (*(this->whileBlock) != *(rhs.whileBlock)) {
      return false;
    }
    return static_cast<TokenizedConditionalStmt&>(*this) == rhs;
  }

  bool operator!=(TokenizedWhileStmt& rhs) {
    return !(*this == rhs);
  }
};

#endif
