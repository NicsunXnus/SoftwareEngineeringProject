#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../AST/Node.h"
#include "Extractor.h"

using namespace std;


/**
 * This class is used as the base class to all abstraction extractors. It is used to extract the designs from the AST.
 */
class AbstractionExtractor : public Extractor {
public:
    // Constructor
    AbstractionExtractor() : 
    AbstractionStorageMap(make_shared<StringMap>()) {}

    shared_ptr<StringMap> getStorageMap();

    // Virtual function for individual extractors to perform pre/post processing on top of the base class's extract method
    virtual void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram);
    void extract(shared_ptr<ProcessedProgram> processedProgram) override;
    void extract(shared_ptr<ProcessedProcedure> processedProcedure) override;
    void extract(shared_ptr<ProcessedStmtList> processedStmtList) override;
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
    
    void insertToAbstractionMap(string key, string value);
    void insertIntoMap(string key, string statementNumber, shared_ptr<StringMap> map);

protected:
    shared_ptr<StringMap> AbstractionStorageMap;  
    
};
