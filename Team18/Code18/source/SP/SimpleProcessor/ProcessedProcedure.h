#ifndef PROCESSEDPROCEDURE
#define PROCESSEDPROCEDURE

#include <memory>
#include <vector>
#include <string>
#include "ProcessedStmtList.h"
#include "../DesignExtractor/Extractor.h"

using namespace std;

class ProcessedProcedure;

/// <summary>
/// Abstraction for a procedure
/// </summary>
class ProcessedProcedure : public enable_shared_from_this<ProcessedProcedure> {
private:
  string procedureName; // to make sure the string is always on the heap
  shared_ptr<ProcessedStmtList> statementList;
public:
  ProcessedProcedure(string procedureName, shared_ptr<ProcessedStmtList> stmtList)
    : procedureName{ procedureName },
      statementList{ stmtList } {};

  // if provided with a vector of statements instead, create stmtList automatically and assign to field
  ProcessedProcedure(string procedureName, vector<shared_ptr<ProcessedStmt>> stmtVector);

  string_view getProcName() {
    return string_view(this->procedureName);
  }

  shared_ptr<ProcessedStmtList> getStmts() {
    return this->statementList;
  }

  static bool checkEquality(shared_ptr<ProcessedProcedure> left, shared_ptr<ProcessedProcedure> right);
  void accept(shared_ptr<Extractor> extractor);

};

#endif
