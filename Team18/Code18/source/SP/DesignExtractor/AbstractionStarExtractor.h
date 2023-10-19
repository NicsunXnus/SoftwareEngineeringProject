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

class AbstractionStarExtractor : public AbstractionExtractor {
public:
    void extractAbstractions(shared_ptr<ProcessedProgram> processedProgram) override;
};
