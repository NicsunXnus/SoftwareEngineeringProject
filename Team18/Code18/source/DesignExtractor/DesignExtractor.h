#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "EntityExtractor.h"
#include "ModifiesAbstractionExtractor.h"
#include "UsesAbstractionExtractor.h"
#include "ParentsAbstractionExtractor.h"
#include "FollowsAbstractionExtractor.h"
#include "../AST/ASTNode.h"
#include "../PKB.h"



/**
* This class represents the Design Extractor component of the SPA. It is used to extract all the entities and abstractions from the AST.
*/
class DesignExtractor {
public:
    // Constructor
    DesignExtractor() {
        this->entityExtractor = make_shared<EntityExtractor>();
        this->modifiesExtractor = make_shared<ModifiesAbstractionExtractor>();
        this->usesExtractor = make_shared<UsesAbstractionExtractor>();
        this->parentsExtractor = make_shared<ParentsAbstractionExtractor>();
        this->followsExtractor = make_shared<FollowsAbstractionExtractor>();
    }

    // Method to extract all the entities
    void extractEntities(shared_ptr<ASTNode> astNode) {
        entityExtractor->extractDesigns(astNode);
    }

    // Method to extract all the abstractions
    void extractAbstractions(shared_ptr<ASTNode> astNode) {
        modifiesExtractor->extractDesigns(astNode);
        usesExtractor->extractDesigns(astNode);
        parentsExtractor->extractDesigns(astNode);
        followsExtractor->extractDesigns(astNode);
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
    
    // Method to insert the entities into the PKB
    void insertEntities() {
        // Get the entity maps
        static shared_ptr<map<string, vector<string>>> statementMap = getStatementEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> procedureMap = getProcedureEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> variableMap = getVariableEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> constantMap = getConstantEntity()->getMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> statementSet = PKB::adapter.convertVectorToUnorderedSet(statementMap);
        static shared_ptr<map<string, unordered_set<string>>> procedureSet = PKB::adapter.convertVectorToUnorderedSet(procedureMap);
        static shared_ptr<map<string, unordered_set<string>>> variableSet = PKB::adapter.convertVectorToUnorderedSet(variableMap);
        static shared_ptr<map<string, unordered_set<string>>> constantSet = PKB::adapter.convertVectorToUnorderedSet(constantMap);
        
        // Insert the entities into the PKB
        // TODO: Change to new API
        PKB::insertor.addEntity(statementSet);
        PKB::insertor.addEntity(procedureSet, PROCEDURE);
        PKB::insertor.addEntity(variableSet, VARIABLE);
        PKB::insertor.addEntity(constantSet, CONSTANT);

    }

    // Method to insert the abstractions into the PKB
    void insertAbstractions() {
        // Get the Abstraction maps
        static shared_ptr<map<string, vector<string>>> modifiesMap = this->modifiesExtractor->getStorageMap();
        static shared_ptr<map<string, vector<string>>> usesMap = this->usesExtractor->getStorageMap();
        static shared_ptr<map<string, vector<string>>> parentsMap = this->parentsExtractor->getStorageMap();
        static shared_ptr<map<string, vector<string>>> followsMap = this->followsExtractor->getStorageMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> modifiesSet = PKB::adapter.convertVectorToUnorderedSet(modifiesMap);
        static shared_ptr<map<string, unordered_set<string>>> usesSet = PKB::adapter.convertVectorToUnorderedSet(usesMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsSet = PKB::adapter.convertParentsFollowsStarToParentsFollows(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsSet = PKB::adapter.convertParentsFollowsStarToParentsFollows(followsMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsStarSet = PKB::adapter.convertVectorToUnorderedSet(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsStarSet = PKB::adapter.convertVectorToUnorderedSet(followsMap);

        // Insert the abstractions into the PKB
        PKB::insertor.addAbstraction(modifiesSet, MODIFIES);
        PKB::insertor.addAbstraction(usesSet, USES);
        PKB::insertor.addAbstraction(parentsSet, PARENT);
        PKB::insertor.addAbstraction(followsSet, FOLLOWS);
        PKB::insertor.addAbstraction(parentsStarSet, PARENTSTAR);
        PKB::insertor.addAbstraction(followsStarSet, FOLLOWSSTAR);
    }


private:
    shared_ptr<EntityExtractor> entityExtractor;
    shared_ptr<ModifiesAbstractionExtractor> modifiesExtractor;
    shared_ptr<UsesAbstractionExtractor> usesExtractor;
    shared_ptr<ParentsAbstractionExtractor> parentsExtractor;
    shared_ptr<FollowsAbstractionExtractor> followsExtractor;
};
