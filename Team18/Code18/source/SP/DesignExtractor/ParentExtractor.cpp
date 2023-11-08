#include "ParentExtractor.h"

void ParentExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList, string parentStatementNumber) {
    for (auto& stmt : processedStmtList->getStmts()) {
        this->insertToAbstractionMap(to_string(stmt->getStatementNumber()), parentStatementNumber);
        stmt->accept(shared_from_this());
    }
}

void ParentExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile) {
    string statementNumber = to_string(processedWhile->getStatementNumber());

    processedWhile->getWhileBlock()->accept(shared_from_this(), statementNumber);
}

void ParentExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf) {
    string statementNumber = to_string(processedIf->getStatementNumber());

    processedIf->getThenBlock()->accept(shared_from_this(), statementNumber);
    processedIf->getElseBlock()->accept(shared_from_this(), statementNumber);
}

