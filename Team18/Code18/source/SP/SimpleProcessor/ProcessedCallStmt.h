#ifndef TOKENIZEDCALLSTMT_H
#define TOKENIZEDCALLSTMT_H

#include <memory>
#include <vector>
#include "ProcessedSemicolonStmt.h"

using namespace std;

class ProcessedCallStmt;
class Extractor;

/// <summary>
/// A call statement
/// </summary>
class ProcessedCallStmt : public ProcessedSemicolonStmt, public enable_shared_from_this<ProcessedCallStmt> {
private:
  shared_ptr<Token> procedureName;

public:
  ProcessedCallStmt(int statementNumber, shared_ptr<Token> procedurename)
    : ProcessedSemicolonStmt{ statementNumber },
      procedureName{ procedurename } {};
  
  shared_ptr<Token> getProcedureName();

  bool equalsTo(ProcessedCallStmt& rhs);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;
};
#endif
