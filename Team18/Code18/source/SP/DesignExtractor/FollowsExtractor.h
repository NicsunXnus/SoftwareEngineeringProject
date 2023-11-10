#pragma once
#include <stdio.h>

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../SimpleProcessor/ProcessedProgram.h"
#include "AbstractionStarExtractor.h"

class FollowsExtractor : public AbstractionStarExtractor {
 public:
  void extract(shared_ptr<ProcessedStmtList> processedStmtList) override;
};