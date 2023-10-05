#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

#include "../AST/ASTNode.h"
#include "AbstractionExtractor.h"

/**
 * This class is used to extract the Calls abstraction from the AST.
 */
class CallsAbstractionExtractor : public AbstractionExtractor {
public:    
    void handleAssign(std::shared_ptr<AssignNode> assignNode) override {}
    void handleCall(std::shared_ptr<CallNode> callNode) override {}
    void handleRead(std::shared_ptr<ReadNode> readNode) override {}
    void handlePrint(std::shared_ptr<PrintNode> printNode) override {}
    void handleExpr(std::shared_ptr<ExprNode> exprNode) override {}
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) override {}

    void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) override {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        string prevStatementNumber;
        for (const auto& statement : statements) {
            string statementNumber = to_string(statement->getStatementNumber());
            if (prevStatementNumber != "") {
                insertToAbstractionMap(prevStatementNumber, statementNumber);
            }
            statementNumber = prevStatementNumber;
            extractDesigns(statement);
        }
    }

    void handleWhile(std::shared_ptr<WhileNode> whileNode) override {
        
    }

    void handleIf(std::shared_ptr<IfNode> ifNode) override {
        
    }



};