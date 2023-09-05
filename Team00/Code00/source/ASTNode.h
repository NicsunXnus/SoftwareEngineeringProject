#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <string>
//PROMPT: Pretend you are a harsh c++ critic who strongly advocates for code efficiency, robust code patterns and no code smells, critic this code fragment written in c++.
//MEDIUM: PERPLEXITY AI PREMIUM
class ExprNode;
class ASTNode {
public:
    virtual ~ASTNode() = default;

    std::string getName() const {
        return name;
    }

    int getStatementNumber() const {
        return statementNumber;
    }

    bool isStatement() const {
        return statementNumber > 0;
    }

    virtual std::string getStr() const {
        return std::string("");
    }

    virtual std::shared_ptr<ExprNode> getVar() const {
        return std::make_shared<ExprNode>();
    }

    virtual std::shared_ptr<ExprNode> getExpr() const {
        return std::make_shared<ExprNode>();
    }

protected:
    ASTNode(std::string name = "", int statementNumber = -1)
        : name(std::move(name)), statementNumber(statementNumber) {}

private:
    std::string name;
    int statementNumber;
};

class ExprNode {
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
};

class AssignNode : public ASTNode {
public:
    AssignNode(std::string name, int statementNumber, std::shared_ptr<ExprNode> variable, std::shared_ptr<ExprNode> expr)
        : ASTNode(std::move(name), statementNumber), variable(variable), expr(expr) {}

    std::shared_ptr<ExprNode> getVar() const override {
        return this->variable;
    }
    std::shared_ptr<ExprNode> getExpr() const override {
        return this->expr;
    }

private:
    std::shared_ptr<ExprNode> variable;
    std::shared_ptr<ExprNode> expr;
};

class ReadNode : public ASTNode {
public:
    ReadNode(std::string name, int statementNumber, std::shared_ptr<ExprNode> variable)
        : ASTNode(std::move(name), statementNumber), variable(variable) {}
    std::shared_ptr<ExprNode> getVar() const override {
        return this->variable;
    }
private:
    std::shared_ptr<ExprNode> variable;
};

class PrintNode : public ASTNode {
public:
    PrintNode(std::string name, int statementNumber, std::shared_ptr<ExprNode> variable)
        : ASTNode(std::move(name), statementNumber), variable(variable) {}
    std::shared_ptr<ExprNode> getVar() const override {
        return this->variable;
    }
private:
    std::shared_ptr<ExprNode> variable;
};

class DividesNode : public ExprNode {
public:
    DividesNode(std::string op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return this->op;
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

class MinusNode : public ExprNode {
public:
    MinusNode(std::string op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return this->op;
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

class ModNode : public ExprNode {
public:
    ModNode(std::string op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return this->op;
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

class PlusNode : public ExprNode {
public:
    PlusNode(std::string op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return this->op;
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
    TimesNode(std::string op, std::shared_ptr<ExprNode> leftExpr, std::shared_ptr<ExprNode> rightExpr)
        : op(op), leftExpr(leftExpr), rightExpr(rightExpr) {}
    std::string getValue() const override {
        return this->op;
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

class ConstantNode : public ExprNode {
public:
    ConstantNode(int value)
        : value(value) {}

    int getIntVal() const {
        return this->value;
    }

    std::string getValue() const override{
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
    VariableNode(std::string name)
        : name(std::move(name)){}

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