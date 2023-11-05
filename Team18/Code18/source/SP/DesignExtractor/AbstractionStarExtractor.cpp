#include "AbstractionStarExtractor.h"


void AbstractionStarExtractor::extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) {
    this->extract(processedProgram);
    processAbstractionStar();
}

void AbstractionStarExtractor::insertToAbstractionStarMap(string key, string value) {
    insertIntoMap(key, value, this->AbstractionStarStorageMap);
}


shared_ptr<unordered_map<string, unordered_set<string>>> AbstractionStarExtractor::getStorageStarMap() {
    return this->AbstractionStarStorageMap;
}

// ai-gen start (gpt-3, 2)
void AbstractionStarExtractor::processAbstractionStar() {
    for (const auto& [key, value] : *AbstractionStorageMap) {
        unordered_set<string> values = unordered_set<string>(value);
        for (const auto& value : value) {
            processAbstractionStarHelper(value, values);
        }
        AbstractionStarStorageMap->insert({ key, values });
    }
}

void AbstractionStarExtractor::processAbstractionStarHelper(string value, unordered_set<string>& values) {
    if (AbstractionStorageMap->find(value) != AbstractionStorageMap->end()) {
        for (const auto& value : AbstractionStorageMap->at(value)) {
            values.insert(value);
            processAbstractionStarHelper(value, values);
        }
    }
}
// ai-gen end