#ifndef TOKENIZEDREADSTMT_H
#define TOKENIZEDREADSTMT_H

#include <memory>
#include <vector>
#include "ProcessedSemicolonStmt.h"
#include "../SimpleTokens/Token.h"

using namespace std::string_view_literals;

class ProcessedReadStmt;

/// <summary>
/// A read statement
/// </summary>
class ProcessedReadStmt : public ProcessedSemicolonStmt, public std::enable_shared_from_this<ProcessedReadStmt> {
private:
  std::shared_ptr<Token> variable;

public:
  ProcessedReadStmt(int statementNumber, std::shared_ptr<Token> variable)
    : ProcessedSemicolonStmt{ statementNumber },
      variable{ variable } {};

  std::shared_ptr<Token> getVariable() {
    return this->variable;
  }

  bool equalsTo(ProcessedReadStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;
};
#endif
