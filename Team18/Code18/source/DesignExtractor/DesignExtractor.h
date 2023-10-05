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
#include "CallsAbstractionExtractor.h"
#include "WithExtractor.h"
#include "../AST/ASTNode.h"
#include "../PKB.h"
#include "../PKB/Adapter.h"



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
        this->callsExtractor = make_shared<CallsAbstractionExtractor>();
    }

    void extractAndInsertAll(shared_ptr<ASTNode> astNode) {
        extractAll(astNode);
        insertAll();
    }

    // Method to extract all the abstractions
    void extractAbstractions(shared_ptr<ASTNode> astNode) {
        modifiesExtractor->extractDesigns(astNode);
        usesExtractor->extractDesigns(astNode);
        parentsExtractor->extractDesigns(astNode);
        followsExtractor->extractDesigns(astNode);
    }

    // Method to extract all the entities
    void extractEntities(shared_ptr<ASTNode> astNode) {
        entityExtractor->extractDesigns(astNode);
    }

    void extractWith(shared_ptr<ASTNode> astNode) {
        withExtractor->extractDesigns(astNode);
    }

    shared_ptr<WithExtractor> getWithExtractor() {
        return this->withExtractor;
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
    

private:
    shared_ptr<EntityExtractor> entityExtractor;
    shared_ptr<ModifiesAbstractionExtractor> modifiesExtractor;
    shared_ptr<UsesAbstractionExtractor> usesExtractor;
    shared_ptr<ParentsAbstractionExtractor> parentsExtractor;
    shared_ptr<FollowsAbstractionExtractor> followsExtractor;
    shared_ptr<CallsAbstractionExtractor> callsExtractor;
    shared_ptr<WithExtractor> withExtractor;


    void extractAll(shared_ptr<ASTNode> astNode) {
        extractEntities(astNode);
        extractAbstractions(astNode);
    }


    void insertAll() {
        insertEntities();
        insertAbstractions();
    }

    // Method to insert the entities into the PKB
    void insertEntities() {
        // Get the entity maps
        static shared_ptr<StringMap> statementMap = getStatementEntity()->getMap();
        static shared_ptr<StringMap> procedureMap = getProcedureEntity()->getMap();
        static shared_ptr<StringMap> variableMap = getVariableEntity()->getMap();
        static shared_ptr<StringMap> constantMap = getConstantEntity()->getMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> statementSet = convertVectorToUnorderedSet(statementMap);
        static shared_ptr<map<string, unordered_set<string>>> procedureSet = convertVectorToUnorderedSet(procedureMap);
        static shared_ptr<map<string, unordered_set<string>>> variableSet = convertVectorToUnorderedSet(variableMap);
        static shared_ptr<map<string, unordered_set<string>>> constantSet = convertVectorToUnorderedSet(constantMap);
        
        // Insert the entities into the PKB
        // TODO: Change to new API
        PKB::insertor.addEntity(statementMap);
        PKB::insertor.addEntity(procedureMap, PROCEDURE);
        PKB::insertor.addEntity(variableMap, VARIABLE);
        PKB::insertor.addEntity(constantMap, CONSTANT);  

    }

    // Method to insert the abstractions into the PKB
    void insertAbstractions() {
        // Get the Abstraction maps
        static shared_ptr<StringMap> modifiesMap = this->modifiesExtractor->getStorageMap();
        static shared_ptr<StringMap> usesMap = this->usesExtractor->getStorageMap();
        static shared_ptr<StringMap> parentsMap = this->parentsExtractor->getStorageMap();
        static shared_ptr<StringMap> followsMap = this->followsExtractor->getStorageMap();
        static shared_ptr<StringMap> callMap = this->callsExtractor->getStorageMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> modifiesSet = convertVectorToUnorderedSet(modifiesMap);
        static shared_ptr<map<string, unordered_set<string>>> usesSet = convertVectorToUnorderedSet(usesMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsSet = convertParentsFollowsStarToParentsFollows(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsSet = convertParentsFollowsStarToParentsFollows(followsMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsStarSet = convertVectorToUnorderedSet(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsStarSet = convertVectorToUnorderedSet(followsMap);
        static shared_ptr<map<string, unordered_set<string>>> callSet = convertVectorToUnorderedSet(callMap);

        // Insert the abstractions into the PKB
        // TODO: Change to new API
        PKB::insertor.addAbstraction(modifiesMap, MODIFIES);
        PKB::insertor.addAbstraction(usesMap, USES);
        PKB::insertor.addAbstraction(parentsMap, PARENT);
        PKB::insertor.addAbstraction(followsMap, FOLLOWS);
        PKB::insertor.addAbstraction(callMap, CALLS);
    }
};
