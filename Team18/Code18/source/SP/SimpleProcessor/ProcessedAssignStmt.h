#ifndef PROCESSEDASSIGNSTMT_H
#define PROCESSEDASSIGNSTMT_H

#include <memory>
#include <vector>

#include "ProcessedSemicolonStmt.h"
#include "../SimpleTokens/Token.h"
#include "../AST/Node.h"
#include "../AST/TreeBuilder.h"

using namespace std;

class ProcessedAssignStmt;
class Extractor;

/// <summary>
/// An assignment statement
/// </summary>
class ProcessedAssignStmt : public ProcessedSemicolonStmt, public enable_shared_from_this<ProcessedAssignStmt> {
private:
  shared_ptr<Token> LHS;
  shared_ptr<Node> RHS;

public:
  ProcessedAssignStmt(int statementNumber, shared_ptr<Token> LHS, shared_ptr<Node> RHS)
    : ProcessedSemicolonStmt{ statementNumber },
      LHS{ LHS },
      RHS{ RHS } {};

  shared_ptr<Token> getLeft();

  shared_ptr<Node> getRight();

  bool equalsTo(ProcessedAssignStmt& that);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;
};
#endif
