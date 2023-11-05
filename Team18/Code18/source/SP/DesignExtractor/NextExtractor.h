#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

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
    std::unordered_set<std::string> extract(shared_ptr<ProcessedStmtList> processedStmtList, std::unordered_set<std::string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedStmt> processedStmt, std::unordered_set<std::string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile, std::unordered_set<std::string>& prevStatementNumbers) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf, std::unordered_set<std::string>& prevStatementNumbers) override;

    void insertKeyToAbstractionMap(string key);

    // This method is used to assist with the extraction of the abstraction
    void traverse(vector<shared_ptr<ProcessedStmt>> Statements);
};