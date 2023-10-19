#ifndef TOKENIZEDIFSTMT_H
#define TOKENIZEDIFSTMT_H

#include <memory>
#include <vector>
#include "ProcessedConditionalStmt.h"
#include "ProcessedStmtList.h"
#include "../SimpleTokens/Token.h"

class ProcessedIfStmt;
class Extractor;

/// <summary>
/// If statement with 2 "children" statement lists
/// </summary>
class ProcessedIfStmt : public ProcessedConditionalStmt, public std::enable_shared_from_this<ProcessedIfStmt>  {
private:
  std::shared_ptr<ProcessedStmtList> thenBlock;
  std::shared_ptr<ProcessedStmtList> elseBlock;
public:
  ProcessedIfStmt(int statementNumber, std::shared_ptr<Node> conditionalExp,
    std::shared_ptr<ProcessedStmtList> thenBlock,
    std::shared_ptr<ProcessedStmtList> elseBlock)
    : ProcessedConditionalStmt{ statementNumber, conditionalExp },
      thenBlock{ thenBlock },
      elseBlock{ elseBlock } {};

  std::shared_ptr<ProcessedStmtList> getThenBlock() {
    return this->thenBlock;
  }
  std::shared_ptr<ProcessedStmtList> getElseBlock() {
    return this->elseBlock;
  }

  bool equalsTo(ProcessedIfStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;
};

#endif
