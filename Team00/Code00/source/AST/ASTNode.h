#ifndef ASTNODE_H
#define ASTNODE_H

#include <iostream>
#include <string>
#include <vector>
#include "ExprNode.h"

//PROMPT: Pretend you are a harsh c++ critic who strongly advocates for code efficiency, robust code patterns and no code smells, critic this code fragment written in c++.
//MEDIUM: PERPLEXITY AI PREMIUM
class StatementNode;
class ProcedureNode;
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

class ProgramNode : public ASTNode {
public:
    ProgramNode(std::vector<std::shared_ptr<ProcedureNode>> procedures) 
    : ASTNode(std::move(std::string("")), -1), procedures(procedures) {

    }
    std::vector<std::shared_ptr<ProcedureNode>> getProcedures() {
        return this->procedures;
    }
private:
    std::vector<std::shared_ptr<ProcedureNode>> procedures;
};

class ProcedureNode: public ASTNode {
public:
    ProcedureNode(std::string procedureName, std::vector<std::shared_ptr<StatementNode>> statements)
    : ASTNode(std::move(procedureName), -1), statements(statements) {

    }
    std::vector<std::shared_ptr<StatementNode>> getStatements() {
        return this->statements;
    }
private:
    std::vector<std::shared_ptr<StatementNode>> statements;
};

class StatementNode: public ASTNode {
public:
    StatementNode(std::string name, int statementNumber)
    : ASTNode(std::move(name), statementNumber) {

    }
private:
};

class AssignNode : public StatementNode {
public:
    AssignNode(int statementNumber, std::shared_ptr<ExprNode> variable, std::shared_ptr<ExprNode> expr)
        : StatementNode(std::move(std::string("")), statementNumber), variable(variable), expr(expr) {}

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

class ReadNode : public StatementNode {
public:
    ReadNode(int statementNumber, std::shared_ptr<ExprNode> variable)
        : StatementNode(std::move(std::string("")), statementNumber), variable(variable) {}
    std::shared_ptr<ExprNode> getVar() const override {
        return this->variable;
    }
private:
    std::shared_ptr<ExprNode> variable;
};

class PrintNode : public StatementNode {
public:
    PrintNode(int statementNumber, std::shared_ptr<ExprNode> variable)
        : StatementNode(std::move(std::string("")), statementNumber), variable(variable) {}
    std::shared_ptr<ExprNode> getVar() const override {
        return this->variable;
    }
private:
    std::shared_ptr<ExprNode> variable;
};


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
