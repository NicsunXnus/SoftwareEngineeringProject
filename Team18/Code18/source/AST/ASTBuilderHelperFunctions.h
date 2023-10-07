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

static string callMapper(shared_ptr<CallNode> callnode);

static string printStatement(shared_ptr<StatementNode> statement, int nesting) {
    auto assignStmt = dynamic_pointer_cast<AssignNode>(statement);
    auto ifStmt = dynamic_pointer_cast<IfNode>(statement);
    auto callStmt = dynamic_pointer_cast<CallNode>(statement);
    auto readStmt = dynamic_pointer_cast<ReadNode>(statement);
    auto printStmt = dynamic_pointer_cast<PrintNode>(statement);
    auto whileStmt = dynamic_pointer_cast<WhileNode>(statement);
    string indent = "";
    int count = nesting;
    while (count > 0) {
        indent.append("\t");
        count -= 1;
    }
    if (assignStmt) {
        return to_string(assignStmt->getStatementNumber()) + indent + assignStmt->getVar()->getValue() + " = " + printExpr(assignStmt->getExpr()) + "\n";
    }
    else if (ifStmt) {
        string res = "";
        res += to_string(ifStmt->getStatementNumber()) + indent + "if (" + printCondExpr(ifStmt->getCondExpr()) + ") {\n";
        std::vector<std::shared_ptr<StatementNode>> thenStmts = ifStmt->getStatements();
        for (std::shared_ptr<StatementNode> thenStmtNode : thenStmts) {
            res += printStatement(thenStmtNode, nesting + 1);
        }
        res += indent + "} else {\n";
        std::vector<std::shared_ptr<StatementNode>> elseStmts = ifStmt->getElseStatements();
        for (std::shared_ptr<StatementNode> elseStmtNode : elseStmts) {
            res += printStatement(elseStmtNode, nesting + 1);
        }
        res += indent + "}\n";
        return res;
    }
    else if (callStmt) {
        return to_string(callStmt->getStatementNumber()) + indent + "calls " + callStmt->getProc()->getName() + "\n" +callMapper(callStmt);
    }
    else if (readStmt) {
        return to_string(readStmt->getStatementNumber()) + indent + readStmt->getName() + " " + readStmt->getVar()->getValue() + "\n";
    }
    else if (printStmt) {
        return to_string(printStmt->getStatementNumber()) + indent + printStmt->getName() + " " + printStmt->getVar()->getValue() + "\n";
    }
    else if (whileStmt) {
        string res = "";
        res += to_string(whileStmt->getStatementNumber()) + indent + "while (" + printCondExpr(whileStmt->getCondExpr()) + ") {\n";
        std::vector<std::shared_ptr<StatementNode>> loopStmts = whileStmt->getStatements();
        for (std::shared_ptr<StatementNode> stmtNode : loopStmts) {
            res += printStatement(stmtNode, nesting + 1);
        }
        res += indent + "}\n";
        return res;
    }
    else {
        return "Unknown node";
    }
}

static string printProcedure(shared_ptr<ProcedureNode> procNode) {
    string res = "procedure ";
    res += procNode->getName();
    res += " {\n";

    //Body
    vector<shared_ptr<StatementNode>> statements = procNode->getStatements();
    for (shared_ptr<StatementNode> statement : statements) {
         res += printStatement(statement, 1);
    }

    res += "}\n";
    return res;
}

string callMapper(shared_ptr<CallNode> callnode) {
    shared_ptr<ProcedureNode> procNode = callnode->getProc();
    string res = "";
    res += "/////////Inner procedure of " + procNode->getName() + "//////////\n";
    res += printProcedure(procNode);
    res += "/////////Endof procedure of " + procNode->getName() + "//////////\n";
    return res;
}

static string printProgram(shared_ptr<ProgramNode> progNode) {
    string res = "";
    for (shared_ptr<ProcedureNode> procNode : progNode->getProcedures()) {
        res += printProcedure(procNode);
    }
    return res;
}
#endif