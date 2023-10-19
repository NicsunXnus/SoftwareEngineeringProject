#ifndef TOKENIZEDSEMICOLONSTMT_H
#define TOKENIZEDSEMICOLONSTMT_H

#include <memory>
#include <vector>
#include "ProcessedStmt.h"
#include "../SimpleTokens/Token.h"

using namespace std::string_view_literals;

class ProcessedSemicolonStmt;
class Extractor;

/// <summary>
/// Regular statement that ends with a semicolon.
/// </summary>
class ProcessedSemicolonStmt : public ProcessedStmt {
public:
  ProcessedSemicolonStmt(int statementNumber) 
    : ProcessedStmt{ statementNumber } {};

  bool equalsTo(ProcessedSemicolonStmt& rhs);
  void accept(std::shared_ptr<Extractor> extractor) override {};
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) override {};
  void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) override {};
};
#endif
