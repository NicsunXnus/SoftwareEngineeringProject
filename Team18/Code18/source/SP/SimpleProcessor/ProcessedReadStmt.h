#ifndef TOKENIZEDREADSTMT_H
#define TOKENIZEDREADSTMT_H

#include <memory>
#include <vector>
#include "ProcessedSemicolonStmt.h"
#include "../SimpleTokens/Token.h"
#include "../DesignExtractor/Extractor.h"

using namespace std;

class ProcessedReadStmt;
class Extractor;

/// <summary>
/// A read statement
/// </summary>
class ProcessedReadStmt : public ProcessedSemicolonStmt, public enable_shared_from_this<ProcessedReadStmt> {
private:
  shared_ptr<Token> variable;

public:
  ProcessedReadStmt(int statementNumber, shared_ptr<Token> variable)
    : ProcessedSemicolonStmt{ statementNumber },
      variable{ variable } {};

  shared_ptr<Token> getVariable();

  bool equalsTo(ProcessedReadStmt& rhs);
  void accept(shared_ptr<Extractor> extractor) override;
  void accept(shared_ptr<Extractor> extractor, string procedureName) override;
  void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) override;
};
#endif
