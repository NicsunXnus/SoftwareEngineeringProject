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

using namespace std::string_view_literals;

class SimpleProcessor {
private:
	// Keeps track of the statement number as the tokenizer results are being generated
	inline static int statementNumber = 0;

	// Simple enum to be used in utility functions
	static enum ConditionalDeclaration {
		IF,
		WHILE
	};
	// processes and evaluates the conditional expression, returning the type of conditional statement and a vector containing the conditional expression
	static std::pair<SimpleProcessor::ConditionalDeclaration, std::shared_ptr<Node>> processConditionalExpression(std::string trimmed, bool isIf, int stmtNum);

	// Processes and validates the conditional statement declaration of a conditional statement
	static std::pair<SimpleProcessor::ConditionalDeclaration, std::shared_ptr<Node>> processConditionalDeclaration(std::string declaration, int stmtNum);
	
	// Processes a Statement List, which can be found in either IF/WHILE or Procedures
	static std::shared_ptr<ProcessedStmtList> processStmtList(std::string stmtList);

	// Processes statements that end with a semicolon.
	static std::shared_ptr<ProcessedSemicolonStmt> processSemicolonStatement(std::string stmt);

	// Processes two-word statements
	static std::shared_ptr<ProcessedSemicolonStmt> processTwoWordStmt(std::string twoWordStmt);

	// Processes an assignment statement
	static std::shared_ptr<ProcessedAssignStmt> processAssignmentStmt(std::string assStmt);
	//static std::pair<std::shared_ptr<Token>, std::shared_ptr<Node>> processAssignmentStmt(std::string assStmt);

	// Processes and validates the details that are found before the {} of a procedure.
	// throws an std::invalid_argument exception
	// Returns the procedure name
	static std::string processPreProcedureDetails(std::string input);

public:
	/// <summary>
	/// Processes a SIMPLE source code into a ProcessedProgram.
	/// 
	/// See ProcessedX for the details of the class.
	/// </summary>
	/// <param name="srcCode">The source code in string view form</param>
	/// <exception cref="std::invalid_argument">Thrown when the source code is syntactically wrong</exception>
	/// <returns>A shared pointer to the ProcessedProgram</returns>
	static std::shared_ptr<ProcessedProgram> processProgram(std::string_view srcCode);
};
#endif
