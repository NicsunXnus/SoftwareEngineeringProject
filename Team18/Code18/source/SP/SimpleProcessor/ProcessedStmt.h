#ifndef TOKENIZEDSTMT_H
#define TOKENIZEDSTMT_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

class ProcessedStmt;
class Extractor;

/// <summary>
/// Abstraction for a tokenised statement.
/// 
/// Wrapper for a vector of tokens. Note that semicolons will NO LONGER be stored, since some statements will not end in semicolons
/// 
/// This can be one of 3 types:
/// - Regular Statement that ends with a semicolon
/// - Conditional Statement (if)
/// - Conditional Statement (while)
/// </summary>
class ProcessedStmt {
private:
  const int statementNumber;

public:
  static bool checkEquality(shared_ptr<ProcessedStmt> left, shared_ptr<ProcessedStmt> right);

  ProcessedStmt(int statementNumber) : statementNumber(statementNumber) {};

  int getStatementNumber();

  string getStatementNumberInString();

  virtual ~ProcessedStmt() {};

  virtual void accept(shared_ptr<Extractor> extractor) {};
  virtual void accept(shared_ptr<Extractor> extractor, string procedureName) {};
  virtual void accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {};
};

#endif
