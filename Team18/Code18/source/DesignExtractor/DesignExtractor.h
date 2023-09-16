#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Entity.h"
#include "../AST/ASTNode.h"
#include "../PKB.h"

/**
* This class represents the Design Extractor component of the SPA.
*/
class DesignExtractor {
public:
    // Constructor creates the entity objects
    DesignExtractor() {
        this->procedureEntity = make_shared<ProcedureEntity>();
        this->statementEntity = make_shared<StatementEntity>();
        this->variableEntity = make_shared<VariableEntity>();
        this->constantEntity = make_shared<ConstantEntity>();
        this->patternStatementEntity = make_shared<PatternStatementEntity>();
    }

    // Extracts the entities from the AST
    void extractEntities(shared_ptr<ASTNode> astNode) {
        // Extract the entities based on the type of astNode
        if (auto programNode = dynamic_pointer_cast<ProgramNode>(astNode)) {
            // astNode is of type ProgramNode
            // Extract the procedures from the ProgramNode
            vector<shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
            for (const auto& procedure : procedures) {
                extractEntities(procedure);
            }
        } 
        else if (auto procedureNode = dynamic_pointer_cast<ProcedureNode>(astNode)) {
            // astNode is of type ProcedureNode
            this->procedureEntity->extractEntity(procedureNode);
            vector<shared_ptr<StatementNode>> statements = procedureNode->getStatements();
            for (const auto& statement : statements) {
                extractEntities(statement);
            }
        } 
        // else if (auto assignNode = dynamic_pointer_cast<AssignNode>(astNode)) {
        //     // astNode is of type assignNode
        //     this->patternStatementEntity->extractEntity(assignNode);     
        // } 
        else if (auto statementNode = dynamic_pointer_cast<StatementNode>(astNode)) {
            // astNode is of type StatementNode
            this->statementEntity->extractEntity(statementNode);
            handleStatement(statementNode);
        } 
        else if (auto variableNode = dynamic_pointer_cast<VariableNode>(astNode)) {
            // astNode is of type VariableNode
            this->variableEntity->extractEntity(variableNode);
        } 
        else if (auto constantNode = dynamic_pointer_cast<ConstantNode>(astNode)) {
            // astNode is of type ConstantNode
            this->constantEntity->extractEntity(constantNode);
        }
        else if (auto exprNode = std::dynamic_pointer_cast<ExprNode>(astNode)) {
            // astNode is of type ExprNode
            std::shared_ptr<ExprNode> leftExpr = exprNode->getLeftExpr();
            std::shared_ptr<ExprNode> rightExpr = exprNode->getRightExpr();
            extractEntities(leftExpr);
            extractEntities(rightExpr);
        }
        else if (auto condExprNode = std::dynamic_pointer_cast<CondExprNode>(astNode)) {
            // astNode is of type CondExprNode
            handleCondExpr(condExprNode);
        }
        else {
            // Handle other cases or report an error
            cerr << "Unsupported ASTNode type." << endl;
        }
    }

    void handleStatement(shared_ptr<StatementNode> statementNode) {        
        //Check what kind of statement it is
        if (auto readNode = dynamic_pointer_cast<ReadNode>(statementNode)) {
            extractEntities(readNode->getVar());  
        } 
        else if (auto printNode = dynamic_pointer_cast<PrintNode>(statementNode)) {
            extractEntities(printNode->getVar());
        }
        else if (auto assignNode = dynamic_pointer_cast<AssignNode>(statementNode)) {
            extractEntities(assignNode->getVar());
            extractEntities(assignNode->getExpr());
        }
        // else if (auto callNode = std::dynamic_pointer_cast<CallNode>(statementNode)) {
        //     //TODO: Handle callNode
        // } 
        else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statementNode)) {
            extractEntities(whileNode->getCondExpr());
            std::vector<std::shared_ptr<StatementNode>> statements = whileNode->getStatements();
            for (const auto& statement : statements) {
                extractEntities(statement);
            }
        } 
        else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statementNode)) {
            extractEntities(ifNode->getCondExpr());
            std::vector<std::shared_ptr<StatementNode>> ifStatements = ifNode->getStatements();
            std::vector<std::shared_ptr<StatementNode>> elseStatements = ifNode->getElseStatements();
            std::vector<std::shared_ptr<StatementNode>> statements;
            statements.insert(statements.end(), ifStatements.begin(), ifStatements.end());
            statements.insert(statements.end(), elseStatements.begin(), elseStatements.end());
            for (const auto& statement : statements) {
                extractEntities(statement);
            }
        }
        else {
            // Handle other cases or report an error
            cerr << "Unsupported ASTNode type." << endl;
        }
    }

    // Method to handle conditional expressions
    void handleCondExpr(std::shared_ptr<CondExprNode> condExprNode) {
        if (auto relExprNode = std::dynamic_pointer_cast<RelExprNode>(condExprNode)) {
            // Get the left and right expressions
            std::shared_ptr<ExprNode> leftExpr = relExprNode->getLeftRelFactor();
            std::shared_ptr<ExprNode> rightExpr = relExprNode->getRightRelFactor();

            // Extract the entities from the left and right expressions
            extractEntities(leftExpr);
            extractEntities(rightExpr);
        }
        else if (auto notNode = std::dynamic_pointer_cast<NotNode>(condExprNode)) {
            // Get the expression
            std::shared_ptr<CondExprNode> condExpr = notNode->getLeftCondExpr();
            extractEntities(condExpr);
        }
        else if (auto orNode = std::dynamic_pointer_cast<OrNode>(condExprNode)) {
            // Cast succeeded, it's an OrNode
            std::shared_ptr<CondExprNode> leftCondExpr = orNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = orNode->getRightCondExpr();
            extractEntities(leftCondExpr);
            extractEntities(rightCondExpr);
        }
        else if (auto andNode = std::dynamic_pointer_cast<AndNode>(condExprNode)) {
            // Cast succeeded, it's an AndNode
            std::shared_ptr<CondExprNode> leftCondExpr = andNode->getLeftCondExpr();
            std::shared_ptr<CondExprNode> rightCondExpr = andNode->getRightCondExpr();
            extractEntities(leftCondExpr);
            extractEntities(rightCondExpr);
        }
        else {
            std::cerr << "Unsupported ASTNode type in handleCondExpr." << std::endl;
        }

    }

    // Method to get procedure entity
    shared_ptr<ProcedureEntity> getProcedureEntity() {
        return this->procedureEntity;
    }

    // Method to get statement entity
    shared_ptr<StatementEntity> getStatementEntity() {
        return this->statementEntity;
    }

    // Method to get variable entity
    shared_ptr<VariableEntity> getVariableEntity() {
        return this->variableEntity;
    }

    // Method to get constant entity
    shared_ptr<ConstantEntity> getConstantEntity() {
        return this->constantEntity;
    }

    // // Method to get pattern statement entity
    // shared_ptr<PatternStatementEntity> getPatternStatementEntity() {
    //     return this->patternStatementEntity;
    // }

    void insertEntities() {
        // Get the entity maps
        static shared_ptr<StringMap> statementMap = this->statementEntity->getMap(); 
        static shared_ptr<StringMap> procedureMap = this->procedureEntity->getMap();
        static shared_ptr<StringMap> variableMap = this->variableEntity->getMap();
        static shared_ptr<StringMap> constantMap = this->constantEntity->getMap();
        
        // Insert the entities into the PKB
        PKB::insertor.addEntity(statementMap);
        PKB::insertor.addEntity(procedureMap, PROCEDURE);
        PKB::insertor.addEntity(variableMap, VARIABLE);
        PKB::insertor.addEntity(constantMap, CONSTANT);  
    }

private:
    shared_ptr<ProcedureEntity> procedureEntity;
    shared_ptr<StatementEntity> statementEntity;
    shared_ptr<VariableEntity> variableEntity;
    shared_ptr<ConstantEntity> constantEntity;
    shared_ptr<PatternStatementEntity> patternStatementEntity;
};
