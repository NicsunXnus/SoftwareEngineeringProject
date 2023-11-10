#include "AbstractionExtractor.h"

void AbstractionExtractor::insertToAbstractionMap(string key, string value) {
       insertIntoMap(key, value, this->AbstractionStorageMap);
}



shared_ptr<StringMap> AbstractionExtractor::getStorageMap() {
    return this->AbstractionStorageMap;
}

void AbstractionExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) {
    extract(processedProgram);
}

void AbstractionExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    vector<shared_ptr<ProcessedProcedure>> procedures = processedProgram->getAllProcedures();
    for (shared_ptr<ProcessedProcedure> procedure : procedures) {
        procedure->accept(shared_from_this());
    }
}

void AbstractionExtractor::extract(shared_ptr<ProcessedProcedure> processedProcedure) {
    shared_ptr<ProcessedStmtList> statementList = processedProcedure->getStmts();
    statementList->accept(shared_from_this());
}

void AbstractionExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList) {
    vector<shared_ptr<ProcessedStmt>> stmts = processedStmtList->getStmts();
    for (shared_ptr<ProcessedStmt> stmt : stmts) {
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

void AbstractionExtractor::insertIntoMap(string key, string statementNumber, shared_ptr<StringMap> map) {
    // Insert to the map if the key is not found
    if (map->find(key) == map->end()) {
        map->insert({ key, unordered_set<string>() });
    }
    map->at(key).insert(statementNumber);
}