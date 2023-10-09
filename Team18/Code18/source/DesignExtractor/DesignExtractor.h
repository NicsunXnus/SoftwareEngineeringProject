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
#include "CallsAbstractionExtractor.h"
#include "NextAbstractionExtractor.h"
#include "WithExtractor.h"
#include "PatternExtractor.h"
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
        this->withExtractor = make_shared<WithExtractor>();
        this->patternExtractor = make_shared<PatternExtractor>();
        this->nextExtractor = make_shared<NextAbstractionExtractor>();

    }

    void extractAndInsertAll(shared_ptr<ASTNode> astNode) {
        extractAll(astNode);
        insertAll();
    }

    // Method to extract all the abstractions
    void extractAllAbstractions(shared_ptr<ASTNode> astNode) {
        modifiesExtractor->extractAbstractions(astNode);
        usesExtractor->extractAbstractions(astNode);
        parentsExtractor->extractAbstractions(astNode);
        followsExtractor->extractAbstractions(astNode);
        callsExtractor->extractAbstractions(astNode);
        nextExtractor->extractAbstractions(astNode);
    }

    // Method to extract all the entities
    void extractAllEntities(shared_ptr<ASTNode> astNode) {
        entityExtractor->extractDesigns(astNode);
    }

    void extractAllWith(shared_ptr<ASTNode> astNode) {
        withExtractor->extractDesigns(astNode);
    }

    void extractPattern(shared_ptr<ASTNode> astNode) {
        patternExtractor->extractDesigns(astNode);
    }

    void extractCalls(shared_ptr<ASTNode> astNode) {
        callsExtractor->extractAbstractions(astNode);
    }

    shared_ptr<WithExtractor> getWithExtractor() {
        return this->withExtractor;
    }

    shared_ptr<PatternExtractor> getPatternExtractor() {
        return this->patternExtractor;
    }

    shared_ptr<CallsAbstractionExtractor> getCallsExtractor() {
        return this->callsExtractor;
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
    shared_ptr<PatternExtractor> patternExtractor;
    shared_ptr<NextAbstractionExtractor> nextExtractor;


    void extractAll(shared_ptr<ASTNode> astNode) {
        extractAllEntities(astNode);
        extractAllAbstractions(astNode);
        extractAllWith(astNode);
        // extractPattern(astNode);
    }


    void insertAll() {
        insertEntities();
        insertAbstractions();
        insertWiths();
        // insertPatterns();
    }

    // Method to insert the entities into the PKB
    void insertEntities() {
        // Get the entity maps
        static shared_ptr<map<string, vector<string>>> statementMap = getStatementEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> procedureMap = getProcedureEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> variableMap = getVariableEntity()->getMap();
        static shared_ptr<map<string, vector<string>>> constantMap = getConstantEntity()->getMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> statementSet = convertVectorToUnorderedSet(statementMap);
        static shared_ptr<map<string, unordered_set<string>>> procedureSet = convertVectorToUnorderedSet(procedureMap);
        static shared_ptr<map<string, unordered_set<string>>> variableSet = convertVectorToUnorderedSet(variableMap);
        static shared_ptr<map<string, unordered_set<string>>> constantSet = convertVectorToUnorderedSet(constantMap);
        
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
        static shared_ptr<map<string, vector<string>>> callMap = this->callsExtractor->getStorageMap();
        static shared_ptr<map<string, vector<string>>> nextMap = this->nextExtractor->getStorageMap();

        // Convert the maps to unordered sets
        static shared_ptr<map<string, unordered_set<string>>> modifiesSet = convertVectorToUnorderedSet(modifiesMap);
        static shared_ptr<map<string, unordered_set<string>>> usesSet = convertVectorToUnorderedSet(usesMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsSet = convertAbstractionStarToAbstraction(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsSet = convertAbstractionStarToAbstraction(followsMap);
        static shared_ptr<map<string, unordered_set<string>>> parentsStarSet = convertVectorToUnorderedSet(parentsMap);
        static shared_ptr<map<string, unordered_set<string>>> followsStarSet = convertVectorToUnorderedSet(followsMap);
        static shared_ptr<map<string, unordered_set<string>>> callSet = convertAbstractionStarToAbstraction(callMap);
        static shared_ptr<map<string, unordered_set<string>>> callStarSet = convertVectorToUnorderedSet(callMap);
        static shared_ptr<map<string, unordered_set<string>>> nextSet = convertVectorToUnorderedSet(nextMap);

        // Insert the abstractions into the PKB
        PKB::insertor.addAbstraction(modifiesSet, MODIFIES);
        PKB::insertor.addAbstraction(usesSet, USES);
        PKB::insertor.addAbstraction(parentsSet, PARENT);
        PKB::insertor.addAbstraction(followsSet, FOLLOWS);
        PKB::insertor.addAbstraction(parentsStarSet, PARENTSTAR);
        PKB::insertor.addAbstraction(followsStarSet, FOLLOWSSTAR);
        PKB::insertor.addAbstraction(callSet, CALLS);
        PKB::insertor.addAbstraction(callStarSet, CALLSSTAR);
        PKB::insertor.addAbstraction(nextSet, NEXT);
    }

    void insertWiths() {
        // Get the With maps
        static shared_ptr<map<string, unordered_set<string>>> callProcNameMap = this->withExtractor->getCallProcNameMap();
        static shared_ptr<map<string, unordered_set<string>>> readVarNameMap = this->withExtractor->getReadVarNameMap();
        static shared_ptr<map<string, unordered_set<string>>> printVarNameMap = this->withExtractor->getPrintVarNameMap();

        // Insert the Withs into the PKB
        PKB::insertor.addEntityNames(callProcNameMap, CALL);
        PKB::insertor.addEntityNames(readVarNameMap, READ);
        PKB::insertor.addEntityNames(printVarNameMap, PRINT);
    }

    void insertPatterns() {
        // TODO
    }
};
