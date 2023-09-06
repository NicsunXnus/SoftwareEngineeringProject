#ifndef EXPRNODE_H
#define EXPRNODE_H
#include <iostream>
#include <string>

class ExprNode : public ASTNode  {
public:
    virtual ~ExprNode() = default;

    virtual bool isTerminal() const {
        return false;
    }

    virtual std::string getValue() const {
        return "";
    }

    virtual std::shared_ptr<ExprNode> getLeftExpr() const {
        return std::make_shared<ExprNode>();
    }

    virtual std::shared_ptr<ExprNode> getRightExpr() const {
        return std::make_shared<ExprNode>();
    }
    ExprNode(std::string name = "", int statementNumber = -1)
    : ASTNode(std::move(name), statementNumber) {}
};

class DividesNode : public ExprNode {
public:
    DividesNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : ExprNode("DividesNode"), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return " / ";
    }

    std::shared_ptr<ExprNode> getLeftExpr() const override {
        return this->leftExpr;
    }

    std::shared_ptr<ExprNode> getRightExpr() const override {
        return this->rightExpr;
    }
private:
    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};

class MinusNode : public ExprNode {
public:
    MinusNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : ExprNode("MinusNode"), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return " - ";
    }

    std::shared_ptr<ExprNode> getLeftExpr() const override {
        return this->leftExpr;
    }

    std::shared_ptr<ExprNode> getRightExpr() const override {
        return this->rightExpr;
    }
private:
    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};

class ModNode : public ExprNode {
public:
    ModNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : ExprNode("ModNode"), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return " % ";
    }

    std::shared_ptr<ExprNode> getLeftExpr() const override {
        return this->leftExpr;
    }

    std::shared_ptr<ExprNode> getRightExpr() const override {
        return this->rightExpr;
    }
private:
    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};

class PlusNode : public ExprNode {
public:
    PlusNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : ExprNode("PlusNode"), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return " + ";
    }


    std::shared_ptr<ExprNode> getLeftExpr() const override {
        return this->leftExpr;
    }

    std::shared_ptr<ExprNode> getRightExpr() const override {
        return this->rightExpr;
    }
private:
    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
    std::string op;
};

class TimesNode : public ExprNode {
public:
    TimesNode(std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : ExprNode("TimesNode"), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return " * ";
    }

    std::shared_ptr<ExprNode> getLeftExpr() const override {
        return this->leftExpr;
    }

    std::shared_ptr<ExprNode> getRightExpr() const override {
        return this->rightExpr;
    }
private:
    std::shared_ptr<ExprNode> leftExpr;
    std::shared_ptr<ExprNode> rightExpr;
};

class ConstantNode : public ExprNode {
public:
    ConstantNode(int value)
        : ExprNode("ConstantNode"), value(value) {}

    int getIntVal() const {
        return this->value;
    }

    std::string getValue() const override {
        return std::to_string(this->value);
    }

    bool isTerminal() const override {
        return true;
    }

private:
    int value;
};

class VariableNode : public ExprNode {
public:
    VariableNode(std::string name, int statementNumber)
        : ExprNode("VariableNode", statementNumber), name(std::move(name)) {}

    bool isTerminal() const override {
        return true;
    }
    std::string getValue() const override {
        return this->name;
    }

private:
    std::string name;
};


#endif