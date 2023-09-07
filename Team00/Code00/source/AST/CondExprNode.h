#ifndef CONDEXPRNODE_H
#define CONDEXPRNODE_H
#include <iostream>
// #include "ExprNode.h"
#include "ASTNode.h"

class CondExprNode {
public:
    virtual ~CondExprNode() = default;
    virtual std::shared_ptr<CondExprNode> getLeftCondExpr() const {
        return std::make_shared<CondExprNode>();
    }
    virtual std::shared_ptr<CondExprNode> getRightCondExpr() const {
        return std::make_shared<CondExprNode>();
    }
    virtual std::shared_ptr<ExprNode> getLeftRelFactor() const {
        return std::make_shared<ExprNode>();
    }
    virtual std::shared_ptr<ExprNode> getRightRelFactor() const {
        return std::make_shared<ExprNode>();
    }
    virtual std::string getOp() {
        return std::string("");
    }
};

class AndNode : public CondExprNode {
public:
    AndNode(std::shared_ptr<CondExprNode> leftCondExpr, std::shared_ptr<CondExprNode> rightCondExpr) :
        leftCondExpr(leftCondExpr), rightCondExpr(rightCondExpr) {}
    std::shared_ptr<CondExprNode> getLeftCondExpr() const override {
        return this->leftCondExpr;
    }
    std::shared_ptr<CondExprNode> getRightCondExpr() const override {
        return this->rightCondExpr;
    }
    std::string getOp() override {
        return " && ";
    }
private:
    std::shared_ptr<CondExprNode> leftCondExpr;
    std::shared_ptr<CondExprNode> rightCondExpr;
};

class OrNode : public CondExprNode {
public:
    OrNode(std::shared_ptr<CondExprNode> leftCondExpr, std::shared_ptr<CondExprNode> rightCondExpr) :
        leftCondExpr(leftCondExpr), rightCondExpr(rightCondExpr) {}
    std::shared_ptr<CondExprNode> getLeftCondExpr() const override {
        return this->leftCondExpr;
    }
    std::shared_ptr<CondExprNode> getRightCondExpr() const override {
        return this->rightCondExpr;
    }
    std::string getOp() override {
        return " || ";
    }
private:
    std::shared_ptr<CondExprNode> leftCondExpr;
    std::shared_ptr<CondExprNode> rightCondExpr;
};

class NotNode : public CondExprNode {
public:
    NotNode(std::shared_ptr<CondExprNode> condExpr) :
        condExpr(condExpr) {}
    std::shared_ptr<CondExprNode> getLeftCondExpr() const override {
        return this->condExpr;
    }
    std::string getOp() override {
        return "!";
    }
private:
    std::shared_ptr<CondExprNode> condExpr;
};

class RelExprNode : public CondExprNode {
public:
    RelExprNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor)
        : leftRelFactor(leftRelFactor), rightRelFactor(rightRelFactor) {}
    std::shared_ptr<ExprNode> getLeftRelFactor() const override {
        return this->leftRelFactor;
    }
    std::shared_ptr<ExprNode> getRightRelFactor() const override {
        return this->rightRelFactor;
    }
private:
    std::shared_ptr<ExprNode> leftRelFactor;
    std::shared_ptr<ExprNode> rightRelFactor;
};

class GreaterThanNode : public RelExprNode {
public:
    GreaterThanNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " > ";
    }
};

class GreaterThanEqualNode : public RelExprNode {
public:
    GreaterThanEqualNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " >= ";
    }
};

class LesserThanNode : public RelExprNode {
public:
    LesserThanNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " < ";
    }
};

class LesserThanEqualNode : public RelExprNode {
public:
    LesserThanEqualNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " <= ";
    }
};

class EqualsNode : public RelExprNode {
public:
    EqualsNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " == ";
    }
};

class NotEqualsNode : public RelExprNode {
public:
    NotEqualsNode(std::shared_ptr<ExprNode> leftRelFactor, std::shared_ptr<ExprNode> rightRelFactor) : RelExprNode(leftRelFactor, rightRelFactor) {}
    std::string getOp() override {
        return " != ";
    }
};

#endif