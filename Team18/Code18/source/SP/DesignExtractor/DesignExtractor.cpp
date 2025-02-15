#include "DesignExtractor.h"
#include <memory>
#include <thread>
#include "../../ThreadPool.h"

// ai-gen start (copilot, 2)
void DesignExtractor::extractEntities(shared_ptr<ProcessedProgram> processedProgram) {
    this->entityExtractor->extract(processedProgram);
    this->procedureLineNumberExtractor->extract(processedProgram);
}

void DesignExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram, bool useMultithread) {
    if (useMultithread) {
         // Create a ThreadPool instance
        ThreadPool threadPool;

        // Define tasks and add them to the thread pool
        threadPool.addTask(&ParentExtractor::extractAbstractions, this->parentsExtractor, processedProgram);
        threadPool.addTask(&FollowsExtractor::extractAbstractions, this->followsExtractor, processedProgram);
        threadPool.addTask(&CallsExtractor::extractAbstractions, this->callsExtractor, processedProgram);
        threadPool.addTask(&UsesExtractor::extractAbstractions, this->usesExtractor, processedProgram);
        threadPool.addTask(&ModifiesExtractor::extractAbstractions, this->modifiesExtractor, processedProgram);
        threadPool.addTask(&NextExtractor::extractAbstractions, this->nextExtractor, processedProgram);

        // Wait for all tasks to complete
        threadPool.wait();

    } else {
        // Execute the extractions sequentially
        this->parentsExtractor->extractAbstractions(processedProgram);
        this->followsExtractor->extractAbstractions(processedProgram);
        this->callsExtractor->extractAbstractions(processedProgram);
        this->usesExtractor->extractAbstractions(processedProgram);
        this->modifiesExtractor->extractAbstractions(processedProgram);
        this->nextExtractor->extractAbstractions(processedProgram);
    }
}

void DesignExtractor::extractParents(shared_ptr<ProcessedProgram> processedProgram) {
    this->parentsExtractor->extractAbstractions(processedProgram);
}

void DesignExtractor::extractFollows(shared_ptr<ProcessedProgram> processedProgram) {
    this->followsExtractor->extractAbstractions(processedProgram);
}

void DesignExtractor::extractUses(shared_ptr<ProcessedProgram> processedProgram) {
    this->usesExtractor->extractAbstractions(processedProgram);
}

void DesignExtractor::extractModifies(shared_ptr<ProcessedProgram> processedProgram) {
    this->modifiesExtractor->extractAbstractions(processedProgram);
}

void DesignExtractor::extractCalls(shared_ptr<ProcessedProgram> processedProgram) {
    this->callsExtractor->extractAbstractions(processedProgram);
}

void DesignExtractor::extractNext(shared_ptr<ProcessedProgram> processedProgram) {
    this->nextExtractor->extractAbstractions(processedProgram);
}

shared_ptr<StringMap> DesignExtractor::getProcedureSetMap() {
    return this->entityExtractor->getProcedureMap();
}
shared_ptr<StringMap> DesignExtractor::getStatementSetMap() {
    return this->entityExtractor->getStatementMap();
}
shared_ptr<StringMap> DesignExtractor::getVariableSetMap() {
    return this->entityExtractor->getVariableMap();
}
shared_ptr<StringMap> DesignExtractor::getConstantSetMap() {
    return this->entityExtractor->getConstantMap();
}
shared_ptr<StringMap> DesignExtractor::getCallProcNameMap() {
    return this->entityExtractor->getCallProcNameMap();
}
shared_ptr<StringMap> DesignExtractor::getReadVarNameMap() {
    return this->entityExtractor->getReadVarNameMap();
}
shared_ptr<StringMap> DesignExtractor::getPrintVarNameMap() {
    return this->entityExtractor->getPrintVarNameMap();
}
shared_ptr<unordered_map<string, shared_ptr<Node>>> DesignExtractor::getPatternMap() {
    return this->entityExtractor->getPatternMap();
}
shared_ptr<StringMap> DesignExtractor::getParentMap() {
    return this->parentsExtractor->getStorageMap();
}
shared_ptr<StringMap> DesignExtractor::getParentStarMap() {
    return this->parentsExtractor->getStorageStarMap();
}
shared_ptr<StringMap> DesignExtractor::getFollowsMap() {
    return this->followsExtractor->getStorageMap();
}
shared_ptr<StringMap> DesignExtractor::getFollowsStarMap() {
    return this->followsExtractor->getStorageStarMap();
}
shared_ptr<StringMap> DesignExtractor::getUsesMap() {
    return this->usesExtractor->getStorageMap();
}
shared_ptr<StringMap> DesignExtractor::getModifiesMap() {
    return this->modifiesExtractor->getStorageMap();
}
shared_ptr<StringMap> DesignExtractor::getCallsMap() {
    return this->callsExtractor->getStorageMap();
}
shared_ptr<StringMap> DesignExtractor::getCallsStarMap() {
    return this->callsExtractor->getStorageStarMap();
}
shared_ptr<StringMap> DesignExtractor::getNextMap() {
    return this->nextExtractor->getStorageMap();
}
shared_ptr<unordered_map<string, pair<string, string>>> DesignExtractor::getProcedureStatementStorageMap() {
    return this->procedureLineNumberExtractor->getProcedureStatementStorageMap();
}

void DesignExtractor::insertEntities() {
    // Get the entity maps
    static shared_ptr<StringMap> procedureSet = this->entityExtractor->getProcedureMap();
    static shared_ptr<StringMap> variableSet = this->entityExtractor->getVariableMap();
    static shared_ptr<StringMap> constantSet = this->entityExtractor->getConstantMap();
    static shared_ptr<StringMap> statementSet = this->entityExtractor->getStatementMap();
    static shared_ptr<StringMap> callProcNameMap = this->entityExtractor->getCallProcNameMap();
    static shared_ptr<StringMap> readVarNameMap = this->entityExtractor->getReadVarNameMap();
    static shared_ptr<StringMap> printVarNameMap = this->entityExtractor->getPrintVarNameMap();
    static shared_ptr<unordered_map<string, shared_ptr<Node>>> patternMap =
        this->entityExtractor->getPatternMap();
    static shared_ptr<unordered_map<string, pair<string, string>>>
        procedureStatementStorageMap = this->procedureLineNumberExtractor
                                           ->getProcedureStatementStorageMap();

    // Insert the entities into the PKB
    PKB::insertor.addEntity(statementSet);
    PKB::insertor.addEntity(procedureSet, PROCEDURE);
    PKB::insertor.addEntity(variableSet, VARIABLE);
    PKB::insertor.addEntity(constantSet, CONSTANT);
    PKB::insertor.addEntityNames(callProcNameMap, CALL);
    PKB::insertor.addEntityNames(readVarNameMap, READ);
    PKB::insertor.addEntityNames(printVarNameMap, PRINT);
    PKB::insertor.addPatterns(patternMap);
    PKB::insertor.addProcLines(procedureStatementStorageMap);
}

void DesignExtractor::insertAbstractions() {
    // Get the Abstraction maps
    static shared_ptr<StringMap>  parentsSet = this->parentsExtractor->getStorageMap();
    static shared_ptr<StringMap>  followsSet = this->followsExtractor->getStorageMap();
    static shared_ptr<StringMap>  parentsStarSet = this->parentsExtractor->getStorageStarMap();
    static shared_ptr<StringMap>  followsStarSet = this->followsExtractor->getStorageStarMap();
    static shared_ptr<StringMap>  callSet = this->callsExtractor->getStorageMap();
    static shared_ptr<StringMap>  callStarSet = this->callsExtractor->getStorageStarMap();
    static shared_ptr<StringMap> modifiesSet = this->modifiesExtractor->getStorageMap();
    static shared_ptr<StringMap> usesSet = this->usesExtractor->getStorageMap();
    static shared_ptr<StringMap> nextSet = this->nextExtractor->getStorageMap();

    // // Insert the abstractions into the PKB
    
    PKB::insertor.addAbstraction(parentsSet, PARENT);
    PKB::insertor.addAbstraction(followsSet, FOLLOWS);
    PKB::insertor.addAbstraction(parentsStarSet, PARENTSTAR);
    PKB::insertor.addAbstraction(followsStarSet, FOLLOWSSTAR);
    PKB::insertor.addAbstraction(modifiesSet, MODIFIES);
    PKB::insertor.addAbstraction(usesSet, USES);
    PKB::insertor.addAbstraction(callSet, CALLS);
    PKB::insertor.addAbstraction(callStarSet, CALLSSTAR);
    PKB::insertor.addAbstraction(nextSet, NEXT);

}
// ai-gen end
