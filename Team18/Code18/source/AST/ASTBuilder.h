#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "ASTNode.h"
#include "../TokenizerClasses/TokenFactory.h"
#include <stack>

class ASTBuilder {
public:

    // Create Program Node
    //For now assume that a program just has one procedure
    //static std::shared_ptr<ProcedureNode> ASTBuilder::parseProcedure(std::vector<std::shared_ptr<Token>> listOfTokens) {
    static std::vector<std::shared_ptr<StatementNode>> parseProcedure(std::vector<std::shared_ptr<Token>> tokens) {
        std::string procedureName = tokens[0]->getName();
        std::vector<std::shared_ptr<Token>> programTokens = std::vector<std::shared_ptr<Token>>(tokens.begin() + 3, tokens.end());
        //std::vector<std::shared_ptr<ProcedureNode>> procedure = std::make_shared<ProcedureNode>(procedureName, parseStatements(programTokens));
        //return procedure
        return parseStatements(programTokens);
    }

    static std::vector<std::shared_ptr<StatementNode>> parseStatements(std::vector<std::shared_ptr<Token>> statements) {
        int statementNumber = 1;
        std::vector<std::shared_ptr<StatementNode>> statementListToReturn;
        for (int i = 0; i < statements.size(); ++i) {
            std::shared_ptr<Token> token = statements[i];
            if (token->getName() == "print") { //for PRINT NODE
                i = i + 1; //To advance to the variable
                std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statements[i]->getName(), statementNumber);
                std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(statementNumber, variableNode);
                statementNumber++;
                statementListToReturn.emplace_back(printNode);
                i = i + 1; //to advance to ;
            }
            else if (token->getName() == "call") {
                //TODO
            }
            else if (token->getName() == "read") {
                i = i + 1; //To advance to the variable
                std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statements[i]->getName(), statementNumber);
                std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(statementNumber, variableNode);
                statementNumber++;
                statementListToReturn.emplace_back(readNode);
                i = i + 1; //to advance to ;
            }
            else if (token->getName() == "if") {
                // i = i + 2;//advance past "if ("
                // int numOfTokensCondExpr = 0;
                // token = statements[i];
                // int startIndex = i;
                // while (token->getName() != "{") {
                //     i = i + 1;
                //     token = statements[i];
                // }
                // int endIndex = i - 1; //go back to position of ")"
                // std::vector<std::shared_ptr<Token>> condExprTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // // Evaluate Expression
                // std::shared_ptr<CondExprNode> condExprNode = std::dynamic_pointer_cast<CondExprNode>(parseExpr(condExprTokens));

                // // Get Then Branch
                // i = i + 1;//advance past "{"
                // token = statements[i];
                // startIndex = i;
                // while (token->getName() != "}") {
                //     i = i + 1;
                //     token = statements[i];
                // }
                // endIndex = i;
                // std::vector<std::shared_ptr<Token>> stmtsThenTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // // Evaluate statements
                // std::vector<std::shared_ptr<StatementNode>> thenStmts = parseStatements(stmtsThenTokens);

                // // Get Else Branch
                // i = i + 3; //advance past {
                // startIndex = i;
                // token = statements[i];
                // while (token->getName() != "}") {
                //     i = i + 1;
                //     token = statements[i];
                // }
                // endIndex = i;
                // std::vector<std::shared_ptr<Token>> stmtsElseTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // // Evaluate statements
                // std::vector<std::shared_ptr<StatementNode>> elseStmts = parseStatements(stmtsElseTokens);

                // std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(condExprNode, thenStmts, elseStmts);
                // statementListToReturn.emplace_back(ifNode);
            }
            else if (token->getName() == "while") {
                // i = i + 2;//advance past "while ("
                // token = statements[i];
                // int startIndex = i;
                // while (token->getName() != "{") {
                //     i = i + 1;
                //     token = statements[i];
                // }
                // int endIndex = i - 1;
                // std::vector<std::shared_ptr<Token>> condExprTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // // Evaluate Expression
                // std::shared_ptr<CondExprNode> condExprNode = std::dynamic_pointer_cast<CondExprNode>(parseExpr(condExprTokens));

                // // Get Loop Branch
                // i = i + 1;//advance past "{"
                // token = statements[i];
                // startIndex = i;
                // while (token->getName() != "}") {
                //     i = i + 1;
                //     token = statements[i];
                // }
                // endIndex = i;
                // std::vector<std::shared_ptr<Token>> stmtsTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // // Evaluate statements
                // std::vector<std::shared_ptr<StatementNode>> loopStmts = parseStatements(stmtsTokens);
                // std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(condExprNode, loopStmts);
                // statementListToReturn.emplace_back(whileNode);
            }
            else {
                // Is assign statement
                std::shared_ptr <ExprNode> varNode = std::make_shared < VariableNode>(token->getName(), statementNumber);
                i = i + 2; //advance past =
                token = statements[i];
                int startIndex = i;
                while (token->getName() != ";") {
                    i = i + 1;
                    token = statements[i];
                }
                int endIndex = i;
                std::vector<std::shared_ptr<Token>> exprTokens(statements.begin() + startIndex, statements.begin() + endIndex);
                // Evaluate statements
                std::shared_ptr <AssignNode> assignNode = std::make_shared<AssignNode>(statementNumber, varNode, parseExpr(exprTokens));
                statementNumber++;
                statementListToReturn.emplace_back(assignNode);
            }
        }
        return statementListToReturn;
    }

    //Establishes priority of operators
    //TODO: Include comparison operators
    static int precedence(std::string op) {
        if (op == "+" || op == "-") {
            return 1;
        }
        else if (op == "*" || op == "/" || op == "%") {
            return 2;
        }
        // else if (op == "!") {
        //     return 3;
        // }
        return 0;
    }

    //Creates the appropriate nodes based on the operator given
    static std::shared_ptr<ExprNode> applyOperation(std::string op, std::shared_ptr<ExprNode> a, std::shared_ptr<ExprNode> b) {
        if (op == "+") return std::make_shared<PlusNode>(a, b);
        else if (op == "*") return std::make_shared<TimesNode>(a, b);
        else if (op == "-") return std::make_shared<MinusNode>(a, b);
        else if (op == "/") return std::make_shared<DividesNode>(a, b);
        else if (op == "%") return std::make_shared<ModNode>(a, b);
        // else if (op == "<") return std::make_shared<LessThanNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == "<=") return std::make_shared<LessThanEqualNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == ">") return std::make_shared<GreaterThanNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == ">=") return std::make_shared<GreaterThanEqualNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == "==") return std::make_shared<EqualNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == "!=") return std::make_shared<NotEqualNode>(std::dynamic_pointer_cast<RelFactorNode>(a), std::dynamic_pointer_cast<RelFactorNode>(b));
        // else if (op == "!") return std::make_shared<NotNode>(std::dynamic_pointer_cast<CondExprNode>(b));
        // else if (op == "&&") return std::make_shared<AndNode>(std::dynamic_pointer_cast<CondExprNode>(a), std::dynamic_pointer_cast<CondExprNode>(b));
        // else if (op == "||") return std::make_shared<OrNode>(std::dynamic_pointer_cast<CondExprNode>(a), std::dynamic_pointer_cast<CondExprNode>(b));
    }

    //Creates a tree of ExprNode which forms one big expression
    //Uses 2 stacks - One to store the operators and paranthesis, one to store the expressions/ExprNode
    //Once a ")" is reached, the top operator is popped and the top 2 expressions are also popped,then the applyoperation
    // method takes in these 3 inputs and outputs the combined expression 
    static std::shared_ptr<ExprNode> parseExpr(std::vector<std::shared_ptr<Token>> listOfTokens) {
        std::stack<std::shared_ptr<ExprNode>> values;
        std::stack<std::string> ops;

        for (int i = 0; i < listOfTokens.size(); ++i) {
            std::shared_ptr<Token> token = listOfTokens[i];
            if (token->getName() == "(") {
                ops.push("(");
            }
            else if (token->getName() == ")") {
                while (!ops.empty() && ops.top() != "(") {
                    std::shared_ptr<ExprNode> b = values.top(); values.pop();
                    std::shared_ptr<ExprNode> a = values.top();
                    if (ops.top() != "!") values.pop();
                    values.push(applyOperation(ops.top(), a, b));
                    ops.pop();
                }
                ops.pop(); // Remove the '('
            }
            else if (isAlphanumeric(token->getName())) {
                std::shared_ptr<ExprNode> refNode;
                if (isNumber(token->getName()))  refNode = std::make_shared<ConstantNode>(std::stoi(token->getName()));
                else refNode = std::make_shared <VariableNode>(token->getName());
                values.push(refNode);
            }
            else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(token->getName())) {
                    std::shared_ptr<ExprNode> b = values.top(); values.pop();
                    std::shared_ptr<ExprNode> a = values.top();
                    if (ops.top() != "!") values.pop();
                    values.push(applyOperation(ops.top(), a, b));
                    ops.pop();
                }
                ops.push(token->getName());
            }
        }
        while (!ops.empty()) {
            std::shared_ptr<ExprNode> b = values.top(); values.pop();
            std::shared_ptr<ExprNode> a = values.top();
            if (ops.top() != "!") values.pop();
            values.push(applyOperation(ops.top(), a, b));
            ops.pop();
        }

        return values.top();
    }

    static bool isAlphanumeric(std::string str) {
        for (char c : str) {
            if (!std::isalnum(c)) {
                return false;
            }
        }
        return true;
    }

    static bool isNumber(std::string str) {
        for (char c : str) {
            if (!std::isdigit(c)) {
                return false;
            }
        }
        return true;
    }

    static std::string printExpr(std::shared_ptr<ExprNode> expr) {
        std::string result = "";
        if (!expr->isTerminal()) {
            result += printExpr(expr->getLeftExpr());
            result += expr->getValue();
            result += printExpr(expr->getRightExpr());
        }
        else {
            result += expr->getValue();
        }
        return result;
    }

};
#endif