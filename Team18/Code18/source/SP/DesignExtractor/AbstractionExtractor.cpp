#include "AbstractionExtractor.h"

void AbstractionExtractor::insertToAbstractionMap(string key, string value) {
       insertIntoMap(key, value, this->AbstractionStorageMap);
}



shared_ptr<unordered_map<string, unordered_set<string>>> AbstractionExtractor::getStorageMap() {
    return this->AbstractionStorageMap;
}

void AbstractionExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) {
    extract(processedProgram);
}

void AbstractionExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    std::vector<std::shared_ptr<ProcessedProcedure>> procedures = processedProgram->getAllProcedures();
    for (std::shared_ptr<ProcessedProcedure> procedure : procedures) {
        procedure->accept(shared_from_this());
    }
}

void AbstractionExtractor::extract(shared_ptr<ProcessedProcedure> processedProcedure) {
    std::shared_ptr<ProcessedStmtList> statementList = processedProcedure->getStmts();
    statementList->accept(shared_from_this());
}

void AbstractionExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList) {
    std::vector<std::shared_ptr<ProcessedStmt>> stmts = processedStmtList->getStmts();
    for (std::shared_ptr<ProcessedStmt> stmt : stmts) {
        stmt->accept(shared_from_this());
    }
}

void AbstractionExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile) {
    processedWhile->getWhileBlock()->accept(shared_from_this());
}

void AbstractionExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf) {
    processedIf->getThenBlock()->accept(shared_from_this());
    processedIf->getElseBlock()->accept(shared_from_this());
}

void AbstractionExtractor::insertIntoMap(string key, string statementNumber, shared_ptr<unordered_map<string, unordered_set<string>>> map) {
    // Insert to the map if the key is not found
    if (map->find(key) == map->end()) {
        map->insert({ key, unordered_set<string>() });
    }
    map->at(key).insert(statementNumber);
}