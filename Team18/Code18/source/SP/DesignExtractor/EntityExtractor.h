#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <map>

#include "Extractor.h"

using namespace std;

class EntityExtractor : public Extractor {
public:
    // Constructor
    EntityExtractor() : procedureMap(make_shared<map<string, unordered_set<string>>>()),
        variableMap(make_shared<map<string, unordered_set<string>>>()),
        statementMap(make_shared<map<string, unordered_set<string>>>()),
        constantMap(make_shared<map<string, unordered_set<string>>>()),
        callProcNameMap(make_shared<map<string, unordered_set<string>>>()),
        readVarNameMap(make_shared<map<string, unordered_set<string>>>()),
        printVarNameMap(make_shared<map<string, unordered_set<string>>>()),
        patternMap(make_shared<map<string, shared_ptr<Node>>>()) {
    }

    void insertToProcedureMap(string key);
    void insertToVariableMap(string key, string value);
    void insertToStatementMap(string key, string value);
    void insertToConstantMap(string key, string value);
    void insertToCallProcNameMap(string key, string value);
    void insertToReadVarNameMap(string key, string value);
    void insertToPrintVarNameMap(string key, string value);
    void insertToPatternMap(string key, shared_ptr<Node> node);

    shared_ptr<map<string, unordered_set<string>>> getProcedureMap();
    shared_ptr<map<string, unordered_set<string>>> getVariableMap();
    shared_ptr<map<string, unordered_set<string>>> getStatementMap();
    shared_ptr<map<string, unordered_set<string>>> getConstantMap();
    shared_ptr<map<string, unordered_set<string>>> getCallProcNameMap();
    shared_ptr<map<string, unordered_set<string>>> getReadVarNameMap();
    shared_ptr<map<string, unordered_set<string>>> getPrintVarNameMap();
    shared_ptr<map<string, shared_ptr<Node>>> getPatternMap();

    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProcedure> processedProcedure) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList) override;
    void extract(shared_ptr<ProcessedStmt> processedStmt) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
    void extract(shared_ptr<ProcessedAssignStmt> processedAssign) override;
    void extract(shared_ptr<ProcessedCallStmt> processedCall) override;
    void extract(shared_ptr<ProcessedReadStmt> processedRead) override;
    void extract(shared_ptr<ProcessedPrintStmt> processedPrint) override;
    void extract(shared_ptr<OpNode> node) override;
    void extract(shared_ptr<VariableNode> node) override;
    void extract(shared_ptr<ConstantNode> node) override;

private:
    shared_ptr<map<string, unordered_set<string>>> procedureMap;
    shared_ptr<map<string, unordered_set<string>>> variableMap;
    shared_ptr<map<string, unordered_set<string>>> statementMap;
    shared_ptr<map<string, unordered_set<string>>> constantMap;
    shared_ptr<map<string, unordered_set<string>>> callProcNameMap;
    shared_ptr<map<string, unordered_set<string>>> readVarNameMap;
    shared_ptr<map<string, unordered_set<string>>> printVarNameMap;
    shared_ptr<map<string, shared_ptr<Node>>> patternMap;

    void insertToMap(shared_ptr<map<string, unordered_set<string>>> map, string key, string value);
    void insertToMap(shared_ptr<map<string, unordered_set<string>>> map, string key);
};
