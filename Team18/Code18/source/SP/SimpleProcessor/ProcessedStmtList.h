#ifndef TOKENIZEDSTMTLIST_H
#define TOKENIZEDSTMTLIST_H

#include <memory>
#include <vector>
#include "ProcessedStmt.h"
#include <string>
#include <unordered_set>

class Extractor;
class ProcessedStmtList;

/// <summary>
/// Abstraction for a vector of statements.
/// 
/// This intermediary abstraction is needed to allow to aid in tokenising statement lists that are in if-else and while loops.
/// </summary>
class ProcessedStmtList : public std::enable_shared_from_this<ProcessedStmtList>  {
private:
  std::vector<std::shared_ptr<ProcessedStmt>> statements;
public:
  // Constructor to fill all existing statements
  ProcessedStmtList(std::vector<std::shared_ptr<ProcessedStmt>> statements) : statements{ statements } {};

  std::vector<std::shared_ptr<ProcessedStmt>> getStmts() {
    return this->statements;
  }

  bool equalsTo(ProcessedStmtList& rhs);

  void accept(std::shared_ptr<Extractor> extractor);
  void accept(std::shared_ptr<Extractor> extractor, std::string procedureName);
  std::unordered_set<std::string> accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers);

};

#endif
