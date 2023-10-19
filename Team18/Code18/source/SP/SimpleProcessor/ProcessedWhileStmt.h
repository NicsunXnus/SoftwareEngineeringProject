#ifndef TOKENIZEDWHILESTMT_H
#define TOKENIZEDWHILESTMT_H

#include <memory>
#include <vector>
#include "ProcessedConditionalStmt.h"
#include "ProcessedStmtList.h"
#include "../SimpleTokens/Token.h"

using namespace std::string_view_literals;

class ProcessedWhileStmt;
class Extractor;


/// <summary>
/// While statement with 1 "child" statement list
/// </summary>
class ProcessedWhileStmt : public ProcessedConditionalStmt, public std::enable_shared_from_this<ProcessedWhileStmt> {
private:
  std::shared_ptr<ProcessedStmtList> whileBlock;
public:
  ProcessedWhileStmt(int statementNumber, std::shared_ptr<Node> conditionalExp,
    std::shared_ptr<ProcessedStmtList> whileBlock)
    : ProcessedConditionalStmt{ statementNumber, conditionalExp },
      whileBlock{ whileBlock } {};

  std::shared_ptr<ProcessedStmtList> getWhileBlock() {
    return this->whileBlock;
  }

  bool equalsTo(ProcessedWhileStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;
};

#endif
