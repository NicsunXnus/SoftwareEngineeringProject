#ifndef TOKENIZEDSTMT_H
#define TOKENIZEDSTMT_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_set>

class Extractor;
class ProcessedStmt;

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
  ProcessedStmt(int statementNumber) : statementNumber(statementNumber) {};
  int getStatementNumber() {
    return this->statementNumber;
  }

  std::string getStatementNumberInString() {
      return std::to_string(this->statementNumber);
  }

  virtual ~ProcessedStmt() {};
  bool equalsTo(ProcessedStmt& rhs);

  virtual void accept(std::shared_ptr<Extractor> extractor) {};
  virtual void accept(std::shared_ptr<Extractor> extractor, std::string procedureName) {};
  virtual void accept(std::shared_ptr<Extractor> extractor, std::unordered_set<std::string>& prevStatementNumbers) {};
};

#endif
