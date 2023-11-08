#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "../SimpleProcessor/ProcessedProgram.h"

#include "AbstractionStarExtractor.h"
#include "ProcedureLineNumberExtractor.h"

class ProcessedCallStmt;

using namespace std;

// This class is used to extract the calls abstraction from the AST
class CallsExtractor : public AbstractionStarExtractor {
public:
    // Constructor
    CallsExtractor() : AbstractionStarExtractor() {
        this->procedureLineNumberExtractor = make_shared<ProcedureLineNumberExtractor>();
    }
    
    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedCallStmt> processedCall) override;

    string getProcedureNameFromStatementNumber(string lineNumber);

private:
    shared_ptr<ProcedureLineNumberExtractor> procedureLineNumberExtractor;
};