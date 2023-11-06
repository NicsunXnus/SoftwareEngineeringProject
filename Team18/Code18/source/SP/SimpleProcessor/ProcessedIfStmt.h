#ifndef TOKENIZEDIFSTMT_H
#define TOKENIZEDIFSTMT_H

#include <memory>
#include <vector>
#include "ProcessedConditionalStmt.h"
#include "ProcessedStmtList.h"

class ProcessedIfStmt;
class Extractor;

/// <summary>
/// If statement with 2 "children" statement lists
/// </summary>
class ProcessedIfStmt : public ProcessedConditionalStmt, public enable_shared_from_this<ProcessedIfStmt>  {
private:
  shared_ptr<ProcessedStmtList> thenBlock;
  shared_ptr<ProcessedStmtList> elseBlock;

public:
  ProcessedIfStmt(int statementNumber, shared_ptr<Node> conditionalExp,
    shared_ptr<ProcessedStmtList> thenBlock,
    shared_ptr<ProcessedStmtList> elseBlock)
    : ProcessedConditionalStmt{ statementNumber, conditionalExp },
      thenBlock{ thenBlock },
      elseBlock{ elseBlock } {};

  shared_ptr<ProcessedStmtList> getThenBlock();
  shared_ptr<ProcessedStmtList> getElseBlock();

  bool equalsTo(ProcessedIfStmt& rhs);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;
};

#endif
