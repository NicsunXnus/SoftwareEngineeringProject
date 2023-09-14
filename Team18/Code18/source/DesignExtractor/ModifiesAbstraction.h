#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "../AST/ASTNode.h"


class ModifiesAbstraction {
public:

    void extractModifiesAbstraction(shared_ptr<ASTNode> astNode) {
        // Extract the entities based on the type of astNode
        if (auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode)) {
            // astNode is of type ProgramNode
            std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
            for (const auto& procedure : procedures) {
                extractModifiesAbstraction(procedure);
            }
        } 
        else if (auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode)) {
            std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
            for (const auto& statement : statements) {
                extractModifiesAbstraction(statement);
            }
        } 
        else if (auto statementNode = std::dynamic_pointer_cast<StatementNode>(astNode)) {
            // astNode is of type StatementNode
            handleStatement(statementNode);
        } 
        else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(astNode)) {
            // astNode is of type VariableNode
            insertToUsesStorageMap(variableNode->getName(), to_string(variableNode->getStatementNumber()));
        } 
        else if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(astNode)) {
            // astNode is of type ExprNode
            std::shared_ptr<ExprNode> leftExpr = exprNode->getLeftExpr();
            std::shared_ptr<ExprNode> rightExpr = exprNode->getRightExpr();
            extractModifiesAbstraction(leftExpr);
            extractModifiesAbstraction(rightExpr);
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
            extractModifiesAbstraction(readNode->getVar());
        } 
        else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(statementNode)) {
            
        }
        else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(statementNode)) {
            extractModifiesAbstraction(assignNode->getVar());
        }
        // else if (auto callNode = std::dynamic_pointer_cast<CallNode>(statementNode)) {
        //     //TODO: Handle callNode
        // } 
        else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statementNode)) {
            std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
            std::vector<int> nestedStatements = vector<int>();
            for (const auto& statement : statements) {
                // Add the statement number to the vector
                nestedStatements.push_back(statement->getStatementNumber());

                extractModifiesAbstraction(statement);
            }
            // if any of the nestedStatement values can be found in the ModifiesStorageMap, add the whileNode statement number to the ModifiesStorageMap
            for (const auto& nestedStatement : nestedStatements) {
                for (const auto& [variable, values] : *this->ModifiesStorageMap) {
                    if (std::find(values.begin(), values.end(), to_string(nestedStatement)) != values.end()) {
                        insertToUsesStorageMap(variable, to_string(whileNode->getStatementNumber()));
                    }
                }
            }
        } 
        else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statementNode)) {
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            std::vector<std::shared_ptr<StatementNode>> statements;
            std::vector<int> nestedStatements = vector<int>();

            statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
            statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
            
            for (const auto& statement : statements) {
                // Add the statement number to the vector
                nestedStatements.push_back(statement->getStatementNumber());
                
                extractModifiesAbstraction(statement);
            }
            // if any of the nestedStatement values can be found in the ModifiesStorageMap, add the ifNode statement number to the ModifiesStorageMap
            for (const auto& nestedStatement : nestedStatements) {
                for (const auto& [variable, values] : *this->ModifiesStorageMap) {
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
            extractModifiesAbstraction(leftExpr);
            extractModifiesAbstraction(rightExpr);
        }
        else if (auto notNode = std::dynamic_pointer_cast<NotNode>(condExprNode)) {
            // Get the expression
            std::shared_ptr<CondExprNode> condExpr = notNode->getLeftCondExpr();
            extractModifiesAbstraction(condExpr);
        }
        else if (auto orNode = std::dynamic_pointer_cast<OrNode>(condExprNode)) {
            // Cast succeeded, it's an OrNode
            std::shared_ptr<CondExprNode> leftCondExpr = orNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = orNode->getRightCondExpr();
            extractModifiesAbstraction(leftCondExpr);
            extractModifiesAbstraction(rightCondExpr);
        }
        else if (auto andNode = std::dynamic_pointer_cast<AndNode>(condExprNode)) {
            // Cast succeeded, it's an AndNode
            std::shared_ptr<CondExprNode> leftCondExpr = andNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = andNode->getRightCondExpr();
            extractModifiesAbstraction(leftCondExpr);
            extractModifiesAbstraction(rightCondExpr);
        }
        else {
            std::cerr << "Unsupported ASTNode type in handleCondExpr." << std::endl;
        }
    }

    void addProcedureNames(std::shared_ptr<map<string, vector<string>>> procedureMap) {
        // For all keys in ModifiesStorageMap, check the values in the string vector, if any of them matches a procedure name, add the procedure name to the ModifiesStorageMap
        for (const auto& [variable, values] : *this->ModifiesStorageMap) {
            for (const auto& [procedureName, procedureValues] : *procedureMap) {
                if (std::find(values.begin(), values.end(), procedureName) != values.end()) {
                    insertToUsesStorageMap(variable, procedureName);
                }
            }
        }       
    }

    void runModifiesExtraction(shared_ptr<ASTNode> astNode, std::shared_ptr<map<string, vector<string>>> procedureMap) {
        extractModifiesAbstraction(astNode);
        addProcedureNames(procedureMap);
    }

    // Method to get ModifiesStorageMap
    std::shared_ptr<map<string, vector<string>>> getModifiesStorageMap() {
        return this->ModifiesStorageMap;
    }

private:
    std::shared_ptr<map<string, vector<string>>> ModifiesStorageMap;

    // insert to ModifiesStorageMap
    void insertToUsesStorageMap(string variable, string value) {
        if (this->ModifiesStorageMap->find(variable) == this->ModifiesStorageMap->end()) {
            (*this->ModifiesStorageMap)[variable] = std::vector<std::string>();
        }
        (*this->ModifiesStorageMap)[variable].push_back(value);
    }
};