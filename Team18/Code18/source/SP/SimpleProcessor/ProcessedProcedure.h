#ifndef PROCESSEDPROCEDURE
#define PROCESSEDPROCEDURE

#include <memory>
#include <vector>
#include <string>
#include "ProcessedStmtList.h"

using namespace std::string_view_literals;

class ProcessedProcedure;
class Extractor;

/// <summary>
/// Abstraction for a procedure
/// </summary>
class ProcessedProcedure : public std::enable_shared_from_this<ProcessedProcedure> {
private:
  std::string procedureName; // to make sure the string is always on the heap
  std::shared_ptr<ProcessedStmtList> statementList;
public:
  ProcessedProcedure(std::string procedureName, std::shared_ptr<ProcessedStmtList> stmtList)
    : procedureName{ procedureName },
      statementList{ stmtList } {};

  // if provided with a vector of statements instead, create stmtList automatically and assign to field
  ProcessedProcedure(std::string procedureName, std::vector<std::shared_ptr<ProcessedStmt>> stmtVector);

  std::string_view getProcName() {
    return std::string_view(this->procedureName);
  }

  std::shared_ptr<ProcessedStmtList> getStmts() {
    return this->statementList;
  }

  bool equalsTo(ProcessedProcedure& rhs);
  void accept(std::shared_ptr<Extractor> extractor);

};

#endif
