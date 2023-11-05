#ifndef TOKENIZEDSEMICOLONSTMT_H
#define TOKENIZEDSEMICOLONSTMT_H

#include <memory>
#include <vector>
#include "ProcessedStmt.h"
#include "../SimpleTokens/Token.h"

using namespace std;

class ProcessedSemicolonStmt;
class Extractor;

/// <summary>
/// Regular statement that ends with a semicolon.
/// </summary>
class ProcessedSemicolonStmt : public ProcessedStmt {
protected:
  friend class ProcessedAssignStmt;
  friend class ProcessedCallStmt;
  friend class ProcessedPrintStmt;
  friend class ProcessedReadStmt;
  bool equalsTo(ProcessedSemicolonStmt& rhs);

public:
  ProcessedSemicolonStmt(int statementNumber) 
    : ProcessedStmt{ statementNumber } {};

  void accept(shared_ptr<Extractor> extractor) override {};
  void accept(shared_ptr<Extractor> extractor, string procedureName) override {};
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override {};
};
#endif
