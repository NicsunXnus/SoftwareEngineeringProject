#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

#include "AbstractionExtractor.h"
#include "UsesModifiesExtractor.h"

using namespace std;


class UsesExtractor : public UsesModifiesExtractor {
public:
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
    void extract(shared_ptr<ProcessedAssignStmt> processedAssign) override;
    void extract(shared_ptr<ProcessedPrintStmt> processedPrint) override;
    void extract(shared_ptr<OpNode> node) override;
    void extract(shared_ptr<VariableNode> node) override;
};
