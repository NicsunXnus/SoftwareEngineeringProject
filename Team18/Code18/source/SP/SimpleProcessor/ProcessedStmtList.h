#ifndef TOKENIZEDSTMTLIST_H
#define TOKENIZEDSTMTLIST_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>
#include "ProcessedStmt.h"

using namespace std;

class ProcessedStmtList;
class Extractor;

/// <summary>
/// Abstraction for a vector of statements.
/// 
/// This intermediary abstraction is needed to allow to aid in tokenising statement lists that are in if-else and while loops.
/// </summary>
class ProcessedStmtList : public enable_shared_from_this<ProcessedStmtList>  {
private:
  vector<shared_ptr<ProcessedStmt>> statements;
public:
  // Constructor to fill all existing statements
  ProcessedStmtList(vector<shared_ptr<ProcessedStmt>> statements) : statements{ statements } {};

  vector<shared_ptr<ProcessedStmt>> getStmts();

  static bool checkEquality(shared_ptr<ProcessedStmtList> left, shared_ptr<ProcessedStmtList> right);

  void accept(shared_ptr<Extractor> extractor);
  void accept(shared_ptr<Extractor> extractor, string procedureName);
  unordered_set<string> accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers);

};

#endif
