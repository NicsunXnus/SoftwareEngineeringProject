#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "UsesModifiesAbstractionBaseExtractor.h"

class ModifiesAbstractionExtractor : public UsesModifiesAbstractionBaseExtractor {
public:
    // Override handlePrint to ignore print statements
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}

    // Override handleAssign to consider the LHS of the assignment statement
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {
        extractDesigns(assignNode->getVar());
    }
};
