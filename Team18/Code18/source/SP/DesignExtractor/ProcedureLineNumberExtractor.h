#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <unordered_map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../AST/Node.h"
#include "Extractor.h"

using namespace std;

// This class is used to extract a map that contains the procedure name to the min and max values of the statement numbers
// Along with methods to determine the procedure name given a statement number
// This assists the extraction of Uses, Modifies and Next abstractions
class ProcedureLineNumberExtractor : public Extractor {
public:
    // Constructor
    ProcedureLineNumberExtractor() : procedureStatementStorageMap(make_shared<unordered_map<string, pair<string, string>>>()) {}
   
    // Get procedureStatementStorageMap
    std::shared_ptr<unordered_map<string, pair<string, string>>> getProcedureStatementStorageMap() {
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
    std::shared_ptr<unordered_map<string, pair<string, string>>> procedureStatementStorageMap;

    
};