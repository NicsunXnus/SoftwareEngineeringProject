#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "ASTNode.h"
#include "../TokenizerClasses/TokenFactory.h"
#include <stack>
#include <cassert>
//Note to self, lots of repeated codes here. Create methods to modularise  them
class ASTBuilder {
public:

    //Takes in a 2d vector of statement tokens and return a 1d vector of StatementNodes
    //Temporary method to match with SIMPLETokenizer functionality
    static std::vector<std::shared_ptr<StatementNode>> parseProgram(std::vector<std::vector<std::shared_ptr<Token>>> statements) {
        std::vector<std::shared_ptr<StatementNode>> statementListToReturn;
        int statementNumber = 1;
        for (int i = 0; i < statements.size(); i++) {
            try {
                statementListToReturn.emplace_back(parseStatement(statements[i], statementNumber));
            }
            catch (std::string errMsg) {
                std::cout << "Error parsing statement " + statementNumber + std::string(" : ") + errMsg;
            }
            statementNumber++;
        }
        return statementListToReturn;
    }

    static std::shared_ptr<StatementNode> parseStatement(std::vector<std::shared_ptr<Token>> statement, int statementNumber) {
        int firstIndex = 0;
        int secondIndex = 1;
        std::shared_ptr<Token> keywordToken = statement[firstIndex];
        if (keywordToken->getName() == "print") {
            std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statement[secondIndex]->getName(), statementNumber);
            std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(statementNumber, variableNode);
            return printNode;
        }
        else if (keywordToken->getName() == "call") {
            //TODO FOR NEXT MILESTONE
        }
        else if (keywordToken->getName() == "read") {
            std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statement[secondIndex]->getName(), statementNumber);
            std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(statementNumber, variableNode);
            return readNode;
        }
        else if (keywordToken->getName() == "if") {
            //TODO FOR NEXT MILESTONE
        }
        else if (keywordToken->getName() == "while") {
            //TODO FOR NEXT MILESTONE
        }
        else if (statement[secondIndex]->getName() == "=") {
            // Is assign statement
            std::shared_ptr <ExprNode> varNode = std::make_shared < VariableNode>(statement[firstIndex]->getName(), statementNumber);
            int startOfExpression = 2; //0 -> variable, 1 -> =, 2 -> ....
            std::shared_ptr<Token> currToken = statement[startOfExpression];
            int endOfExpression = 2;
            while (currToken->getName() != ";") {
                endOfExpression = endOfExpression + 1;
                currToken = statement[endOfExpression];
            }
            std::vector<std::shared_ptr<Token>> exprTokens(statement.begin() + startOfExpression, statement.begin() + endOfExpression);
            // Evaluate statements
            std::shared_ptr <AssignNode> assignNode = std::make_shared<AssignNode>(statementNumber, varNode, parseExpr(exprTokens));
            return assignNode;
            
        }
        else {
            // Handle other cases or report an error
            throw "Unsupported Statement Type.";
        }
    }

    /*COMMENTED OUT TO BE LEFT AS REFERENCE CODE, WILL DELETE ONCE NOT NEEDED*/
    /*
     static std::vector<std::shared_ptr<StatementNode>> parseStatements(std::vector<std::shared_ptr<Token>> statements) {
         int statementNumber = 1;
         std::vector<std::shared_ptr<StatementNode>> statementListToReturn;
         for (int i = 0; i < statements.size(); ++i) {
             std::shared_ptr<Token> token = statements[i];
             if (token->getName() == "print") { //for PRINT NODE
                 i = i + 1; //To advance to the variable
                 std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statements[i]->getName());
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
                 std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statements[i]->getName());
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
                 std::shared_ptr <ExprNode> varNode = std::make_shared < VariableNode>(token->getName());
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
     }*/

    //Establishes priority of operators
    //sources: https://learn.microsoft.com/en-us/office/vba/language/reference/user-interface-help/operator-precedence
    //, https://discuss.codecademy.com/t/what-is-the-order-of-operations-for-logical-operators/452126
    static int precedence(std::string op) {
        if (op == "||") {
            return 1;
        }
        else if (op == "&&") {
            return 2;
        }
        else if (op == "!") {
            return 3;
        }
        else if (op == ">" || op == ">=" || op == "<" || op == "<=" || op == "==" || op == "!=") {
            return 4;
        }
        else if (op == "+" || op == "-") {
            return 5;
        }
        else if (op == "*" || op == "/" || op == "%") {
            return 6;
        }
        return 0;
    }

    //Creates the appropriate nodes based on the operator given
    static std::shared_ptr<ExprNode> applyOperation(std::string op, std::shared_ptr<ExprNode> a, std::shared_ptr<ExprNode> b) {
        if (op == "+") return std::make_shared<PlusNode>(a, b);
        else if (op == "*") return std::make_shared<TimesNode>(a, b);
        else if (op == "-") return std::make_shared<MinusNode>(a, b);
        else if (op == "/") return std::make_shared<DividesNode>(a, b);
        else if (op == "%") return std::make_shared<ModNode>(a, b);
        else throw "Unsupported mathematical operation.";
    }

    //Creates a tree of ExprNode which forms one big expression
    //Uses 2 stacks - One to store the operators and paranthesis, one to store the expressions/ExprNode
    //Once a ")" is reached, the top operator is popped and the top 2 expressions are also popped,then the applyoperation
    // method takes in these 3 inputs and outputs the combined expression 
    static std::shared_ptr<ExprNode> parseExpr(std::vector<std::shared_ptr<Token>> listOfTokens) {
        std::stack<std::shared_ptr<ExprNode>> values;
        std::stack<std::string> ops;

        for (int i = 0; i < listOfTokens.size(); ++i) {
            std::shared_ptr<Token> currToken = listOfTokens[i];
            if (currToken->getName() == "(") {
                ops.push("(");
            }
            else if (currToken->getName() == ")") {
                while (!ops.empty() && ops.top() != "(") {
                    std::shared_ptr<ExprNode> b = values.top(); values.pop();
                    std::shared_ptr<ExprNode> a = values.top();
                    values.pop();
                    values.push(applyOperation(ops.top(), a, b));
                    ops.pop();
                }
                ops.pop(); // Remove the '('
            }
            else if (isAlphanumeric(currToken->getName())) {
                std::shared_ptr<ExprNode> refNode;
                if (isNumber(currToken->getName()))  refNode = std::make_shared<ConstantNode>(std::stoi(currToken->getName()));
                else refNode = std::make_shared <VariableNode>(currToken->getName());
                values.push(refNode);
            }
            else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(currToken->getName())) {
                    std::shared_ptr<ExprNode> b = values.top(); values.pop();
                    std::shared_ptr<ExprNode> a = values.top();
                    values.pop();
                    values.push(applyOperation(ops.top(), a, b));
                    ops.pop();
                }
                ops.push(currToken->getName());
            }
        }
        while (!ops.empty()) {
            std::shared_ptr<ExprNode> b = values.top(); values.pop();
            std::shared_ptr<ExprNode> a = values.top();
            values.pop();
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

    //Helper function to visualise output of parseExpr
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

    static std::shared_ptr<CondExprNode> applyCompOperation(std::string op, std::shared_ptr<ExprNode> a, std::shared_ptr<ExprNode> b) {
        if (op == "<") return std::make_shared<LesserThanNode>(a, b);
        else if (op == ">") return std::make_shared<GreaterThanNode>(a, b);
        else if (op == "<=") return std::make_shared<LesserThanEqualNode>(a, b);
        else if (op == ">=") return std::make_shared<GreaterThanEqualNode>(a, b);
        else if (op == "==") return std::make_shared<EqualsNode>(a, b);
        else if (op == "!=") return std::make_shared<NotEqualsNode>(a, b);
        else throw "Unsupported Comparison Operator.";
    }

    static std::shared_ptr<CondExprNode> applyBoolOperation(std::string op, std::shared_ptr<CondExprNode> a, std::shared_ptr<CondExprNode> b) {
        if (op == "&&") return std::make_shared<AndNode>(a, b);
        else if (op == "||") return std::make_shared<OrNode>(a, b);
        else if (op == "!") return std::make_shared<NotNode>(b);
        else throw "Unsupported Boolean Operator.";
    }

    static bool isComparisonOpr(std::string op) {
        std::vector<std::string> ops = { "<", "<=", ">", ">=", "==", "!=" };
        auto it = std::find(ops.begin(), ops.end(), op);
        return it != ops.end();
    }

    static bool isBoolOpr(std::string op) {
        std::vector<std::string> ops = { "!", "&&", "||" };
        auto it = std::find(ops.begin(), ops.end(), op);
        return it != ops.end();
    }

    static std::shared_ptr<CondExprNode> parseCondExpr(std::vector<std::shared_ptr<Token>> listOfTokens) {
        std::stack<std::shared_ptr<CondExprNode>> condValues;
        std::stack<std::shared_ptr<ExprNode>> relValues;
        std::stack<std::string> ops;

        for (int i = 0; i < listOfTokens.size(); ++i) {
            std::shared_ptr<Token> currToken = listOfTokens[i];
            if (currToken->getName() == "(") {
                ops.push("(");
            }
            else if (currToken->getName() == ")") {
                while (!ops.empty() && ops.top() != "(") {
                    if (ASTBuilder::isComparisonOpr(ops.top())) {
                        assert(!relValues.empty());
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        condValues.push(applyCompOperation(ops.top(), a, b));
                        ops.pop();
                    }
                    else if (ASTBuilder::isBoolOpr(ops.top())) {
                        assert(!condValues.empty());
                        std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                        if (ops.top() == "!") {
                            condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                        }
                        else {
                            std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                            condValues.push(applyBoolOperation(ops.top(), a, b));
                        }
                        ops.pop();
                    }
                    else {
                        assert(!relValues.empty());
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        relValues.push(applyOperation(ops.top(), a, b));
                        ops.pop();
                    }
                }
                ops.pop(); // Remove the '('  
            }
            else if (isAlphanumeric(currToken->getName())) {
                std::shared_ptr<ExprNode> refNode;
                if (isNumber(currToken->getName()))  refNode = std::make_shared<ConstantNode>(std::stoi(currToken->getName()));
                else refNode = std::make_shared <VariableNode>(currToken->getName());
                relValues.push(refNode);
            }
            else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(currToken->getName())) {
                    if (ASTBuilder::isComparisonOpr(ops.top())) {
                        assert(!relValues.empty());
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        condValues.push(applyCompOperation(ops.top(), a, b));
                        ops.pop();
                    }
                    else if (ASTBuilder::isBoolOpr(ops.top())) {
                        assert(!condValues.empty());
                        std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                        if (ops.top() == "!") {
                            condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                        }
                        else {
                            std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                            condValues.push(applyBoolOperation(ops.top(), a, b));
                        }
                        ops.pop();
                    }
                    else {
                        assert(!relValues.empty());
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        relValues.push(applyOperation(ops.top(), a, b));
                        ops.pop();
                    }
                }
                ops.push(currToken->getName());
            }
        }
        while (!ops.empty()) {
            if (ASTBuilder::isComparisonOpr(ops.top())) {
                assert(!relValues.empty());
                std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                std::shared_ptr<ExprNode> a = relValues.top();
                relValues.pop();
                condValues.push(applyCompOperation(ops.top(), a, b));
                ops.pop();
            }
            else if (ASTBuilder::isBoolOpr(ops.top())) {
                assert(!condValues.empty());
                std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                if (ops.top() == "!") {
                    condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                }
                else {
                    std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                    condValues.push(applyBoolOperation(ops.top(), a, b));
                }
                ops.pop();
            }
            else {
                assert(!relValues.empty());
                std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                std::shared_ptr<ExprNode> a = relValues.top();
                relValues.pop();
                relValues.push(applyOperation(ops.top(), a, b));
                ops.pop();
            }
        }

        return condValues.top();
    }

    //Helper function to visualise output of parseCondEpxr
    static std::string printCondExpr(std::shared_ptr<CondExprNode> condExpr) {
        std::string result = std::string("");
        if (condExpr->getOp() == "!") {
            result += std::string("!(") + printCondExpr(condExpr->getLeftCondExpr()) + ")";
            return result;
        }
        else {
            if (condExpr->getLeftCondExpr()->getOp() != "") {
                result += "(" + printCondExpr(condExpr->getLeftCondExpr()) + ")";
            }
            else {
                result += printExpr(condExpr->getLeftRelFactor());
            }

            result += condExpr->getOp();

            if (condExpr->getRightCondExpr()->getOp() != "") {
                result += "(" + printCondExpr(condExpr->getRightCondExpr()) + ")";
            }
            else {
                result += printExpr(condExpr->getRightRelFactor());
            }

            return result;
        }
    }
};
#endif