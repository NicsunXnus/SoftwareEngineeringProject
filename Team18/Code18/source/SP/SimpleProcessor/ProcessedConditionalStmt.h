#ifndef TOKENIZEDCONDSTMT_H
#define TOKENIZEDCONDSTMT_H

#include <memory>
#include <vector>

#include "ProcessedStmt.h"
#include "../AST/Node.h"

using namespace std;

class ProcessedConditionalStmt;
class Extractor;

/// <summary>
/// Conditional Statement containing a relational/Conditional expression
/// </summary>
class ProcessedConditionalStmt : public ProcessedStmt {
private:
  shared_ptr<Node> condExp;
protected:
  friend class ProcessedIfStmt;
  friend class ProcessedWhileStmt;
  bool equalsTo(ProcessedConditionalStmt& rhs);

public:
  ProcessedConditionalStmt(int statementNumber, shared_ptr<Node> conditionalExp)
    : ProcessedStmt{ statementNumber },
      condExp{ conditionalExp } {};

  shared_ptr<Node> getConditionalExp();

  void accept(shared_ptr<Extractor> extractor) override {};
  void accept(shared_ptr<Extractor> extractor, string procedureName) override {};
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override {};
};

#endif
