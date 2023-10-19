#ifndef PROCESSEDASSIGNSTMT_H
#define PROCESSEDASSIGNSTMT_H

#include <memory>
#include <vector>

#include "ProcessedSemicolonStmt.h"
#include "../SimpleTokens/Token.h"
#include "../AST/Node.h"
#include "../AST/TreeBuilder.h"

using namespace std::string_view_literals;

class ProcessedAssignStmt;
class Extractor;


/// <summary>
/// An assignment statement
/// </summary>
class ProcessedAssignStmt : public ProcessedSemicolonStmt, public std::enable_shared_from_this<ProcessedAssignStmt> {
private:
  std::shared_ptr<Token> LHS;
  std::shared_ptr<Node> RHS;

public:
  ProcessedAssignStmt(int statementNumber, std::shared_ptr<Token> LHS, std::shared_ptr<Node> RHS)
    : ProcessedSemicolonStmt{ statementNumber },
      LHS{ LHS },
      RHS{ RHS } {};

  std::shared_ptr<Token> getLeft() {
    return this->LHS;
  }

  std::shared_ptr<Node> getRight() {
    return this->RHS;
  }

  bool equalsTo(ProcessedAssignStmt& that);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;
};
#endif
