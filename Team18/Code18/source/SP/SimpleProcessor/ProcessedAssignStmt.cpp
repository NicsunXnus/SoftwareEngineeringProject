#include "ProcessedAssignStmt.h"
#include "../DesignExtractor/Extractor.h"

bool ProcessedAssignStmt::equalsTo(ProcessedAssignStmt& that) {
  bool lhsEqual = Token::checkEquality(this->LHS, that.LHS);
  bool rhsEqual = (this->RHS)->isIdentical(that.RHS, true);
  if (!lhsEqual || !rhsEqual) {
    return false;
  }
  auto castedThis = static_cast<ProcessedSemicolonStmt&>(*this);
  auto castedThat = static_cast<ProcessedSemicolonStmt&>(that);
  return castedThis.equalsTo(castedThat);
}
shared_ptr<Token> ProcessedAssignStmt::getLeft() {
  return this->LHS;
}

shared_ptr<Node> ProcessedAssignStmt::getRight() {
  return this->RHS;
}

void ProcessedAssignStmt::accept(shared_ptr<Extractor> extractor) {
  extractor->extract(shared_from_this());
}

void ProcessedAssignStmt::accept(shared_ptr<Extractor> extractor, string procedureName) {
  extractor->extract(shared_from_this(), procedureName);
}

void ProcessedAssignStmt::accept(shared_ptr<Extractor> extractor, unordered_set<string>& prevStatementNumbers) {
  extractor->extract(shared_from_this(), prevStatementNumbers);
}