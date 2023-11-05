#ifndef SIMPLEPROCESSOR_H
#define SIMPLEPROCESSOR_H

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "ProcessedProgram.h"
#include "ProcessedStmtList.h"
#include "ProcessedSemicolonStmt.h"
#include "ProcessedAssignStmt.h"
#include "../SimpleTokens/Token.h"
#include "../AST/Node.h"

using namespace std;

class SimpleProcessor {
private:
	// Keeps track of the statement number as the tokenizer results are being generated
	inline static int statementNumber = 0;

	// Simple enum to be used in utility functions
	enum ConditionalDeclaration {
		IF = 0,
		WHILE = 1
	};
	// processes and evaluates the conditional expression, returning the type of conditional statement and a vector containing the conditional expression
	static pair<SimpleProcessor::ConditionalDeclaration, shared_ptr<Node>> processConditionalExpression(string trimmed, bool isIf, int stmtNum);

	// Processes and validates the conditional statement declaration of a conditional statement
	static pair<SimpleProcessor::ConditionalDeclaration, shared_ptr<Node>> processConditionalDeclaration(string declaration, int stmtNum);
	
	// Processes a Statement List, which can be found in either IF/WHILE or Procedures
	static shared_ptr<ProcessedStmtList> processStmtList(string stmtList);

	// Processes statements that end with a semicolon.
	static shared_ptr<ProcessedSemicolonStmt> processSemicolonStatement(string stmt);

	// Processes two-word statements
	static shared_ptr<ProcessedSemicolonStmt> processTwoWordStmt(string twoWordStmt);

	// Processes an assignment statement
	static shared_ptr<ProcessedAssignStmt> processAssignmentStmt(string assStmt);

	// Processes and validates the details that are found before the {} of a procedure.
	// throws an invalid_argument exception
	// Returns the procedure name
	static string processPreProcedureDetails(string input);

public:
	/// <summary>
	/// Processes a SIMPLE source code into a ProcessedProgram.
	/// 
	/// See ProcessedX for the details of the class.
	/// </summary>
	/// <param name="srcCode">The source code in string view form</param>
	/// <exception cref="invalid_argument">Thrown when the source code is syntactically wrong</exception>
	/// <returns>A shared pointer to the ProcessedProgram</returns>
	static shared_ptr<ProcessedProgram> processProgram(string_view srcCode);
};
#endif
