#pragma once
#include <string>
#include "ClauseStmtNode.h"

class SuchThatClauseStmtNode : public ClauseStmtNode {
    SuchThatClauseStmtNode(const std::string& abstraction, const std::string& leftRef,const std::string& rightRef);
    std::string abstraction;
    std::string leftRef;
    std::string rightRef;
};