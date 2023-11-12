#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "Extractor.h"

using namespace std;

/**
 * This class is used to extract the entity abstraction from the AST
 */
class EntityExtractor : public Extractor {
public:
    // Constructor
    EntityExtractor() : procedureMap(make_shared<StringMap>()),
        variableMap(make_shared<StringMap>()),
        statementMap(make_shared<StringMap>()),
        constantMap(make_shared<StringMap>()),
        callProcNameMap(make_shared<StringMap>()),
        readVarNameMap(make_shared<StringMap>()),
        printVarNameMap(make_shared<StringMap>()),
        patternMap(make_shared<unordered_map<string, shared_ptr<Node>>>()) {
    }

    void insertToProcedureMap(string key);
    void insertToVariableMap(string key, string value);
    void insertToStatementMap(string key, string value);
    void insertToConstantMap(string key, string value);
    void insertToCallProcNameMap(string key, string value);
    void insertToReadVarNameMap(string key, string value);
    void insertToPrintVarNameMap(string key, string value);
    void insertToPatternMap(string key, shared_ptr<Node> node);

    shared_ptr<StringMap> getProcedureMap();
    shared_ptr<StringMap> getVariableMap();
    shared_ptr<StringMap> getStatementMap();
    shared_ptr<StringMap> getConstantMap();
    shared_ptr<StringMap> getCallProcNameMap();
    shared_ptr<StringMap> getReadVarNameMap();
    shared_ptr<StringMap> getPrintVarNameMap();
    shared_ptr<unordered_map<string, shared_ptr<Node>>> getPatternMap();

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
    shared_ptr<StringMap> procedureMap;
    shared_ptr<StringMap> variableMap;
    shared_ptr<StringMap> statementMap;
    shared_ptr<StringMap> constantMap;
    shared_ptr<StringMap> callProcNameMap;
    shared_ptr<StringMap> readVarNameMap;
    shared_ptr<StringMap> printVarNameMap;
    shared_ptr<unordered_map<string, shared_ptr<Node>>> patternMap;

    void insertToMap(shared_ptr<StringMap> map, string key, string value);
    void insertToMap(shared_ptr<StringMap> map, string key);
};
