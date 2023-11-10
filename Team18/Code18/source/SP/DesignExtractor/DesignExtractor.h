#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "EntityExtractor.h"
#include "ParentExtractor.h"
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
      parentsExtractor(make_shared<ParentExtractor>()),
      followsExtractor(make_shared<FollowsExtractor>()),
      usesExtractor(make_shared<UsesExtractor>()),
      modifiesExtractor(make_shared<ModifiesExtractor>()),
      callsExtractor(make_shared<CallsExtractor>()),
      nextExtractor(make_shared<NextExtractor>()),
      procedureLineNumberExtractor(make_shared<ProcedureLineNumberExtractor>()) {}

    void extractAndInsertAll(shared_ptr<ProcessedProgram> processedProgram, bool useMultithread = false) {
        if (useMultithread) {
            extractAbstractions(processedProgram, true);
        } else {
            extractAbstractions(processedProgram, false);
        }
        extractEntities(processedProgram);
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
    shared_ptr<StringMap> getProcedureSetMap();
    shared_ptr<StringMap> getStatementSetMap();
    shared_ptr<StringMap> getVariableSetMap();
    shared_ptr<StringMap> getConstantSetMap();
    shared_ptr<StringMap> getCallProcNameMap();
    shared_ptr<StringMap> getReadVarNameMap();
    shared_ptr<StringMap> getPrintVarNameMap();
    shared_ptr<unordered_map<string, shared_ptr<Node>>> getPatternMap();
    shared_ptr<StringMap> getParentMap();
    shared_ptr<StringMap> getParentStarMap();
    shared_ptr<StringMap> getFollowsMap();
    shared_ptr<StringMap> getFollowsStarMap();
    shared_ptr<StringMap> getUsesMap();
    shared_ptr<StringMap> getModifiesMap();
    shared_ptr<StringMap> getCallsMap();
    shared_ptr<StringMap> getCallsStarMap();
    shared_ptr<StringMap> getNextMap();
    shared_ptr<unordered_map<string, pair<string, string>>>
    getProcedureStatementStorageMap();

    void extractEntities(shared_ptr<ProcessedProgram> processedProgram);
    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram, bool useMultithread = false);
    
private:
    shared_ptr<EntityExtractor> entityExtractor;
    shared_ptr<ParentExtractor> parentsExtractor;
    shared_ptr<FollowsExtractor> followsExtractor;
    shared_ptr<UsesExtractor> usesExtractor;
    shared_ptr<ModifiesExtractor> modifiesExtractor;
    shared_ptr<CallsExtractor> callsExtractor;
    shared_ptr<NextExtractor> nextExtractor;
    shared_ptr<ProcedureLineNumberExtractor> procedureLineNumberExtractor;

    void insertEntities();

    void insertAbstractions();
};
