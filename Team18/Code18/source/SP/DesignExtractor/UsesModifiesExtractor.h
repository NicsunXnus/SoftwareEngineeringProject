#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../SimpleProcessor/ProcessedCallStmt.h"
#include "../SimpleProcessor/ProcessedStmtList.h"
#include "AbstractionExtractor.h"
#include "CallsExtractor.h"

using namespace std;

// This class is the parent class of UsesExtractor and ModifiesExtractor
// It holds common methods that are used by both classes
class UsesModifiesExtractor : public AbstractionExtractor {
public:
    // Constructor
    UsesModifiesExtractor() : AbstractionExtractor(),
        callsProcedureParentProcedureMap(make_shared<map<string, unordered_set<string>>>()),
        procedureCallLinesMap(make_shared<map<string, unordered_set<string>>>()),
        ifWhileNestedStatementsMap(make_shared<map<string, unordered_set<string>>>()),
        callsExtractor(make_shared<CallsExtractor>()) {}

    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList, string parentConditionalStatementNumber);
    void extract(shared_ptr<ProcessedCallStmt> processedCallStmt) override;


    string getProcedureNameFromStatementNumber(string lineNumber);

    void insertIntoProcedureCallLinesMap(string procedureName, string statementNumber) {
        insertIntoMap(procedureName, statementNumber, procedureCallLinesMap);
    }
    void insertToIfWhileNestedStatementsMap(string statementNumber, string parentStatementNumber) {
        insertIntoMap(statementNumber, parentStatementNumber, ifWhileNestedStatementsMap);
    }
    void addStatementNumberAndProcedureName(string variableName, string statementNumber);

protected:
    // This map is used to store procedure names as the key and a vector of the procedures names that calls it
    shared_ptr<map<string, unordered_set<string>>> callsProcedureParentProcedureMap;
    // This map is used to store procedure names as the key and a vector of the statement numbers that calls it
    shared_ptr<map<string, unordered_set<string>>> procedureCallLinesMap;
    // This map is used to storeif/while statement number as the key and nested statement numbers as the value 
    shared_ptr<map<string, unordered_set<string>>> ifWhileNestedStatementsMap;
    shared_ptr<CallsExtractor> callsExtractor;

    void processIndirectProcedureCalls();
    void processNestedIfWhileStatements();
    void nestedIfWhileHelper(string childStatementNumber, shared_ptr<unordered_set<std::string>> statementNumbersToBeAdded);
    void createCallsProcedureParentProcedureMap(shared_ptr<map<string, unordered_set<string>>> callsMap);
};
