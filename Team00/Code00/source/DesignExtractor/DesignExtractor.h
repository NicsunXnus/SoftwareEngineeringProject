#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Entity.h"
#include "../AST/ASTNode.h"
// #include "../AST/ExprNode.h"
// #include "../PKB/Inserter.h"

/**
* This class represents the Design Extractor component of the SPA.
*/
class DesignExtractor {
public:
    // Constructor creates the entity objects
    DesignExtractor() {
        this->procedureEntity = std::make_shared<ProcedureEntity>();
        this->statementEntity = std::make_shared<StatementEntity>();
        this->variableEntity = std::make_shared<VariableEntity>();
        this->constantEntity = std::make_shared<ConstantEntity>();
        this->patternStatementEntity = std::make_shared<PatternStatementEntity>();
    }

    // Extracts the entities from the AST
    void extractEntities(std::shared_ptr<ASTNode> astNode) {
        // Extract the entities based on the type of astNode
        if (auto programNode = std::dynamic_pointer_cast<ProgramNode>(astNode)) {
            // astNode is of type ProgramNode
            // Extract the procedures from the ProgramNode
            std::vector<std::shared_ptr<ProcedureNode>> procedures = programNode->getProcedures();
            for (const auto& procedure : procedures) {
                extractEntities(procedure);
            }
        } 
        else if (auto procedureNode = std::dynamic_pointer_cast<ProcedureNode>(astNode)) {
            // astNode is of type ProcedureNode
            this->procedureEntity->extractEntity(procedureNode);
            std::vector<std::shared_ptr<StatementNode>> statements = procedureNode->getStatements();
            for (const auto& statement : statements) {
                extractEntities(statement);
            }
        } 
        else if (auto assignNode = std::dynamic_pointer_cast<AssignNode>(astNode)) {
            // astNode is of type assignNode
            this->patternStatementEntity->extractEntity(assignNode);     
        } 
        else if (auto statementNode = std::dynamic_pointer_cast<StatementNode>(astNode)) {
            // astNode is of type StatementNode
            this->statementEntity->extractEntity(statementNode);
            handleStatement(statementNode);
        } 
        else if (auto variableNode = std::dynamic_pointer_cast<VariableNode>(astNode)) {
            // astNode is of type VariableNode
            this->variableEntity->extractEntity(variableNode);
        } 
        else if (auto constantNode = std::dynamic_pointer_cast<ConstantNode>(astNode)) {
            // astNode is of type ConstantNode
            this->constantEntity->extractEntity(constantNode);
        } 
        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type." << std::endl;
        }
    }

    void handleStatement(std::shared_ptr<StatementNode> statementNode) {        
        //Check what kind of statement it is
        if (auto readNode = std::dynamic_pointer_cast<ReadNode>(statementNode)) {
            extractEntities(readNode->getVar());  
        } 
        else if (auto printNode = std::dynamic_pointer_cast<PrintNode>(statementNode)) {
            extractEntities(printNode->getVar());
        } 
        // else if (auto callNode = std::dynamic_pointer_cast<CallNode>(statementNode)) {
        //     //TODO: Handle callNode
        // } 
        // else if (auto whileNode = std::dynamic_pointer_cast<WhileNode>(statementNode)) {
        //     //TODO: Handle whileNode
        // } 
        // else if (auto ifNode = std::dynamic_pointer_cast<IfNode>(statementNode)) {
        //     //TODO: Handle IfNode
        // } 
        else {
            // Handle other cases or report an error
            std::cerr << "Unsupported ASTNode type." << std::endl;
        }
    }

    // void insertEntities() {
    //     // Get the entity maps
    //     std::map<std::string, std::vector<int>> procedureMap = this->procedureEntity->getMap();
    //     std::map<std::string, std::vector<int>> statementMap = this->statementEntity->getMap();
    //     std::map<std::string, std::vector<int>> variableMap = this->variableEntity->getMap();
    //     std::map<std::string, std::vector<int>> constantMap = this->constantEntity->getMap();
    //     // std::map<std::string, std::vector<int>> assignStatementMap = this->assignStatementEntity->getMap();

    //     // Insert the entities into the PKB
    //     Insertor::addProcedures(procedureMap);
    //     Insertor::addStatements(statementMap);
    //     Insertor::addVariables(variableMap);
    //     Insertor::addConstants(constantMap);
    // }

private:
    std::shared_ptr<ProcedureEntity> procedureEntity;
    std::shared_ptr<StatementEntity> statementEntity;
    std::shared_ptr<VariableEntity> variableEntity;
    std::shared_ptr<ConstantEntity> constantEntity;
    std::shared_ptr<PatternStatementEntity> patternStatementEntity;
};
