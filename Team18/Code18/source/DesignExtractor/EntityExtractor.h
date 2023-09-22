#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "../AST/ASTNode.h"
#include "Extractor.h"
#include "Entity.h"

/**
 * This class is used to extract all the entities from the AST.
 */
class EntityExtractor : public Extractor {
private:
    shared_ptr<ProcedureEntity> procedureEntity;
    shared_ptr<StatementEntity> statementEntity;
    shared_ptr<VariableEntity> variableEntity;
    shared_ptr<ConstantEntity> constantEntity;

public:
    // Constructor creates the entity objects
    EntityExtractor() {
        this->procedureEntity = make_shared<ProcedureEntity>();
        this->statementEntity = make_shared<StatementEntity>();
        this->variableEntity = make_shared<VariableEntity>();
        this->constantEntity = make_shared<ConstantEntity>();
    }

    // Overriden method to to extract procedure names
    void preProcessProcedureNode(shared_ptr<ProcedureNode> procedureNode) override {
        this->procedureEntity->extractEntity(procedureNode);
    }

    // Overriden method to to extract statement names and line numbers
    void preProcessStatementNode(shared_ptr<StatementNode> statementNode) override {
        this->statementEntity->extractEntity(statementNode);
    }

    // Overriden method to to extract variable names and line numbers
    void handleVariable(shared_ptr<VariableNode> variableNode) override {
        this->variableEntity->extractEntity(variableNode);
    }

    // Overriden method to to extract constant names and line numbers
    void handleConstant(shared_ptr<ConstantNode> constantNode) override {
        this->constantEntity->extractEntity(constantNode);
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

};