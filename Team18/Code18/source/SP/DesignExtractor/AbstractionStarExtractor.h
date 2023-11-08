#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <map>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "../AST/Node.h"
#include "AbstractionExtractor.h"

using namespace std;


/**
 * This class includes methods and attributes to extract the abstraction star from the abstraction map in its parent class
 */
class AbstractionStarExtractor : public AbstractionExtractor {
public:
    // Constructor that initializes the map
    AbstractionStarExtractor() : AbstractionStarStorageMap(make_shared<StringMap>()) {}

    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) override;
    void insertToAbstractionStarMap(string key, string value);
    shared_ptr<StringMap> getStorageStarMap();

private:
    shared_ptr<StringMap> AbstractionStarStorageMap;  

    // The following methods are used to extract the abstraction star from the abstraction map
    void processAbstractionStar();
    void processAbstractionStarHelper(string value, unordered_set<string>& values);
};
