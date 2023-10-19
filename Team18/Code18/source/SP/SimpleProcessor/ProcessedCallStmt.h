#ifndef TOKENIZEDCALLSTMT_H
#define TOKENIZEDCALLSTMT_H

#include <memory>
#include <vector>
#include "ProcessedSemicolonStmt.h"

using namespace std::string_view_literals;

class ProcessedCallStmt;
class Extractor;

/// <summary>
/// A call statement
/// </summary>
class ProcessedCallStmt : public ProcessedSemicolonStmt, public std::enable_shared_from_this<ProcessedCallStmt> {
private:
  std::shared_ptr<Token> procedureName;

public:
  ProcessedCallStmt(int statementNumber, std::shared_ptr<Token> procedurename)
    : ProcessedSemicolonStmt{ statementNumber },
      procedureName{ procedurename } {};
  
  std::shared_ptr<Token> getProcedureName() {
    return this->procedureName;
  }

  bool equalsTo(ProcessedCallStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override;
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override;
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override;
};
#endif
