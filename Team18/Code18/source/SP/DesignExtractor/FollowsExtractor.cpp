#include "FollowsExtractor.h"

void FollowsExtractor::extract(std::shared_ptr<ProcessedStmtList> processedStmtList) {
    string previousStatementNumber = "-1";
    for (auto &stmt : processedStmtList->getStmts()) {
        std::string currentStatementNumber = std::to_string(stmt->getStatementNumber());
        if (previousStatementNumber != "-1") {
        this->insertToAbstractionMap(previousStatementNumber, currentStatementNumber);
        }
        previousStatementNumber = currentStatementNumber;
        stmt->accept(shared_from_this());
    }
}