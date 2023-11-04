#ifndef TOKENIZEDWHILESTMT_H
#define TOKENIZEDWHILESTMT_H

#include <memory>
#include <vector>
#include "ProcessedConditionalStmt.h"
#include "ProcessedStmtList.h"

using namespace std;

class ProcessedWhileStmt;
class Extractor;

/// <summary>
/// While statement with 1 "child" statement list
/// </summary>
class ProcessedWhileStmt : public ProcessedConditionalStmt, public enable_shared_from_this<ProcessedWhileStmt> {
private:
  shared_ptr<ProcessedStmtList> whileBlock;
public:
  ProcessedWhileStmt(int statementNumber, shared_ptr<Node> conditionalExp,
    shared_ptr<ProcessedStmtList> whileBlock)
    : ProcessedConditionalStmt{ statementNumber, conditionalExp },
      whileBlock{ whileBlock } {};

  shared_ptr<ProcessedStmtList> getWhileBlock();

  bool equalsTo(ProcessedWhileStmt& rhs);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;
};

#endif
