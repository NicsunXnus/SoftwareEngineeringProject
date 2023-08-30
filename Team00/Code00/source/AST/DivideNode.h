#pragma once
#include "ExprNode.h"
#include <memory>

class DivideNode : public ExprNode {
public:
    DivideNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr);

    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};