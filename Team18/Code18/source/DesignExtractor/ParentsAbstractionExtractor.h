#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "ParentsFollowsAbstractionBaseExtractor.h"

class ParentsAbstractionExtractor : public ParentsFollowsAbstractionBaseExtractor {
public:
    void handleNode(shared_ptr<ProcedureNode> procedureNode, shared_ptr<IfNode> ifNode, shared_ptr<WhileNode> whileNode) override {
        vector<shared_ptr<StatementNode>> statements;
            int parentStatementNumber = -1;

            if (procedureNode) {
                statements = procedureNode->getStatements();
            }
            else if (ifNode) {
                std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
                std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
                statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
                statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
                parentStatementNumber = ifNode->getStatementNumber();
            }
            else if (whileNode) {
                statements = whileNode->getStatements();
                parentStatementNumber = whileNode->getStatementNumber();
            }
            
            for (const auto& statement : statements) {
                if (parentStatementNumber != -1) {
                    insertToMapFront(to_string(statement->getStatementNumber()), to_string(parentStatementNumber));
                    
                    // Append Grandparents
                    vector<string> grandParents = getValueFromMap(to_string(parentStatementNumber));
                    for (const auto& grandParent : grandParents) {
                        insertToMap(to_string(statement->getStatementNumber()), grandParent);
                    }

                }
                
                extractDesigns(statement);
            }
    }

};
