#include "ParentsExtractor.h"

void ParentsExtractor::extract(std::shared_ptr<ProcessedStmtList> processedStmtList, std::string parentStatementNumber) {
    for (auto& stmt : processedStmtList->getStmts()) {
        this->insertToAbstractionMap(std::to_string(stmt->getStatementNumber()), parentStatementNumber);
        stmt->accept(shared_from_this());
    }
}

void ParentsExtractor::extract(std::shared_ptr<ProcessedWhileStmt> processedWhile) {
    string statementNumber = std::to_string(processedWhile->getStatementNumber());

    processedWhile->getWhileBlock()->accept(shared_from_this(), statementNumber);
}

void ParentsExtractor::extract(std::shared_ptr<ProcessedIfStmt> processedIf) {
    string statementNumber = std::to_string(processedIf->getStatementNumber());

    processedIf->getThenBlock()->accept(shared_from_this(), statementNumber);
    processedIf->getElseBlock()->accept(shared_from_this(), statementNumber);
}

