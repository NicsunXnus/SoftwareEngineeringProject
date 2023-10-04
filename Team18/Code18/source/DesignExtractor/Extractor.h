#pragma once
#include<stdio.h>
#include <iostream>
#include <string>

using namespace std;

#include "../AST/ASTNode.h"


/**
 * This class is used as the base class to all the extractors. It is used to extract all the designs from the AST.
 * Methods are virtual so that they can be overriden by the individual extractors.
 */
class Extractor {
public:
    // Constructor
    virtual ~Extractor() = default;

    // Method to extract the designs, override if needed
    virtual void extractDesigns(shared_ptr<ASTNode> astNode) {
        // Extract the designs based on the type of astNode
        if (auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode)) {
            preProcessProgramNode();
            handleProgram(programNode);
        } 
        else if (auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode)) {
            preProcessProcedureNode(procedureNode);
            handleProcedure(procedureNode);
        }
        else if (auto statementNode = std::dynamic_pointer_cast<StatementNode>(astNode)) {
            // astNode is of type StatementNode
            preProcessStatementNode(statementNode);
            handleStatement(statementNode);
        } 
        else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(astNode)) {
            // astNode is of type VariableNode
            handleVariable(variableNode);
        }
        else if (auto constantNode = std::dynamic_pointer_cast<ConstantNode>(astNode)) {
            // astNode is of type ConstantNode
            handleConstant(constantNode);
        }
        else if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(astNode)) {
            // astNode is of type ExprNode
            handleExpr(exprNode);
        }
        else if (auto condExprNode = std::dynamic_pointer_cast<CondExprNode>(astNode)) {
            // astNode is of type CondExprNode
            handleCondExpr(condExprNode);
        }

        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type in extractUsesAbstraction." << std::endl;
        }
    }

    // Method to handle program nodes, override if needed
    virtual void handleProgram(std::shared_ptr<ProgramNode> programNode) {
        std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
        for (const auto &procedure : procedures)
        {
            extractDesigns(procedure);
        }
    }

    // Method to handle procedure nodes, override if needed
    virtual void handleProcedure(std::shared_ptr<ProcedureNode> procedureNode) {
        std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
        for (const auto& statement : statements) {
            extractDesigns(statement);
        }
    }

    // Method to handle statement nodes, override if needed
    virtual void handleStatement(std::shared_ptr<StatementNode> statementNode) {        
        //Check what kind of statement it is
        if (auto readNode = std::dynamic_pointer_cast<ReadNode>(statementNode)) {
            handleRead(readNode);
        }
        else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(statementNode)) {
            handlePrint(printNode);
        }
        else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(statementNode)) {
            handleAssign(assignNode);
        }
        else if (auto callNode = std::dynamic_pointer_cast<CallNode>(statementNode)) {
            handleCall(callNode);
        } 
        else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statementNode)) {
            handleWhile(whileNode);
        } 
        else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statementNode)) {
            handleIf(ifNode);
        } 
        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type in handleStatement." << std::endl;
        }
        
    }

    // Method to handle read statements
    virtual void handleRead(std::shared_ptr<ReadNode> readNode) {
        extractDesigns(readNode->getVar());
    }

    // Method to handle print statements
    virtual void handlePrint(std::shared_ptr<PrintNode> printNode) {
        extractDesigns(printNode->getVar());
    }

    // Method to handle assignment statements
    virtual void handleAssign(std::shared_ptr<AssignNode> assignNode) {
        extractDesigns(assignNode->getExpr());
        extractDesigns(assignNode->getVar());
    }

    // Method to handle while statements
    virtual void handleWhile(std::shared_ptr<WhileNode> whileNode) {
        extractDesigns(whileNode->getCondExpr());
        std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
        std::vector<int> nestedStatements = vector<int>();
        for (const auto& statement : statements) {
            extractDesigns(statement);
        }
    }

    // Method to handle if statements
    virtual void handleIf(std::shared_ptr<IfNode> ifNode) {
        extractDesigns(ifNode->getCondExpr());
        std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
        std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
        std::vector<std::shared_ptr<StatementNode>> statements;

        statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
        statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
        
        for (const auto& statement : statements) {
            extractDesigns(statement);
        }
    }

    // Method to handle expressions
    void handleExpr(std::shared_ptr<ExprNode> exprNode) {
        std::shared_ptr<ExprNode> leftExpr = exprNode->getLeftExpr();
        std::shared_ptr<ExprNode> rightExpr = exprNode->getRightExpr();
        extractDesigns(leftExpr);
        extractDesigns(rightExpr);
    }

    // Method to handle conditional expressions
    virtual void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) {
        if (auto relExprNode = std::dynamic_pointer_cast<RelExprNode>(condExprNode)) {
            // Get the left and right expressions
            std::shared_ptr<ExprNode> leftExpr = relExprNode->getLeftRelFactor();
            std::shared_ptr<ExprNode> rightExpr = relExprNode->getRightRelFactor();

            // Extract the entities from the left and right expressions
            extractDesigns(leftExpr);
            extractDesigns(rightExpr);
        }
        else if (auto notNode = std::dynamic_pointer_cast<NotNode>(condExprNode)) {
            // Get the expression
            std::shared_ptr<CondExprNode> condExpr = notNode->getLeftCondExpr();
            extractDesigns(condExpr);
        }
        else if (auto orNode = std::dynamic_pointer_cast<OrNode>(condExprNode)) {
            // Cast succeeded, it's an OrNode
            std::shared_ptr<CondExprNode> leftCondExpr = orNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = orNode->getRightCondExpr();
            extractDesigns(leftCondExpr);
            extractDesigns(rightCondExpr);
        }
        else if (auto andNode = std::dynamic_pointer_cast<AndNode>(condExprNode)) {
            // Cast succeeded, it's an AndNode
            std::shared_ptr<CondExprNode> leftCondExpr = andNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = andNode->getRightCondExpr();
            extractDesigns(leftCondExpr);
            extractDesigns(rightCondExpr);
        }
        
        else {
            std::cerr << "Unsupported ASTNode type in handleCondExpr." << std::endl;
        }
    }

    // Methods to be overriden if additional functionality is needed
    virtual void handleVariable(std::shared_ptr<VariableNode> variableNode) {}
    virtual void handleConstant(std::shared_ptr<ConstantNode> constantNode) {}
    virtual void preProcessProgramNode() {}
    virtual void preProcessProcedureNode(std::shared_ptr<ProcedureNode> procedureNode) {}
    virtual void preProcessStatementNode(std::shared_ptr<StatementNode> statementNode) {}
    virtual void handleCall(std::shared_ptr<CallNode> callNode) {}

};