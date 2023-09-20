#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "UsesModifiesAbstractionBaseExtractor.h"

class UsesAbstractionExtractor : public UsesModifiesAbstractionBaseExtractor {
public:
    // Override handleRead to ignore read statements
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}

    // Override handleAssign to just consider the RHS of the assignment statement
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {
        extractUsesAbstraction(assignNode->getExpr());
    }

    // Override handleWhile to consider the condition expression of the while statement
    void preProcessWhileNode(std::shared_ptr<WhileNode> whileNode) {
        extractDesigns(whileNode->getCondExpr());
    }

    // Override handleIf to consider the condition expression of the if statement
    void preProcessIfNode(std::shared_ptr<IfNode> ifNode) override {
        extractDesigns(ifNode->getCondExpr());
    }
};
