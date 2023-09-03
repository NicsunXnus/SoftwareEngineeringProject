#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ASTNode.h"

class SelectStmtNode : public ASTNode {
public:
    SelectStmtNode(const std::string& synonym);
    std::string synonym;
};