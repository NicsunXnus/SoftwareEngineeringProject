#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "Entity.h"
#include "EntityExtractor.h"
#include "ModifiesAbstractionExtractor.h"
#include "UsesAbstractionExtractor.h"
#include "ParentsAbstractionExtractor.h"
#include "FollowsAbstractionExtractor.h"
#include "../AST/ASTNode.h"
#include "../PKB.h"


/**
* This class represents the Design Extractor component of the SPA.
*/
class DesignExtractor {
public:
    // Constructor
    DesignExtractor() {
        this->entityExtractor = make_shared<EntityExtractor>();
    }

    void extractEntities(shared_ptr<ASTNode> astNode) {
        entityExtractor->extractDesigns(astNode);
    }

    // Method to get procedure entity
    shared_ptr<ProcedureEntity> getProcedureEntity() {
        return this->entityExtractor->getProcedureEntity();
    }

    // Method to get statement entity
    shared_ptr<StatementEntity> getStatementEntity() {
        return this->entityExtractor->getStatementEntity();
    }

    // Method to get variable entity
    shared_ptr<VariableEntity> getVariableEntity() {
        return this->entityExtractor->getVariableEntity();
    }

    // Method to get constant entity
    shared_ptr<ConstantEntity> getConstantEntity() {
        return this->entityExtractor->getConstantEntity();
    }
    
    void insertEntities() {
        // Get the entity maps
        static shared_ptr<StringMap> statementMap = getStatementEntity()->getMap();
        static shared_ptr<StringMap> procedureMap = getProcedureEntity()->getMap();
        static shared_ptr<StringMap> variableMap = getVariableEntity()->getMap();
        static shared_ptr<StringMap> constantMap = getConstantEntity()->getMap();
        
        // Insert the entities into the PKB
        PKB::insertor.addEntity(statementMap);
        PKB::insertor.addEntity(procedureMap, PROCEDURE);
        PKB::insertor.addEntity(variableMap, VARIABLE);
        PKB::insertor.addEntity(constantMap, CONSTANT);  
    }

private:
    shared_ptr<EntityExtractor> entityExtractor;
};
