#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "AbstractionStarExtractor.h"

// TO ADDRESS: nit Parent*

class ParentsExtractor : public AbstractionStarExtractor {
public:
    void extract(shared_ptr<ProcessedStmtList> processedStmtList, std::string parentStatementNumber);
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
};