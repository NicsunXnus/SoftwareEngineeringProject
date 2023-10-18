#include "AbstractionExtractor.h"

void AbstractionExtractor::insertToAbstractionMap(string key, string value) {
       insertIntoMap(key, value, this->AbstractionStorageMap);
}

void AbstractionExtractor::insertToAbstractionStarMap(string key, string value) {
    insertIntoMap(key, value, this->AbstractionStarStorageMap);
}

shared_ptr<map<string, unordered_set<string>>> AbstractionExtractor::getStorageMap() {
    return this->AbstractionStorageMap;
}

shared_ptr<map<string, unordered_set<string>>> AbstractionExtractor::getStorageStarMap() {
    return this->AbstractionStarStorageMap;
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

void AbstractionExtractor::insertIntoMap(string key, string statementNumber, shared_ptr<map<string, unordered_set<string>>> map) {
    // Insert to the map if the key is not found
    if (map->find(key) == map->end()) {
        map->insert({ key, unordered_set<string>() });
    }
    map->at(key).insert(statementNumber);
}

void AbstractionExtractor::processAbstractionStar() {
    for (const auto& [key, value] : *AbstractionStorageMap) {
        unordered_set<string> values = unordered_set<string>(value);
        for (const auto& value : value) {
            processAbstractionStarHelper(value, values);
        }
        AbstractionStarStorageMap->insert({ key, values });
    }
}

void AbstractionExtractor::processAbstractionStarHelper(string value, unordered_set<string>& values) {
    if (AbstractionStorageMap->find(value) != AbstractionStorageMap->end()) {
        for (const auto& value : AbstractionStorageMap->at(value)) {
            values.insert(value);
            processAbstractionStarHelper(value, values);
        }
    }
}