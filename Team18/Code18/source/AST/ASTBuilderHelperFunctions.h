#ifndef ASTBUILDERHELPERFUNCTIONS_H
#define ASTBUILDERHELPERFUNCTIONS_H

#include "ASTNode.h"
using namespace std;
//Establishes priority of operators
//sources: https://learn.microsoft.com/en-us/office/vba/language/reference/user-interface-help/operator-precedence
//, https://discuss.codecademy.com/t/what-is-the-order-of-operations-for-logical-operators/452126
static int precedence(string op) {
    if (op == "(") {
        return 0;
    }
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
    else {
        throw invalid_argument("Unsupported operation for precedence check: " + op);
    }
    return 0;
}

//Helper function to visualise output of parseExpr
static string printExpr(shared_ptr<ExprNode> expr) {
    string result = "";
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

//Creates the appropriate nodes based on the operator given
static std::shared_ptr<ExprNode> applyOperation(std::string op, std::shared_ptr<ExprNode> a, std::shared_ptr<ExprNode> b) {
    if (op == "+") return std::make_shared<PlusNode>(a, b);
    else if (op == "*") return std::make_shared<TimesNode>(a, b);
    else if (op == "-") return std::make_shared<MinusNode>(a, b);
    else if (op == "/") return std::make_shared<DividesNode>(a, b);
    else if (op == "%") return std::make_shared<ModNode>(a, b);
    else throw std::invalid_argument("Unsupported mathematical operation: " + op);
}

static shared_ptr<CondExprNode> applyCompOperation(string op, shared_ptr<ExprNode> a, shared_ptr<ExprNode> b) {
    if (op == "<") return make_shared<LesserThanNode>(a, b);
    else if (op == ">") return make_shared<GreaterThanNode>(a, b);
    else if (op == "<=") return make_shared<LesserThanEqualNode>(a, b);
    else if (op == ">=") return make_shared<GreaterThanEqualNode>(a, b);
    else if (op == "==") return make_shared<EqualsNode>(a, b);
    else if (op == "!=") return make_shared<NotEqualsNode>(a, b);
    else throw invalid_argument("Unsupported Comparison Operator: " + op);
}

static shared_ptr<CondExprNode> applyBoolOperation(string op, shared_ptr<CondExprNode> a, shared_ptr<CondExprNode> b) {
    if (op == "&&") return make_shared<AndNode>(a, b);
    else if (op == "||") return make_shared<OrNode>(a, b);
    else if (op == "!") return make_shared<NotNode>(b);
    else throw invalid_argument("Unsupported Boolean Operator: " + op);
}

static bool isComparisonOpr(string op) {
    vector<string> ops = { "<", "<=", ">", ">=", "==", "!=" };
    auto it = find(ops.begin(), ops.end(), op);
    return it != ops.end();
}

static bool isBoolOpr(string op) {
    vector<string> ops = { "!", "&&", "||" };
    auto it = find(ops.begin(), ops.end(), op);
    return it != ops.end();
}

//Helper function to visualise output of parseCondEpxr
static string printCondExpr(shared_ptr<CondExprNode> condExpr) {
    string result = string("");
    if (condExpr->getOp() == "!") {
        result += string("!(") + printCondExpr(condExpr->getLeftCondExpr()) + ")";
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
#endif