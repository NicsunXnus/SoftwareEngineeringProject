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
class ProcessedPrintStmt : public ProcessedSemicolonStmt, public std::enable_shared_from_this<ProcessedPrintStmt> {
private:
  std::shared_ptr<Token> variable;

public:
  ProcessedPrintStmt(int statementNumber, std::shared_ptr<Token> variable)
    : ProcessedSemicolonStmt{ statementNumber },
      variable{ variable } {};

  std::shared_ptr<Token> getVariable();

  bool equalsTo(ProcessedPrintStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;

};
#endif
