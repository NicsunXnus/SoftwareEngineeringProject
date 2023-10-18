#pragma once
#include "ASTBuilder.h"

shared_ptr<ProgramNode> ASTBuilder::parseProgram(
    shared_ptr<TokenizedProgram> tokenizedProgram) {
  shared_ptr<ProgramNode> programNode;

  vector<shared_ptr<TokenizedProcedure>> tokenizedProcedures =
      tokenizedProgram->getAllProcedures();
  vector<shared_ptr<ProcedureNode>> procedureListToReturn;
  for (shared_ptr<TokenizedProcedure> currProcedure : tokenizedProcedures) {
    shared_ptr<ProcedureNode> procedureNode;
    string procName = string(currProcedure->getName());
    vector<shared_ptr<StatementNode>> statementListToReturn;
    vector<shared_ptr<TokenizedStmt>> stmtList =
        currProcedure->getStmts()->getStmts();
    for (shared_ptr<TokenizedStmt> currStmt : stmtList) {
      shared_ptr<StatementNode> statementNode;
      statementNode = parseStatement(currStmt);
      statementListToReturn.emplace_back(statementNode);
    }
    procedureNode = make_shared<ProcedureNode>(procName, statementListToReturn);
    ASTBuilder::procedureMap[procName] = procedureNode;
    procedureListToReturn.emplace_back(procedureNode);
  }

  // adds all the procedurenode to those callnode calling them too early before
  for (const auto& [thisProcName, callNode] : ASTBuilder::callMap) {
    for (const auto& [thatProcName, procNode] : ASTBuilder::procedureMap) {
      if (thisProcName == thatProcName) {
        callNode->setProcNode(procNode);
        break;
      }
    }
  }

  programNode = make_shared<ProgramNode>(procedureListToReturn);

  return programNode;
}

static bool containsProcedure(string procedureToFind,
                              map<string, shared_ptr<ProcedureNode>> procMap) {
  return procMap.find(procedureToFind) != procMap.end();
}

shared_ptr<StatementNode> ASTBuilder::parseStatement(
    shared_ptr<TokenizedStmt> statement) {
  int statementNumber = statement->getStatementNumber();
  if (auto tokenizedSemicolonStmt =
          dynamic_pointer_cast<TokenizedSemicolonStmt>(statement)) {
    vector<shared_ptr<Token>> statementContents =
        tokenizedSemicolonStmt->getContents();
    int firstIndex = 0;
    int secondIndex = 1;
    shared_ptr<Token> keywordToken = statementContents[firstIndex];
    int statementNumber = tokenizedSemicolonStmt->getStatementNumber();
    if (statementContents[secondIndex]->getName() == "=") {
      // Is assign statement
      std::shared_ptr<ExprNode> varNode = std::make_shared<VariableNode>(
          statementContents[firstIndex]->getName(), statementNumber);
      int startOfExpression = 2;  // 0 -> variable, 1 -> =, 2 -> ....
      std::vector<std::shared_ptr<Token>> exprTokens(
          statementContents.begin() + startOfExpression,
          statementContents.end());
      // Evaluate statements
      std::shared_ptr<AssignNode> assignNode = std::make_shared<AssignNode>(
          statementNumber, varNode, parseExpr(exprTokens, statementNumber));
      return assignNode;
    } else if (keywordToken->getName() == "call") {
      shared_ptr<CallNode> callNode;
      string procedureName = statementContents[secondIndex]->getName();
      if (containsProcedure(procedureName, ASTBuilder::procedureMap)) {
        callNode = make_shared<CallNode>(
            statementNumber, ASTBuilder::procedureMap[procedureName]);
        return callNode;
      } else {
        vector<shared_ptr<StatementNode>> dummyStatements;
        shared_ptr<ProcedureNode> dummyProcNode =
            make_shared<ProcedureNode>("dummyProc", dummyStatements);
        callNode = make_shared<CallNode>(statementNumber, dummyProcNode);
        ASTBuilder::callMap.insert(
            {procedureName, callNode});  // store pointer to callNode, will use
                                         // pointer to update the callNode after
                                         // all procedures have been parsed
        return callNode;
      }
    } else if (keywordToken->getName() == "print") {
      shared_ptr<VariableNode> variableNode = make_shared<VariableNode>(
          statementContents[secondIndex]->getName(), statementNumber);
      shared_ptr<PrintNode> printNode =
          make_shared<PrintNode>(statementNumber, variableNode);
      return printNode;
    } else {  // read
      shared_ptr<VariableNode> variableNode = make_shared<VariableNode>(
          statementContents[secondIndex]->getName(), statementNumber);
      shared_ptr<ReadNode> readNode =
          make_shared<ReadNode>(statementNumber, variableNode);
      return readNode;
    }
  } else {  // TokenizedConditionalStmt
    if (auto tokenizedIfStmt =
            dynamic_pointer_cast<TokenizedIfStmt>(statement)) {
      vector<shared_ptr<Token>> condExprTokens =
          tokenizedIfStmt->getConditionalExp();
      shared_ptr<CondExprNode> condExpr =
          parseCondExpr(condExprTokens, statementNumber);

      vector<shared_ptr<TokenizedStmt>> tokenizedThenSmts =
          tokenizedIfStmt->getThenBlock()->getStmts();
      vector<shared_ptr<StatementNode>> thenStmts;
      for (shared_ptr<TokenizedStmt> thenStatement : tokenizedThenSmts) {
        thenStmts.emplace_back(parseStatement(thenStatement));
      }

      vector<shared_ptr<TokenizedStmt>> tokenizedElseStmts =
          tokenizedIfStmt->getElseBlock()->getStmts();
      vector<shared_ptr<StatementNode>> elseStmts;
      for (shared_ptr<TokenizedStmt> elseStatement : tokenizedElseStmts) {
        elseStmts.emplace_back(parseStatement(elseStatement));
      }
      shared_ptr<IfNode> ifNode =
          make_shared<IfNode>(tokenizedIfStmt->getStatementNumber(), condExpr,
                              thenStmts, elseStmts);
      return ifNode;
    } else {  // TokenizedWhileStmt
      shared_ptr<TokenizedWhileStmt> tokenizedWhileStmt =
          dynamic_pointer_cast<TokenizedWhileStmt>(statement);
      vector<shared_ptr<Token>> condExprTokens =
          tokenizedWhileStmt->getConditionalExp();
      shared_ptr<CondExprNode> condExpr =
          parseCondExpr(condExprTokens, statementNumber);

      vector<shared_ptr<TokenizedStmt>> tokenizedLoopStmts =
          tokenizedWhileStmt->getWhileBlock()->getStmts();
      vector<shared_ptr<StatementNode>> loopStmts;
      for (shared_ptr<TokenizedStmt> loopStatement : tokenizedLoopStmts) {
        loopStmts.emplace_back(parseStatement(loopStatement));
      }
      shared_ptr<WhileNode> whileNode = make_shared<WhileNode>(
          tokenizedWhileStmt->getStatementNumber(), condExpr, loopStmts);
      return whileNode;
    }
  }
}

static void applyExprNodeStackOperation(stack<shared_ptr<ExprNode>>& values,
                                        stack<string>& ops) {
  if (values.size() <
      2) {  // at least 2 items in values to apply mathematical operation
    throw runtime_error("Need 2 operatees to operate on.");
  }
  shared_ptr<ExprNode> b = values.top();
  values.pop();
  shared_ptr<ExprNode> a = values.top();
  values.pop();
  values.push(applyOperation(ops.top(), a, b));
  ops.pop();
}

// Creates a tree of ExprNode which forms one big expression
// Uses 2 stacks - One to store the operators and paranthesis, one to store the
// expressions/ExprNode Once a ")" is reached, the top operator is popped and
// the top 2 expressions are also popped,then the applyoperation
//  method takes in these 3 inputs and outputs the combined expression
shared_ptr<ExprNode> ASTBuilder::parseExpr(
    vector<shared_ptr<Token>> listOfTokens, int statementNumber) {
  stack<shared_ptr<ExprNode>> values;
  stack<string> ops;

  for (int i = 0; i < listOfTokens.size(); ++i) {
    shared_ptr<Token> currToken = listOfTokens[i];
    if (currToken->getName() == "(") {
      ops.push("(");
    } else if (currToken->getName() == ")") {
      while (!ops.empty() && ops.top() != "(") {
        try {
          applyExprNodeStackOperation(values, ops);
        } catch (const runtime_error& e) {
          cerr << e.what() << endl;
          return nullptr;
        }
      }
      if (ops.empty()) {
        throw runtime_error("Missing left bracket.");
      }
      ops.pop();  // Remove the '('
    } else if (isAlphanumeric(currToken->getName())) {
      shared_ptr<ExprNode> refNode;
      if (isNumber(currToken->getName()))
        refNode = make_shared<ConstantNode>(stoi(currToken->getName()),
                                            statementNumber);
      else
        refNode =
            make_shared<VariableNode>(currToken->getName(), statementNumber);
      values.push(refNode);
    } else {
      while (!ops.empty() &&
             precedence(ops.top()) >= precedence(currToken->getName())) {
        try {
          applyExprNodeStackOperation(values, ops);
        } catch (const runtime_error& e) {
          cerr << e.what() << endl;
          return nullptr;
        }
      }
      ops.push(currToken->getName());
    }
  }
  while (!ops.empty()) {
    try {
      applyExprNodeStackOperation(values, ops);
    } catch (const runtime_error& e) {
      cerr << e.what() << endl;
      return nullptr;
    }
  }

  return values.top();
}

static void applyCondExprNodeStackOperation(
    stack<shared_ptr<CondExprNode>>& condValues,
    stack<shared_ptr<ExprNode>>& relValues, stack<string>& ops) {
  if (isComparisonOpr(ops.top())) {
    if (relValues.size() < 2) {
      throw runtime_error("Need 2 comparison operatees to operate on.");
    }
    shared_ptr<ExprNode> b = relValues.top();
    relValues.pop();
    shared_ptr<ExprNode> a = relValues.top();
    relValues.pop();
    condValues.push(applyCompOperation(ops.top(), a, b));
    ops.pop();
  } else if (isBoolOpr(ops.top())) {
    if (condValues.empty()) {
      throw runtime_error("Missing conditional operatee.");
    }

    shared_ptr<CondExprNode> b = condValues.top();
    condValues.pop();
    if (ops.top() == "!") {
      condValues.push(
          applyBoolOperation(ops.top(), shared_ptr<CondExprNode>(), b));
    } else {
      if (condValues.empty()) {
        throw runtime_error("Missing conditional operatee.");
      }
      shared_ptr<CondExprNode> a = condValues.top();
      condValues.pop();
      condValues.push(applyBoolOperation(ops.top(), a, b));
    }
    ops.pop();
  } else {
    if (relValues.size() < 2) {
      throw runtime_error("Need 2 operatees to operate on.");
    }
    shared_ptr<ExprNode> b = relValues.top();
    relValues.pop();
    shared_ptr<ExprNode> a = relValues.top();
    relValues.pop();
    relValues.push(applyOperation(ops.top(), a, b));
    ops.pop();
  }
}

shared_ptr<CondExprNode> ASTBuilder::parseCondExpr(
    vector<shared_ptr<Token>> listOfTokens, int statementNumber) {
  stack<shared_ptr<CondExprNode>> condValues;
  stack<shared_ptr<ExprNode>> relValues;
  stack<string> ops;

  for (int i = 0; i < listOfTokens.size(); ++i) {
    shared_ptr<Token> currToken = listOfTokens[i];
    if (currToken->getName() == "(") {
      ops.push("(");
    } else if (currToken->getName() == ")") {
      while (!ops.empty() && ops.top() != "(") {
        try {
          applyCondExprNodeStackOperation(condValues, relValues, ops);
        } catch (const runtime_error& e) {
          cerr << e.what() << endl;
          return nullptr;
        }
      }
      ops.pop();  // Remove the '('
    } else if (isAlphanumeric(currToken->getName())) {
      shared_ptr<ExprNode> refNode;
      if (isNumber(currToken->getName()))
        refNode = make_shared<ConstantNode>(stoi(currToken->getName()),
                                            statementNumber);
      else
        refNode =
            make_shared<VariableNode>(currToken->getName(), statementNumber);
      relValues.push(refNode);
    } else {
      while (!ops.empty() &&
             precedence(ops.top()) >= precedence(currToken->getName())) {
        try {
          applyCondExprNodeStackOperation(condValues, relValues, ops);
        } catch (const runtime_error& e) {
          cerr << e.what() << endl;
          return nullptr;
        }
      }
      ops.push(currToken->getName());
    }
  }
  while (!ops.empty()) {
    try {
      applyCondExprNodeStackOperation(condValues, relValues, ops);
    } catch (const runtime_error& e) {
      cerr << e.what() << endl;
      return nullptr;
    }
  }

  return condValues.top();
}