#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H

#include <map>
#include <variant>
#include <vector>
#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;

class ParsedProgram;
class ParsedProcedure;
class ParsedStmtList;
class ParsedStmt;

// Wrapper for a vector of Procedures
class ParsedProgram {
private:
  // procedure name, Procedure class
  std::map<std::string, ParsedProcedure> procedures;
public:
  // Default constructor
  ParsedProgram() {}
  ParsedProgram(std::map<std::string, ParsedProcedure> procedures) : procedures{ procedures } {};

  std::map<std::string, ParsedProcedure> getAllProcedures() {
    return this->procedures;
  }

  bool containsProcedure(std::string prodName) {}

  bool addProcedure(std::string prodName, ParsedProcedure prod) {}

  ParsedProcedure getProcedure(std::string prodName) {}
};

// Simply a wrapper for a statementList
class ParsedProcedure {
private:
  ParsedStmtList statementList;
public:
  ParsedProcedure(ParsedStmtList stmtList) : statementList{ stmtList } {};

  // if provided with a vector of statements instead, create stmtList automatically and assign to field
  ParsedProcedure(std::vector<ParsedStmt> stmtVector);

  // No point for procedures to return a ParsedStmtList object since the goal is to get the statements anyways
  std::vector<ParsedStmt> getStmts() {
    return this->statementList.getStmts();
  }
};

// Wrapper for a vector of statements.
// This intermediary is needed to allow to aid in tokenising statement lists that are in if-else and while loops.
class ParsedStmtList {
private:
  std::vector<ParsedStmt> statements;
public:
  // Default Constructor
  ParsedStmtList() {}
  // Constructor to fill all existing statements
  ParsedStmtList(std::vector<ParsedStmt> statements) : statements{ statements } {};
  
  std::vector<ParsedStmt> getStmts() {
    return this->statements;
  }
};

// Wrapper for a vector of tokens. Note that semicolons will NO LONGER be stored, since some statements will not end in semicolons
class ParsedStmt {
private:
  std::vector<std::variant<std::shared_ptr<Token>, ParsedStmtList>> contents;
public:
  // Default Constructor
  ParsedStmt() {}
  // Constructor to fill all existing statements
  ParsedStmt(std::vector<std::variant<std::shared_ptr<Token>, ParsedStmtList>> contents) : contents{ contents } {};

  std::vector<std::variant<std::shared_ptr<Token>, ParsedStmtList>> getContents() {
    return this->contents;
  }

  bool addContent(std::variant<std::shared_ptr<Token>, ParsedStmtList> content);
};


#endif
