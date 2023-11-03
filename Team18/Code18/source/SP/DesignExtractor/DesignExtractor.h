#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "EntityExtractor.h"
#include "ParentsExtractor.h"
#include "FollowsExtractor.h"
#include "UsesExtractor.h"
#include "ModifiesExtractor.h"
#include "CallsExtractor.h"
#include "NextExtractor.h"
#include "ProcedureLineNumberExtractor.h"
#include "../../PKB/PKB.h"
#include "../SimpleProcessor/ProcessedProgram.h"



/**
* This class represents the Design Extractor component of the SPA. It is used to extract all the entities and abstractions from the AST.
*/
class DesignExtractor {
public:
    // Constructor
    DesignExtractor()
    : entityExtractor(make_shared<EntityExtractor>()),
      parentsExtractor(make_shared<ParentsExtractor>()),
      followsExtractor(make_shared<FollowsExtractor>()),
      usesExtractor(make_shared<UsesExtractor>()),
      modifiesExtractor(make_shared<ModifiesExtractor>()),
      callsExtractor(make_shared<CallsExtractor>()),
      nextExtractor(make_shared<NextExtractor>()),
      procedureLineNumberExtractor(make_shared<ProcedureLineNumberExtractor>()) {}

    void extractAndInsertAll(shared_ptr<ProcessedProgram> processedProgram) {
        extractEntities(processedProgram);
        extractAbstractions(processedProgram);
        insertEntities();
        insertAbstractions();
    }

    // Used for testing
    void extractParents(shared_ptr<ProcessedProgram> processedProgram);
    void extractFollows(shared_ptr<ProcessedProgram> processedProgram);
    void extractUses(shared_ptr<ProcessedProgram> processedProgram);
    void extractModifies(shared_ptr<ProcessedProgram> processedProgram);
    void extractCalls(shared_ptr<ProcessedProgram> processedProgram);
    void extractNext(shared_ptr<ProcessedProgram> processedProgram);
    shared_ptr<map<string, unordered_set<string>>> getProcedureSetMap();
    shared_ptr<map<string, unordered_set<string>>> getStatementSetMap();
    shared_ptr<map<string, unordered_set<string>>> getVariableSetMap();
    shared_ptr<map<string, unordered_set<string>>> getConstantSetMap();
    shared_ptr<map<string, unordered_set<string>>> getCallProcNameMap();
    shared_ptr<map<string, unordered_set<string>>> getReadVarNameMap();
    shared_ptr<map<string, unordered_set<string>>> getPrintVarNameMap();
    shared_ptr<map<string, shared_ptr<Node>>> getPatternMap();
    shared_ptr<map<string, unordered_set<string>>> getParentMap();
    shared_ptr<map<string, unordered_set<string>>> getParentStarMap();
    shared_ptr<map<string, unordered_set<string>>> getFollowsMap();
    shared_ptr<map<string, unordered_set<string>>> getFollowsStarMap();
    shared_ptr<map<string, unordered_set<string>>> getUsesMap();
    shared_ptr<map<string, unordered_set<string>>> getModifiesMap();
    shared_ptr<map<string, unordered_set<string>>> getCallsMap();
    shared_ptr<map<string, unordered_set<string>>> getCallsStarMap();
    shared_ptr<map<string, unordered_set<string>>> getNextMap();
    shared_ptr<map<string, pair<string, string>>> getProcedureStatementStorageMap();

    void extractEntities(shared_ptr<ProcessedProgram> processedProgram);
    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram);
    
private:
    shared_ptr<EntityExtractor> entityExtractor;
    shared_ptr<ParentsExtractor> parentsExtractor;
    shared_ptr<FollowsExtractor> followsExtractor;
    shared_ptr<UsesExtractor> usesExtractor;
    shared_ptr<ModifiesExtractor> modifiesExtractor;
    shared_ptr<CallsExtractor> callsExtractor;
    shared_ptr<NextExtractor> nextExtractor;
    shared_ptr<ProcedureLineNumberExtractor> procedureLineNumberExtractor;

    void insertEntities();

    void insertAbstractions();
};
