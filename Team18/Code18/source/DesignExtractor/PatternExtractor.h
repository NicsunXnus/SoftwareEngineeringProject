#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"

class PatternExtractor : public Extractor {
public:
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {
        string statementNumber = to_string(assignNode->getStatementNumber());
        insertToMap(statementNumber, assignNode);
    }

    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        string statementNumber = to_string(whileNode->getStatementNumber());
        insertToMap(statementNumber, whileNode);
    }

    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        string statementNumber = to_string(ifNode->getStatementNumber());
        insertToMap(statementNumber, ifNode);
    }

    // Get map of patternMap
    std::shared_ptr<map<string, std::shared_ptr<ASTNode>>> getPatternMap() {
        return this->patternMap;
    }

private:
    std::shared_ptr<map<string, std::shared_ptr<ASTNode>>> patternMap;

    void insertToMap(string statementNumber, std::shared_ptr<ASTNode> patternNode) {
        if (patternMap->find(statementNumber) == patternMap->end()) {
            patternMap->insert({ statementNumber, patternNode });
        }
    }

};