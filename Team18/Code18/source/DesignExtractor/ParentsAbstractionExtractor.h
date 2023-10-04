#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"
#include "ParentsFollowsAbstractionBaseExtractor.h"

/**
 * This class is used to extract the parents abstraction from the AST.
 */
class ParentsAbstractionExtractor : public ParentsFollowsAbstractionBaseExtractor {
public:
    // Overriden handleNode to handle the different types of nodes, its use is found in the ParentsFollowsAbstractionBaseExtractor base class
    // Extracts the parents and grandparents of nodes
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
                // Add the statement number to the ParentsStorageMap only if there is a parent
                if (parentStatementNumber != -1) {
                    insertToMapFront(to_string(statement->getStatementNumber()), to_string(parentStatementNumber));
                    
                    // Append Grandparents
                    vector<string> grandParents = getValueFromMap(to_string(parentStatementNumber));
                    for (const auto& grandParent : grandParents) {
                        insertToAbstractionMap(to_string(statement->getStatementNumber()), grandParent);
                    }

                }
                
                extractDesigns(statement);
            }
    }

};
