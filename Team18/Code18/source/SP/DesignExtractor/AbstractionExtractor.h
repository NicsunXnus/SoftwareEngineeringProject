#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../AST/Node.h"
#include "Extractor.h"

using namespace std;

class AbstractionExtractor : public Extractor {
public:
    // Constructor
    AbstractionExtractor() : 
    AbstractionStorageMap(make_shared<map<string, unordered_set<string>>>()), 
    AbstractionStarStorageMap(make_shared<map<string, unordered_set<string>>>()) {}

    shared_ptr<map<string, unordered_set<string>>> getStorageMap();
    shared_ptr<map<string, unordered_set<string>>> getStorageStarMap();

    virtual void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram);
    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProcedure> processedProcedure) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
    
    void insertToAbstractionMap(string key, string value);
    void insertToAbstractionStarMap(string key, string value);
    void insertIntoMap(string key, string statementNumber, shared_ptr<map<string, unordered_set<string>>> map);


protected:
    std::shared_ptr<map<string, unordered_set<string>>> AbstractionStorageMap;  
    std::shared_ptr<map<string, unordered_set<string>>> AbstractionStarStorageMap;  

    void processAbstractionStar();
    void processAbstractionStarHelper(string value, unordered_set<string>& values);
};
