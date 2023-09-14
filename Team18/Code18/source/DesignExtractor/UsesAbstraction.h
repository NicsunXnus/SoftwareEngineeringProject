#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"


class UsesAbstraction {
public:
    UsesAbstraction() {
        this->UsesStorageMap = std::make_shared<map<string, vector<string>>>();
    }


    void extractUsesAbstraction(shared_ptr<ASTNode> astNode) {
        // Extract the entities based on the type of astNode
        if (auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode)) {
            // astNode is of type ProgramNode
            std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
            for (const auto& procedure : procedures) {
                extractUsesAbstraction(procedure);
            }
        } 
        else if (auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode)) {
            std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
            for (const auto& statement : statements) {
                extractUsesAbstraction(statement);
            }
        } 
        else if (auto statementNode = std::dynamic_pointer_cast<StatementNode>(astNode)) {
            // astNode is of type StatementNode
            handleStatement(statementNode);
        } 
        else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(astNode)) {
            // astNode is of type VariableNode
            string variableName = variableNode->getName();
            string statementNumber = to_string(variableNode->getStatementNumber());
            // Check if the UsesStorageMap is a null pointer
            if (this->UsesStorageMap == nullptr) {
                // Throw an error
                throw std::runtime_error("help la");
            }
            throw std::runtime_error("help l2a");

            insertToUsesStorageMap(variableName, statementNumber);
        } 
        else if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(astNode)) {
            // astNode is of type ExprNode
            std::shared_ptr<ExprNode> leftExpr = exprNode->getLeftExpr();
            std::shared_ptr<ExprNode> rightExpr = exprNode->getRightExpr();
            extractUsesAbstraction(leftExpr);
            extractUsesAbstraction(rightExpr);
        }
        else if (auto condExprNode = std::dynamic_pointer_cast<CondExprNode>(astNode)) {
            // astNode is of type CondExprNode
            handleCondExpr(condExprNode);
        }
        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type." << std::endl;
        }
    }

    void handleStatement(std::shared_ptr<StatementNode> statementNode) {        
        //Check what kind of statement it is
        if (auto readNode = std::dynamic_pointer_cast<ReadNode>(statementNode)) {

        } 
        else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(statementNode)) {
            extractUsesAbstraction(printNode->getVar());
        }
        else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(statementNode)) {
            extractUsesAbstraction(assignNode->getExpr());
        }
        // else if (auto callNode = std::dynamic_pointer_cast<CallNode>(statementNode)) {
        //     //TODO: Handle callNode
        // } 
        else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statementNode)) {
            extractUsesAbstraction(whileNode->getCondExpr());
            
            std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
            std::vector<int> nestedStatements = vector<int>();
            for (const auto& statement : statements) {
                // Add the statement number to the vector   
                nestedStatements.push_back(statement->getStatementNumber());

                extractUsesAbstraction(statement);
            }
            // if any of the nestedStatement values can be found in the UsesStorageMap, add the whileNode statement number to the UsesStorageMap
            for (const auto& nestedStatement : nestedStatements) {
                for (const auto& [variable, values] : *this->UsesStorageMap) {
                    if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                        insertToUsesStorageMap(variable, to_string(whileNode->getStatementNumber()));
                    }
                }
            }
        } 
        else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statementNode)) {
            extractUsesAbstraction(ifNode->getCondExpr());
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            std::vector<std::shared_ptr<StatementNode>> statements;
            std::vector<int> nestedStatements = vector<int>();

            statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
            statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
            
            for (const auto& statement : statements) {
                // Add the statement number to the vector
                nestedStatements.push_back(statement->getStatementNumber());
                
                extractUsesAbstraction(statement);
            }
            // if any of the nestedStatement values can be found in the UsesStorageMap, add the ifNode statement number to the UsesStorageMap
            for (const auto& nestedStatement : nestedStatements) {
                for (const auto& [variable, values] : *this->UsesStorageMap) {
                    if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                        insertToUsesStorageMap(variable, to_string(ifNode->getStatementNumber()));
                    }
                }
            }

        } 
        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type." << std::endl;
        }
        
    }

    // Method to handle conditional expressions
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) {
        if (auto relExprNode = std::dynamic_pointer_cast<RelExprNode>(condExprNode)) {
            // Get the left and right expressions
            std::shared_ptr<ExprNode> leftExpr = relExprNode->getLeftRelFactor();
            std::shared_ptr<ExprNode> rightExpr = relExprNode->getRightRelFactor();

            // Extract the entities from the left and right expressions
            extractUsesAbstraction(leftExpr);
            extractUsesAbstraction(rightExpr);
        }
        else if (auto notNode = std::dynamic_pointer_cast<NotNode>(condExprNode)) {
            // Get the expression
            std::shared_ptr<CondExprNode> condExpr = notNode->getLeftCondExpr();
            extractUsesAbstraction(condExpr);
        }
        else if (auto orNode = std::dynamic_pointer_cast<OrNode>(condExprNode)) {
            // Cast succeeded, it's an OrNode
            std::shared_ptr<CondExprNode> leftCondExpr = orNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = orNode->getRightCondExpr();
            extractUsesAbstraction(leftCondExpr);
            extractUsesAbstraction(rightCondExpr);
        }
        else if (auto andNode = std::dynamic_pointer_cast<AndNode>(condExprNode)) {
            // Cast succeeded, it's an AndNode
            std::shared_ptr<CondExprNode> leftCondExpr = andNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = andNode->getRightCondExpr();
            extractUsesAbstraction(leftCondExpr);
            extractUsesAbstraction(rightCondExpr);
        }
        else {
            std::cerr << "Unsupported ASTNode type in handleCondExpr." << std::endl;
        }
    }

    std::shared_ptr<map<string, vector<string>>> getUsesStorageMap() {
        return this->UsesStorageMap;
    }

private:
    std::shared_ptr<map<string, vector<string>>> UsesStorageMap;

    // insert to UsesStorageMap
    void insertToUsesStorageMap(string variableName, string statementNumber) {
        if (this->UsesStorageMap->find(variableName) == this->UsesStorageMap->end()) {
            this->UsesStorageMap->insert({variableName, vector<string>()});
        }
        this->UsesStorageMap->at(variableName).push_back(statementNumber);
    }
};