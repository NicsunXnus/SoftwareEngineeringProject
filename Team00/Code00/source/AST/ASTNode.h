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

#endif