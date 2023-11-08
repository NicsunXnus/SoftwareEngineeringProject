#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "../AST/Node.h"
#include "AbstractionExtractor.h"

using namespace std;

class ProcessedProgram;
class ProcessedStmtList;
class ProcessedStmt;
class ProcessedWhileStmt;
class ProcessedIfStmt;

class NextExtractor : public AbstractionExtractor {
public:
    // Constructor
    NextExtractor() : AbstractionExtractor() {}

    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList) override;
    unordered_set<string> extract(shared_ptr<ProcessedStmtList> processedStmtList, unordered_set<string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedStmt> processedStmt, unordered_set<string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile, unordered_set<string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf, unordered_set<string>& prevStatementNumbers) override;

    void insertKeyToAbstractionMap(string key);
};