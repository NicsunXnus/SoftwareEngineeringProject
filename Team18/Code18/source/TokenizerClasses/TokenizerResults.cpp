#include "TokenizerResults.h"

bool ParsedProgram::containsProcedure(std::string prodName) {
  return this->procedures.count(prodName) == 1;
}

bool ParsedProgram::addProcedure(std::string prodName, ParsedProcedure prod) {
  if (this->containsProcedure(prodName)) {
    return false;
  }
  this->procedures[prodName] = prod;
  return true;
}

ParsedProcedure ParsedProgram::getProcedure(std::string prodName) {
  return this->procedures[prodName];
}

ParsedProcedure::ParsedProcedure(std::vector<ParsedStmt> stmtVector) {
  ParsedStmtList stmtList = ParsedStmtList(stmtVector);
  this->statementList = stmtList;
}

bool ParsedStmt::addContent(std::variant<std::shared_ptr<Token>, ParsedStmtList> content) {
  this->contents.push_back(content);
}