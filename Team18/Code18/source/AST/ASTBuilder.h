#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include "ASTNode.h"
#include "../TokenizerClasses/TokenFactory.h"
#include "../HelperFunctions.h"
#include "ASTBuilderHelperFunctions.h"
#include <stack>
#include <map>
#include <algorithm>

class ASTBuilder {
private:
    //UNLOCK IN MILESTONE 2
    //static std::map<std::string, std::shared_ptr<CallNode>> callMap; //Map to store all callNodes calling procedures that have not been parsed
    //static std::map<std::string,std::shared_ptr<ProcedureNode>> procedureMap; //Map to store all procedureNodes for callNodes reference
public:
    //Takes in a 2d vector of statement tokens and return a 1d vector of StatementNodes
    //Temporary method to match with SIMPLETokenizer functionality
    static std::shared_ptr<ProgramNode> parseProgram2(std::make_shared<TokenizedProgram> tokenizedProgram) {
        std::shared_ptr<ProgramNode> programNode;

        std::vector<std::shared_ptr<TokenizedProcedure>> tokenizedProcedures = tokenizedProgram->getAllProcedures();
        std::vector<std::shared_ptr<ProcedureNode>> procedureListToReturn;
        for (std::shared_ptr<TokenizedProcedure> currProcedure : tokenizedProcedures) {
            std::shared_ptr<ProcedureNode> procedureNode;
            std::string procName = currProcedure->getName();
            std::vector<std::shared_ptr<StatementNode>> statementListToReturn;
            std::shared_ptr<TokenizedStmtList> stmtList = currProcedure->getStmts();
            for (std::shared_ptr<TokenizedStmt> currStmt : stmtList) {
                std::shared_ptr<StatementNode> statementNode;
                statementNode = parseStatement2(currStmt);
                statementListToReturn.emplace_back(statementNode);
            }
            procedureNode = std::make_shared<ProcedureNode>(procName, statementListToReturn);
            procedureListToReturn.emplace_back(procedureNode);
        }
        programNode = std::make_shared<ProgramNode>(procedureListToReturn);

        return programNode;
    }

    static std::shared_ptr<StatementNode> parseStatement2(std::shared_ptr<TokenizedStmt> statement) {
        if (auto tokenizedSemicolonStmt = std::dynamic_pointer_cast<TokenizedSemicolonStmt>(statement)) {
            std::vector<std::shared_ptr<Token>> statementContents = tokenizedSemicolonStmt->getContents();
            int firstIndex = 0;
            int secondIndex = 1;
            std::shared_ptr<Token> keywordToken = statementContents[firstIndex];
            int statementNumber = tokenizedSemicolonStmt->getStatementNumber();
            if (keywordToken->getName() == "call") {
                //TODO for Milestone 2
                //UNLOCK IN MILESTONE 2
                /*std::shared_ptr<CallNode> callNode;
                std::string procedureName = statement[secondIndex]->getName();
                if (containsProcedure(procedureName, procedureMap)) {
                    callNode = std::make_shared<CallNode>(statementNumber, procedureMap[procedureName]);
                    return callNode;
                }
                else {
                    std::vector<std::shared_ptr<StatementNode>> dummyStatements;
                    std::shared_ptr<ProcedureNode> dummyProcNode = std::make_shared<ProcedureNode>("dummyProc",  dummyStatements);
                    callNode = std::make_shared<CallNode>(statementNumber, dummyProcNode);
                    callMap.insert({ procedureName, callNode }); //store pointer to callNode, will use pointer to update the callNode after all procedures have been parsed
                 return callNode;
                }*/
                std::string procedureName = statementContents[secondIndex]->getName();
                std::vector<std::shared_ptr<StatementNode>> dummyStatements;
                std::shared_ptr<ProcedureNode> dummyProcNode = std::make_shared<ProcedureNode>(procedureName, dummyStatements);
                std::shared_ptr<CallNode> dummyCallNode = std::make_shared<CallNode>(statementNumber, dummyProcNode);
                return dummyCallNode;
            }
            else if (keywordToken->getName() == "print") {
                std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statementContents[secondIndex]->getName(), statementNumber);
                std::shared_ptr<PrintNode> printNode = std::make_shared<PrintNode>(statementNumber, variableNode);
                return printNode;
            }
            else { //read
                std::shared_ptr<VariableNode> variableNode = std::make_shared<VariableNode>(statementContents[secondIndex]->getName(), statementNumber);
                std::shared_ptr<ReadNode> readNode = std::make_shared<ReadNode>(statementNumber, variableNode);
                return readNode;
            }
        }
        else { //TokenizedConditionalStmt 
            if (auto tokenizedIfStmt = std::dynamic_pointer_cast<TokenizedIfStmt>(statement)) { 
                std::vector<std::shared_ptr<Token>> condExprTokens = tokenizedIfStmt->getConditionalExp(); 
                std::shared_ptr<CondExprNode> condExpr = parseCondExpr(condExprTokens);

                std::shared_ptr<TokenizedStmtList> tokenizedThenSmts = tokenizedIfStmt->getThenBlock();
                std::vector<std::shared_ptr<StatementNode>> thenStmts;
                for (std::shared_ptr<TokenizedStmt> thenStatement : tokenizedThenSmts) {
                    thenStmts.emplace_back(parseStatememt2(thenStatement));
                }

                std::shared_ptr<TokenizedStmtList> tokenizedElseStmts = tokenizedIfStmt->getElseBlock();
                std::vector<std::shared_ptr<StatementNode>> elseStmts;
                for (std::shared_ptr<TokenizedStmt> elseStatement : tokenizedElseStmts) {
                    elseStmts.emplace_back(parseStatememt2(elseStatement));
                }
                std::shared_ptr<IfNode> ifNode = std::make_shared<IfNode>(tokenizedIfStmt->getStatementNumber(), condExpr, thenStmts, elseStmts);
                return ifNode;
            }
            else { //TokenizedWhileStmt 
                std::shared_ptr<TokenizedWhileStmt> tokenizedWhileStmt = std::dynamic_pointer_cast<TokenizedWhileStmt>(statement);
                std::vector<std::shared_ptr<Token>> condExprTokens = tokenizedWhileStmt->getConditionalExp();
                std::shared_ptr<CondExprNode> condExpr = parseCondExpr(condExprTokens);

                std::shared_ptr<TokenizedStmtList> tokenizedLoopStmts = tokenizedWhileStmt->getWhileBlock();
                std::vector<std::shared_ptr<StatementNode>> loopStmts;
                for (std::shared_ptr<TokenizedStmt> loopStatement : tokenizedLoopStmts) {
                    loopStmts.emplace_back(parseStatememt2(loopStatement));
                }
                std::shared_ptr<WhileNode> whileNode = std::make_shared<WhileNode>(tokenizedWhileStmt->getStatementNumber(), condExpr, thenStmts, elseStmts);
                return whileNode;
            }
        }
    }

    /*static bool containsProcedure(std::string procedureToFind, std::map<std::string, std::shared_ptr<ProcedureNode>> procMap) {
        return procMap.find(procedureToFind) != procMap.end();
    }*/

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
                    if (values.size() < 2) { //at least 2 items in values to apply mathematical operation
                        throw std::runtime_error("Need 2 operatees to operate on.");
                    }
                    std::shared_ptr<ExprNode> b = values.top(); values.pop();
                    std::shared_ptr<ExprNode> a = values.top();
                    values.pop();
                    values.push(applyOperation(ops.top(), a, b));
                    ops.pop();
                }
                if (ops.empty()) {
                    throw std::runtime_error("Missing left bracket.");
                }
                ops.pop();// Remove the '('
            }
            else if (isAlphanumeric(currToken->getName())) {
                std::shared_ptr<ExprNode> refNode;
                if (isNumber(currToken->getName()))  refNode = std::make_shared<ConstantNode>(std::stoi(currToken->getName()));
                else refNode = std::make_shared <VariableNode>(currToken->getName());
                values.push(refNode);
            }
            else {
                while (!ops.empty() && precedence(ops.top()) >= precedence(currToken->getName())) {
                    if (values.size() < 2) { //at least 2 items in values to apply mathematical operation
                        throw std::runtime_error("Need 2 operatees to operate on.");
                    }
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
            if (values.size() < 2) { //at least 2 items in values to apply mathematical operation
                throw std::runtime_error("Need 2 operatees to operate on.");
            }
            std::shared_ptr<ExprNode> b = values.top(); values.pop();
            std::shared_ptr<ExprNode> a = values.top();
            values.pop();
            values.push(applyOperation(ops.top(), a, b));
            ops.pop();
        }

        return values.top();
    }
    
    //Refactor this to remove duplicate codes and handle error situations
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
                    if (isComparisonOpr(ops.top())) {
                        //assert(!relValues.empty());
                        if (relValues.size() < 2) {
                            throw std::runtime_error("Need 2 comparison operatees to operate on.");
                        }
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        condValues.push(applyCompOperation(ops.top(), a, b));
                        ops.pop();
                    }
                    else if (isBoolOpr(ops.top())) {
                        //assert(!condValues.empty());
                        if (condValues.empty()) {
                            throw std::runtime_error("Missing conditional operatee.");
                        }
                        std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                        if (ops.top() == "!") {
                            condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                        }
                        else {
                            if (condValues.empty()) {
                                throw std::runtime_error("Missing conditional operatee.");
                            }
                            std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                            condValues.push(applyBoolOperation(ops.top(), a, b));
                        }
                        ops.pop();
                    }
                    else {
                        //assert(!relValues.empty());
                        if (relValues.size() < 2) {
                            throw std::runtime_error("Need 2 operatees to operate on.");
                        }
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
                    if (isComparisonOpr(ops.top())) {
                        //assert(!relValues.empty());
                        if (relValues.size() < 2) {
                            throw std::runtime_error("Need 2 comparison operatees to operate on.");
                        }
                        std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                        std::shared_ptr<ExprNode> a = relValues.top();
                        relValues.pop();
                        condValues.push(applyCompOperation(ops.top(), a, b));
                        ops.pop();
                    }
                    else if (isBoolOpr(ops.top())) {
                        //assert(!condValues.empty());
                        if (condValues.empty()) {
                            throw std::runtime_error("Missing conditional operatee.");
                        }
                        std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                        if (ops.top() == "!") {
                            condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                        }
                        else {
                            //assert(!condValues.empty());
                            if (condValues.empty()) {
                                throw std::runtime_error("Missing conditional operatee.");
                            }
                            std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                            condValues.push(applyBoolOperation(ops.top(), a, b));
                        }
                        ops.pop();
                    }
                    else {
                        //assert(!relValues.empty());
                        if (relValues.size() < 2) {
                            throw std::runtime_error("Need 2 operatees to operate on.");
                        }
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
            if (isComparisonOpr(ops.top())) {
                //assert(!relValues.empty());
                if (relValues.size() < 2) {
                    throw std::runtime_error("Need 2 operatees to operate on.");
                }
                std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                std::shared_ptr<ExprNode> a = relValues.top();
                relValues.pop();
                condValues.push(applyCompOperation(ops.top(), a, b));
                ops.pop();
            }
            else if (isBoolOpr(ops.top())) {
                //assert(!condValues.empty());
                if (condValues.empty()) {
                    throw std::runtime_error("Missing conditional operatee.");
                }
                std::shared_ptr<CondExprNode> b = condValues.top(); condValues.pop();
                if (ops.top() == "!") {
                    condValues.push(applyBoolOperation(ops.top(), std::shared_ptr<CondExprNode>(), b));
                }
                else {
                    //assert(!condValues.empty());
                    if (condValues.empty()) {
                        throw std::runtime_error("Missing conditional operatee.");
                    }
                    std::shared_ptr<CondExprNode> a = condValues.top(); condValues.pop();
                    condValues.push(applyBoolOperation(ops.top(), a, b));
                }
                ops.pop();
            }
            else {
                //assert(!relValues.empty());
                if (relValues.size() < 2) {
                    throw std::runtime_error("Need 2 operatees to operate on.");
                }
                std::shared_ptr<ExprNode> b = relValues.top(); relValues.pop();
                std::shared_ptr<ExprNode> a = relValues.top();
                relValues.pop();
                relValues.push(applyOperation(ops.top(), a, b));
                ops.pop();
            }
        }

        return condValues.top();
    }
};
#endif