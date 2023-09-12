#ifndef TOKENIZERRESULTS_H
#define TOKENIZERRESULTS_H

#include <map>
#include <variant>
#include <vector>
#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;

class TokenizedProgram;
class TokenizedProcedure;
class TokenizedStmtList;
class TokenizedStmt;
class TokenizedSemicolonStmt;
class TokenizedConditionStmt;
class TokenizedIfStmt;
class TokenizedWhileStmt;

/// <summary>
/// Abstraction for a vector of Procedures
/// </summary>
class TokenizedProgram {
private:
  // procedure name, Procedure class. Ordered.
  std::map<std::string, TokenizedProcedure> procedures;
public:
  // Default constructor
  TokenizedProgram() {}
  TokenizedProgram(std::map<std::string, TokenizedProcedure> procedures) : procedures{ procedures } {};

  std::map<std::string, TokenizedProcedure> getAllProcedures() {
    return this->procedures;
  }

  bool containsProcedure(std::string prodName);

  bool addProcedure(std::string prodName, TokenizedProcedure prod);

  TokenizedProcedure getProcedure(std::string prodName);
};

/// <summary>
/// Abstraction for a statement list inside a procedure
/// </summary>
class TokenizedProcedure {
private:
  TokenizedStmtList statementList;
public:
  TokenizedProcedure(TokenizedStmtList stmtList) : statementList{ stmtList } {};

  // if provided with a vector of statements instead, create stmtList automatically and assign to field
  TokenizedProcedure(std::vector<TokenizedStmt> stmtVector);

  // No point for procedures to return a ParsedStmtList object since the goal is to get the statements anyways
  std::vector<TokenizedStmt> getStmts() {
    return this->statementList.getStmts();
  }
};


/// <summary>
/// Abstraction for a vector of statements.
/// 
/// This intermediary abstraction  is needed to allow to aid in tokenising statement lists that are in if-else and while loops.
/// </summary>
class TokenizedStmtList {
private:
  std::vector<TokenizedStmt> statements;
public:
  // Default Constructor
  TokenizedStmtList() {}
  // Constructor to fill all existing statements
  TokenizedStmtList(std::vector<TokenizedStmt> statements) : statements{ statements } {};
  
  std::vector<TokenizedStmt> getStmts() {
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

};

/// <summary>
/// Regular statement that ends with a semicolon
/// </summary>
class TokenizedSemicolonStmt : public TokenizedStmt {
private:
  std::vector<std::shared_ptr<Token>> contents;
public:
  TokenizedSemicolonStmt(std::vector<std::shared_ptr<Token>> contents) : contents{ contents } {};

  std::vector<std::shared_ptr<Token>> getContents() {
    return this->contents;
  }
};

/// <summary>
/// Conditional Statement containing a relational expression
/// </summary>
class TokenizedConditionStatement : public TokenizedStmt {
private:
  std::vector<std::shared_ptr<Token>> conditionStatement;
public:
  TokenizedConditionStatement(std::vector<std::shared_ptr<Token>> conditionStatement)
    : conditionStatement{ conditionStatement } {};

  std::vector<std::shared_ptr<Token>> getCondition() {
    return this->conditionStatement;
  }
};

/// <summary>
/// If statement with 2 "children" statement lists
/// </summary>
class TokenizedIfStmt : public TokenizedConditionStatement {
private:
  TokenizedStmtList ifBlock;
  TokenizedStmtList elseBlock;
public:
  TokenizedIfStmt(std::vector<std::shared_ptr<Token>> conditionBlock, TokenizedStmtList ifBlock, TokenizedStmtList elseBlock)
    : TokenizedConditionStatement{ conditionBlock } {
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
  }
  TokenizedStmtList getIfBlock() {
    return this->ifBlock;
  }
  TokenizedStmtList getElseBlock() {
    return this->elseBlock;
  }
};

/// <summary>
/// While statement with 1 "child" statement list
/// </summary>
class TokenizedWhileStmt : public TokenizedConditionStatement {
private:
  TokenizedStmtList whileBlock;
public:
  TokenizedWhileStmt(std::vector<std::shared_ptr<Token>> conditionBlock, TokenizedStmtList whileBlock)
    : TokenizedConditionStatement{ conditionBlock } {
    this->whileBlock = whileBlock;
  }
  TokenizedStmtList getWhileBlock() {
    return this->whileBlock;
  }
};

#endif
