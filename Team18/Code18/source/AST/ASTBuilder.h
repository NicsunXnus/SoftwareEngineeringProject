#ifndef ASTBUILDER_H
#define ASTBUILDER_H

#include <algorithm>
#include <map>
#include <stack>

#include "../HelperFunctions.h"
#include "../SimpleTokenizer/TokenizedConditionalStmt.h"
#include "../SimpleTokenizer/TokenizedProcedure.h"
#include "../SimpleTokenizer/TokenizedProgram.h"
#include "../SimpleTokenizer/TokenizedSemicolonStmt.h"
#include "../SimpleTokens/TokenFactory.h"
#include "ASTBuilderHelperFunctions.h"
#include "ASTNode.h"
using namespace std;
class ASTBuilder {
 public:
  // UNLOCK IN MILESTONE 2
  inline static multimap<string, shared_ptr<CallNode>>
      callMap;  // Map to store all callNodes calling procedures that have not
                // been parsed
  inline static map<string, shared_ptr<ProcedureNode>>
      procedureMap;  // Map to store all procedureNodes for callNodes reference

  static shared_ptr<ProgramNode> parseProgram(
      shared_ptr<TokenizedProgram> tokenizedProgram);
  static shared_ptr<ExprNode> parseExpr(vector<shared_ptr<Token>> listOfTokens,
                                        int statementNumber);
  static shared_ptr<CondExprNode> parseCondExpr(
      vector<shared_ptr<Token>> listOfTokens, int statementNumber);
  static shared_ptr<StatementNode> parseStatement(
      shared_ptr<TokenizedStmt> statement);
};
#endif