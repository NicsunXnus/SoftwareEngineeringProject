#ifndef TOKENIZEDCONDSTMT_H
#define TOKENIZEDCONDSTMT_H

#include <memory>
#include <vector>

#include "ProcessedStmt.h"
#include "../SimpleTokens/Token.h"
#include "../AST/Node.h"
#include "../AST/TreeBuilder.h"

using namespace std::string_view_literals;

class ProcessedConditionalStmt;
class Extractor;

/// <summary>
/// Conditional Statement containing a relational/Conditional expression
/// </summary>
class ProcessedConditionalStmt : public ProcessedStmt {
private:
  std::shared_ptr<Node> condExp;
public:
  ProcessedConditionalStmt(int statementNumber, std::shared_ptr<Node> conditionalExp)
    : ProcessedStmt{ statementNumber },
      condExp{ conditionalExp } {};

  std::shared_ptr<Node> getConditionalExp() {
    return this->condExp;
  };

  bool equalsTo(ProcessedConditionalStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override {};
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override {};
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override {};
};

#endif
