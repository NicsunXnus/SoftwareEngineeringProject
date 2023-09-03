#pragma once
#include <vector>
#include <memory>
#include <string>
#include "ASTNode.h"

class DeclarationStmtNode : public ASTNode {
public:
    DeclarationStmtNode(const std::string& entity, const std::string& synonym);
    std::string entity;
    std::string synonym;
};