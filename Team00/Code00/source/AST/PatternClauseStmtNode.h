#pragma once
#include <string>
#include "ClauseStmtNode.h"

class PatternClauseStmtNode : public ClauseStmtNode {
    PatternClauseStmtNode(const std::string& leftRef,const std::string& rightRef);
    std::string leftRef;
    std::string rightRef;
};