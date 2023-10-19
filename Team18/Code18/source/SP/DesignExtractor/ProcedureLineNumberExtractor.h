#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../AST/Node.h"
#include "Extractor.h"

using namespace std;

class ProcedureLineNumberExtractor : public Extractor {
public:
    // Constructor
    ProcedureLineNumberExtractor() : procedureStatementStorageMap(make_shared<map<string, pair<string, string>>>()) {}
   
    // Get procedureStatementStorageMap
    std::shared_ptr<map<string, pair<string, string>>> getProcedureStatementStorageMap() {
        return this->procedureStatementStorageMap;
    }

    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProcedure> processedProcedure) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList, string procedureName) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile, string procedureName) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf, string procedureName) override;


    // method to get procedure name from statement number
    string getProcedureNameFromStatementNumber(string statementNumber);

    void insertToProcedureStatementStorageMap(string procedureName, string statementNumber);

private:
    // map to store procedurename to the min and max values of the statement numbers
    std::shared_ptr<map<string, pair<string, string>>> procedureStatementStorageMap;

    
};