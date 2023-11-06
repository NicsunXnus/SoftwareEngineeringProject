#include "UsesExtractor.h"

void UsesExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile) {
    shared_ptr<Node> node = processedWhile->getConditionalExp();
    string statementNumber = to_string(processedWhile->getStatementNumber());

    node->accept(shared_from_this());
    processedWhile->getWhileBlock()->accept(shared_from_this(), statementNumber);
}

void UsesExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf) {
    shared_ptr<Node> node = processedIf->getConditionalExp();
    string statementNumber = to_string(processedIf->getStatementNumber());

    node->accept(shared_from_this());
    processedIf->getThenBlock()->accept(shared_from_this(), statementNumber);
    processedIf->getElseBlock()->accept(shared_from_this(), statementNumber);
}

void UsesExtractor::extract(shared_ptr<ProcessedAssignStmt> processedAssign) {
    shared_ptr<Node> node = processedAssign->getRight();
    node->accept(shared_from_this());
}

void UsesExtractor::extract(shared_ptr<ProcessedPrintStmt> processedPrint) {
    string statementNumber = to_string(processedPrint->getStatementNumber());
    string variableName = processedPrint->getVariable()->getName();
    this->addStatementNumberAndProcedureName(variableName, statementNumber);
}

void UsesExtractor::extract(shared_ptr<OpNode> node) {
  vector<shared_ptr<Node>> children = node->getChildren();
  for (shared_ptr<Node> child : children) {
    child->accept(shared_from_this());
  }
}

void UsesExtractor::extract(shared_ptr<VariableNode> node) {
  this->addStatementNumberAndProcedureName(node->getValue(), to_string(node->getStatementNumber()));
}