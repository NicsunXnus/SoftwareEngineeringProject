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
        //  // Create a ThreadPool instance
        // ThreadPool threadPool;

        // // Define tasks and add them to the thread pool
        // threadPool.addTask(&ParentsExtractor::extractAbstractions, this->parentsExtractor, processedProgram);
        // threadPool.addTask(&FollowsExtractor::extractAbstractions, this->followsExtractor, processedProgram);
        // threadPool.addTask(&CallsExtractor::extractAbstractions, this->callsExtractor, processedProgram);
        // threadPool.addTask(&UsesExtractor::extractAbstractions, this->usesExtractor, processedProgram);
        // threadPool.addTask(&ModifiesExtractor::extractAbstractions, this->modifiesExtractor, processedProgram);
        // threadPool.addTask(&NextExtractor::extractAbstractions, this->nextExtractor, processedProgram);

        // // Wait for all tasks to complete
        // threadPool.wait();

        std::vector<std::thread> threads;

        // Define lambda functions for each abstraction
        auto parentsThread = [&] {
            this->parentsExtractor->extractAbstractions(processedProgram);
        };

        auto followsThread = [&] {
            this->followsExtractor->extractAbstractions(processedProgram);
        };

        auto callsThread = [&] {
            this->callsExtractor->extractAbstractions(processedProgram);
        };

        auto usesThread = [&] {
            this->usesExtractor->extractAbstractions(processedProgram);
        };

        auto modifiesThread = [&] {
            this->modifiesExtractor->extractAbstractions(processedProgram);
        };

        auto nextThread = [&] {
            this->nextExtractor->extractAbstractions(processedProgram);
        };

        // Start each abstraction in a separate thread
        threads.emplace_back(parentsThread);
        threads.emplace_back(followsThread);
        threads.emplace_back(callsThread);
        threads.emplace_back(usesThread);
        threads.emplace_back(modifiesThread);
        threads.emplace_back(nextThread);

        // Join all threads to wait for them to complete
        for (auto& thread : threads) {
            thread.join();
        }

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

shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getProcedureSetMap() {
    return this->entityExtractor->getProcedureMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getStatementSetMap() {
    return this->entityExtractor->getStatementMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getVariableSetMap() {
    return this->entityExtractor->getVariableMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getConstantSetMap() {
    return this->entityExtractor->getConstantMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getCallProcNameMap() {
    return this->entityExtractor->getCallProcNameMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getReadVarNameMap() {
    return this->entityExtractor->getReadVarNameMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getPrintVarNameMap() {
    return this->entityExtractor->getPrintVarNameMap();
}
shared_ptr<map<string, shared_ptr<Node>>> DesignExtractor::getPatternMap() {
    return this->entityExtractor->getPatternMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getParentMap() {
    return this->parentsExtractor->getStorageMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getParentStarMap() {
    return this->parentsExtractor->getStorageStarMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getFollowsMap() {
    return this->followsExtractor->getStorageMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getFollowsStarMap() {
    return this->followsExtractor->getStorageStarMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getUsesMap() {
    return this->usesExtractor->getStorageMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getModifiesMap() {
    return this->modifiesExtractor->getStorageMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getCallsMap() {
    return this->callsExtractor->getStorageMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getCallsStarMap() {
    return this->callsExtractor->getStorageStarMap();
}
shared_ptr<map<string, unordered_set<string>>> DesignExtractor::getNextMap() {
    return this->nextExtractor->getStorageMap();
}
shared_ptr<map<string, pair<string, string>>> DesignExtractor::getProcedureStatementStorageMap() {
    return this->procedureLineNumberExtractor->getProcedureStatementStorageMap();
}

void DesignExtractor::insertEntities() {
    // Get the entity maps
    static shared_ptr<map<string, unordered_set<string>>> procedureSet = this->entityExtractor->getProcedureMap();
    static shared_ptr<map<string, unordered_set<string>>> variableSet = this->entityExtractor->getVariableMap();
    static shared_ptr<map<string, unordered_set<string>>> constantSet = this->entityExtractor->getConstantMap();
    static shared_ptr<map<string, unordered_set<string>>> statementSet = this->entityExtractor->getStatementMap();
    static shared_ptr<map<string, unordered_set<string>>> callProcNameMap = this->entityExtractor->getCallProcNameMap();
    static shared_ptr<map<string, unordered_set<string>>> readVarNameMap = this->entityExtractor->getReadVarNameMap();
    static shared_ptr<map<string, unordered_set<string>>> printVarNameMap = this->entityExtractor->getPrintVarNameMap();
    static shared_ptr<map<string, shared_ptr<Node>>> patternMap = this->entityExtractor->getPatternMap();
    static shared_ptr<map<string, pair<string, string>>> procedureStatementStorageMap = this->procedureLineNumberExtractor->getProcedureStatementStorageMap();

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
    static shared_ptr<map<string, unordered_set<string>>>  parentsSet = this->parentsExtractor->getStorageMap();
    static shared_ptr<map<string, unordered_set<string>>>  followsSet = this->followsExtractor->getStorageMap();
    static shared_ptr<map<string, unordered_set<string>>>  parentsStarSet = this->parentsExtractor->getStorageStarMap();
    static shared_ptr<map<string, unordered_set<string>>>  followsStarSet = this->followsExtractor->getStorageStarMap();
    static shared_ptr<map<string, unordered_set<string>>>  callSet = this->callsExtractor->getStorageMap();
    static shared_ptr<map<string, unordered_set<string>>>  callStarSet = this->callsExtractor->getStorageStarMap();
    static shared_ptr<map<string, unordered_set<string>>> modifiesSet = this->modifiesExtractor->getStorageMap();
    static shared_ptr<map<string, unordered_set<string>>> usesSet = this->usesExtractor->getStorageMap();
    static shared_ptr<map<string, unordered_set<string>>> nextSet = this->nextExtractor->getStorageMap();

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
