#pragma once
#include<stdio.h>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <unordered_map>

#include "UsesModifiesExtractor.h"

using namespace std;


class ModifiesExtractor : public UsesModifiesExtractor {
public:
    void extract(shared_ptr<ProcessedWhileStmt> processedWhile) override;
    void extract(shared_ptr<ProcessedIfStmt> processedIf) override;
    void extract(shared_ptr<ProcessedAssignStmt> processedAssign) override;
    void extract(shared_ptr<ProcessedReadStmt> processedRead) override;
};