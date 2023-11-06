#ifndef TOKENIZEDPRINTSTMT_H
#define TOKENIZEDPRINTSTMT_H

#include <memory>
#include <vector>
#include "ProcessedSemicolonStmt.h"
#include "../SimpleTokens/Token.h"

using namespace std;

class ProcessedPrintStmt;
class Extractor;

/// <summary>
/// A print statement
/// </summary>
class ProcessedPrintStmt : public ProcessedSemicolonStmt, public enable_shared_from_this<ProcessedPrintStmt> {
private:
  shared_ptr<Token> variable;

public:
  ProcessedPrintStmt(int statementNumber, shared_ptr<Token> variable)
    : ProcessedSemicolonStmt{ statementNumber },
      variable{ variable } {};

  shared_ptr<Token> getVariable();

  bool equalsTo(ProcessedPrintStmt& rhs);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;

};
#endif
