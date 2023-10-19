#pragma once
#include<stdio.h>
#include <iostream>
#include <string>

#include "EntityExtractor.h"

using namespace std;

void EntityExtractor::extract(shared_ptr<ProcessedProgram> processedProgram) {
    std::vector<std::shared_ptr<ProcessedProcedure>> procedures = processedProgram->getAllProcedures();
    for (std::shared_ptr<ProcessedProcedure> procedure : procedures) {
        std::string_view procName = procedure->getProcName();
        std::string procedureName(procName);
        this->insertToProcedureMap(procedureName);
        procedure->accept(shared_from_this());
    }
}

void EntityExtractor::extract(shared_ptr<ProcessedProcedure> processedProcedure) {
    std::shared_ptr<ProcessedStmtList> statementList = processedProcedure->getStmts();
    statementList->accept(shared_from_this());
}

void EntityExtractor::extract(shared_ptr<ProcessedStmtList> processedStmtList) {
    std::vector<std::shared_ptr<ProcessedStmt>> stmts = processedStmtList->getStmts();
    for (std::shared_ptr<ProcessedStmt> stmt : stmts) {
        stmt->accept(shared_from_this());
    }
}

void EntityExtractor::extract(shared_ptr<ProcessedStmt> processedStmt) {
}

void EntityExtractor::extract(shared_ptr<ProcessedWhileStmt> processedWhile) {
    string statementNumber = std::to_string(processedWhile->getStatementNumber());

    // for statementMap
    this->insertToStatementMap("while", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // for patternMap
    std::shared_ptr<Node> node = processedWhile->getConditionalExp();
    this->insertToPatternMap(statementNumber, node);

    node->accept(shared_from_this());

    // for whileBlock
    processedWhile->getWhileBlock()->accept(shared_from_this());
}

void EntityExtractor::extract(shared_ptr<ProcessedIfStmt> processedIf) {
    string statementNumber = std::to_string(processedIf->getStatementNumber());

    // for statementMap
    this->insertToStatementMap("if", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // for patternMap
    std::shared_ptr<Node> node = processedIf->getConditionalExp();
    this->insertToPatternMap(statementNumber, node);

    node->accept(shared_from_this());

    // for thenBlock
    processedIf->getThenBlock()->accept(shared_from_this());
    // for elseBlock
    processedIf->getElseBlock()->accept(shared_from_this());
}

void EntityExtractor::extract(shared_ptr<ProcessedAssignStmt> processedAssign) {
    string statementNumber = std::to_string(processedAssign->getStatementNumber());
    string assignedVariable = processedAssign->getLeft()->getName();

    //for variableMap
    this->insertToVariableMap(assignedVariable, statementNumber);

    // for statementMap
    this->insertToStatementMap("assign", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // for patternMap
    std::shared_ptr<Node> node = processedAssign->getRight();
    this->insertToPatternMap(statementNumber, node);

    node->accept(shared_from_this());
}

void EntityExtractor::extract(shared_ptr<ProcessedCallStmt> processedCall) {
    string statementNumber = std::to_string(processedCall->getStatementNumber());

    // For statementMap
    this->insertToStatementMap("call", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // For callProcNameMap
    string procedureName = processedCall->getProcedureName()->getName();
    this->insertToCallProcNameMap(statementNumber, procedureName);
}

void EntityExtractor::extract(shared_ptr<ProcessedReadStmt> processedRead) {
    string statementNumber = std::to_string(processedRead->getStatementNumber());

    // for statementMap
    this->insertToStatementMap("read", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // for variableMap
    string variableName = processedRead->getVariable()->getName();
    this->insertToVariableMap(variableName, statementNumber);

    // for printVarMap
    this->insertToReadVarNameMap(statementNumber, variableName);
}

void EntityExtractor::extract(shared_ptr<ProcessedPrintStmt> processedPrint) {
     string statementNumber = std::to_string(processedPrint->getStatementNumber());

    // for statementMap
    this->insertToStatementMap("print", statementNumber);
    this->insertToStatementMap("stmt", statementNumber);

    // for variableMap
    string variableName = processedPrint->getVariable()->getName();
    this->insertToVariableMap(variableName, statementNumber);

    // for printVarMap
    this->insertToPrintVarNameMap(statementNumber, variableName);
}

void EntityExtractor::extract(shared_ptr<OpNode> node) {
    std::vector<std::shared_ptr<Node>> children = node->getChildren();
    for (std::shared_ptr<Node> child : children) {
        child->accept(shared_from_this());
    }
}

void EntityExtractor::extract(shared_ptr<VariableNode> node) {
    std::string variableName = node->getValue();
    std::string statementNumber = std::to_string(node->getStatementNumber());
    this->insertToVariableMap(variableName, statementNumber);
}

void EntityExtractor::extract(shared_ptr<ConstantNode> node) {
    std::string constantValue = node->getValue();
    std::string statementNumber = std::to_string(node->getStatementNumber());

    this->insertToConstantMap(constantValue, statementNumber);
}


void EntityExtractor::insertToProcedureMap(string key) {
    insertToMap(procedureMap, key);
}

void EntityExtractor::insertToVariableMap(string key, string value) {
    insertToMap(variableMap, key, value);
}

void EntityExtractor::insertToStatementMap(string key, string value) {
    insertToMap(statementMap, key, value);
}

void EntityExtractor::insertToConstantMap(string key, string value) {
    insertToMap(constantMap, key, value);
}

void EntityExtractor::insertToCallProcNameMap(string key, string value) {
    insertToMap(callProcNameMap, key, value);
}

void EntityExtractor::insertToReadVarNameMap(string key, string value) {
    insertToMap(readVarNameMap, key, value);
}

void EntityExtractor::insertToPrintVarNameMap(string key, string value) {
    insertToMap(printVarNameMap, key, value);
}

void EntityExtractor::insertToPatternMap(string key, shared_ptr<Node> node) {
    if (patternMap->find(key) == patternMap->end()) {
        patternMap->insert({key, node});
    }
}

void EntityExtractor::insertToMap(shared_ptr<map<string, unordered_set<string>>> map, string key, string value) {
    if (map->find(key) == map->end()) {
        unordered_set<string> valueSet;
        valueSet.insert(value);
        map->insert({key, valueSet});
    } else {
        map->at(key).insert(value);
    }
}

void EntityExtractor::insertToMap(shared_ptr<map<string, unordered_set<string>>> map, string key) {
    if (map->find(key) == map->end()) {
        unordered_set<string> valueSet;
        map->insert({key, valueSet});
    } 
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getProcedureMap() {
    return procedureMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getVariableMap() {
    return variableMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getStatementMap() {
    return statementMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getConstantMap() {
    return constantMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getCallProcNameMap() {
    return callProcNameMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getReadVarNameMap() {
    return readVarNameMap;
}

shared_ptr<map<string, unordered_set<string>>> EntityExtractor::getPrintVarNameMap() {
    return printVarNameMap;
}

shared_ptr<map<string, shared_ptr<Node>>> EntityExtractor::getPatternMap() {
    return patternMap;
}