#include "SimpleProcessor.h"
#include "ExpressionProcessor.h"

#include "ProcessedProcedure.h"
#include "ProcessedConditionalStmt.h"
#include "ProcessedIfStmt.h"
#include "ProcessedWhileStmt.h"
#include "ProcessedAssignStmt.h"
#include "ProcessedReadStmt.h"
#include "ProcessedPrintStmt.h"
#include "ProcessedCallStmt.h"

#include "../SimpleTokens/TokenFactory.h"
#include "../../ExceptionMessages.h"
#include "../../HelperFunctions.h"
#include "../../Constants/SPConstants.h"

// evaluates the conditional expression, returning the type of conditional statement and a vector containing the conditional expression
pair<SimpleProcessor::ConditionalDeclaration, shared_ptr<Node>> SimpleProcessor::processConditionalExpression(string trimmed, bool isIf, int stmtNum) {
	ConditionalDeclaration dec;
	int afterKeyword;
	int beforeLast;
	if (isIf) {
		dec = ConditionalDeclaration::IF;
		afterKeyword = trimmed.find(IF_STRING) + IF_STR_LEN;
		beforeLast = trimmed.rfind(THEN_STRING) - 1; // to get to the character before "then"
	}
	else {
		dec = ConditionalDeclaration::WHILE;
		afterKeyword = trimmed.find(WHILE_STRING) + WHILE_STR_LEN;
		beforeLast = trimmed.size() - 1; // to get to the character before the open curly for while block
	}
	string inBetween = trimWhitespaces(substring(trimmed, afterKeyword, beforeLast));
	bool parenFormatting = inBetween[0] == OPEN_BRACKET_CHAR && inBetween.back() == CLOSE_BRACKET_CHAR;
	if (!parenFormatting) {
		throw invalid_argument(ExceptionMessages::invalidConditionalDeclaration);
	}

	ExpressionProcessor ep = ExpressionProcessor(stmtNum);
	shared_ptr<Node> condExp = ep.nodifyConditionalExpression(substring(inBetween, SEPARATOR_LEN, inBetween.size() - SEPARATOR_LEN - 1));
	return pair(dec, condExp);
}

pair<SimpleProcessor::ConditionalDeclaration, shared_ptr<Node>> SimpleProcessor::processConditionalDeclaration(string declaration, int stmtNum) {
	string trimmed = trimWhitespaces(declaration);
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::emptyStatementGiven);
	}

	// find first word
	// if "if", find last word, it should be "then"
	// if "while", do nothing
	// finally process conditional exp

	// Determine first word and type of conditional statement
	bool firstWordWhile = trimmed.find_first_not_of(whitespaces) == trimmed.find(WHILE_STRING);
	bool firstWordIf = trimmed.find_first_not_of(whitespaces) == trimmed.find(IF_STRING);
	bool isIf = false;
	if (firstWordIf) {
		// difference of minus_one because rfind will find the position of the "t" in "then", but find_last_not_of will find position of "n".
		bool lastWordThen = (trimmed.find_last_not_of(whitespaces) - THEN_STR_LEN_MINUS_ONE) == trimmed.rfind(THEN_STRING);
		if (!lastWordThen) {
			throw invalid_argument(ExceptionMessages::invalidIfDeclaration);
		}
		isIf = true;
	}
	if (!firstWordIf && !firstWordWhile) {
		throw invalid_argument(ExceptionMessages::invalidConditionalDeclaration);
	}

	return processConditionalExpression(trimmed, isIf, stmtNum);
}

shared_ptr<ProcessedStmtList> SimpleProcessor::processStmtList(string stmtList) {
	string trimmed = trimWhitespaces(string(stmtList));
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::emptyStatementListGiven);
	}
	vector<shared_ptr<pair<int, int>>> curlyPairs = outermostSepDetector(trimmed, Separator::CURLY); // this should detect { and } for if-then-else and while
	vector<shared_ptr<ProcessedStmt>> out;

	size_t prevEnd = 0;
	size_t pairIndex = 0;
	while (pairIndex < curlyPairs.size()) {
		pair<int, int> currPair = *(curlyPairs[pairIndex]);

		///// Process the part that is before the conditional statement: <here>read r; call c; ... ;</here> <IF|WHILE> <condExp> { }
		string preCurly = substring(trimmed, prevEnd, currPair.first - SEPARATOR_LEN);
		// this should contain no semicolons and no { }. Should also be at least size 1 due to conditional declaration
		vector<string> preCurlySplit = splitString(preCurly, SEMICOLON_STRING, false);
		if (preCurlySplit.empty()) {
			throw invalid_argument(ExceptionMessages::missingConditionalDeclaration);
		}
		// process each semicolon statement. Last element is not a semicolon statement due to conditional declaration
		for (size_t stmtIndex = 0; stmtIndex < preCurlySplit.size() - 1; stmtIndex++) {
			out.push_back(processSemicolonStatement(preCurlySplit[stmtIndex]));
		}

		// Increment and save it here first. Will be used once the IF/WHILE statements' body stmtLists are completely defined.
		SimpleProcessor::statementNumber++;
		const int conditionalStmtNumber = SimpleProcessor::statementNumber;

		///// Process the part that is within the conditional statement <here><IF|WHILE> <conditional></here> { }
		string lastPreCurly = preCurlySplit.back();
		pair<ConditionalDeclaration, shared_ptr<Node>> processed = processConditionalDeclaration(lastPreCurly, conditionalStmtNumber);
		ConditionalDeclaration dec = processed.first;

		// Processed StmtList for either the THEN-BLOCK or WHILE-BLOCK
		shared_ptr<ProcessedStmtList> firstBody = processStmtList(substring(trimmed, currPair.first + SEPARATOR_LEN, currPair.second - SEPARATOR_LEN));
		if (dec == ConditionalDeclaration::WHILE) {
			// process statementList within while body
			out.push_back(make_shared<ProcessedWhileStmt>(conditionalStmtNumber, processed.second, firstBody));
			prevEnd = currPair.second + SEPARATOR_LEN;
			pairIndex++;
			continue;
		}

		///// Look ahead to the next pair to process the ELSE-BLOCK
		if (dec == ConditionalDeclaration::IF) {
			pairIndex++;
			if (pairIndex >= curlyPairs.size()) {
				throw invalid_argument(ExceptionMessages::missingElseBlock);
			}
			pair<int, int> nextPair = *(curlyPairs[pairIndex]);
			string inBetweenCurly = substring(trimmed, currPair.second + SEPARATOR_LEN, nextPair.first - SEPARATOR_LEN);
			bool isElseString = trimWhitespaces(inBetweenCurly) == ELSE_STRING;
			if (!isElseString) {
				throw invalid_argument(ExceptionMessages::missingElseKeyword);
			}
			shared_ptr<ProcessedStmtList> elseBody = processStmtList(substring(trimmed, nextPair.first + SEPARATOR_LEN, nextPair.second - SEPARATOR_LEN));
			out.push_back(make_shared<ProcessedIfStmt>(conditionalStmtNumber, processed.second, firstBody, elseBody));
			prevEnd = nextPair.second + SEPARATOR_LEN;
			pairIndex++;
			continue; // not needed but added for clarity
		}
	}
	if (prevEnd < trimmed.size()) {
		// process any leftover code after all curly braces
		string afterAllCurly = substring(trimmed, prevEnd, trimmed.size() - 1);
		// this should contain no semicolons and no { }
		vector<string> afterCurlySplit = splitString(afterAllCurly, SEMICOLON_STRING, false);
		// process each semicolon statement.
		for (string stmt : afterCurlySplit) {
			out.push_back(processSemicolonStatement(stmt));
		}
	}
	if (out.empty()) {
		throw invalid_argument(ExceptionMessages::emptyStatementListGiven);
	}

	return make_shared<ProcessedStmtList>(out);
}

shared_ptr<ProcessedSemicolonStmt> SimpleProcessor::processSemicolonStatement(string stmt) {
	string trimmed = trimWhitespaces(stmt);
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::emptyStatementGiven);
	}

	// Prioritise Assignment statement processing first
	if (trimmed.find(EQUALS_STRING) != string::npos) {
		return processAssignmentStmt(trimmed);
	}
	else {
		return processTwoWordStmt(trimmed);
	}
}

shared_ptr<ProcessedSemicolonStmt> SimpleProcessor::processTwoWordStmt(string twoWordStmt) {
	vector<string> split = splitString(twoWordStmt);
	if (split.size() != 2) {
		throw invalid_argument(ExceptionMessages::invalidSemicolonStmt);
	}
	if (!isValidName(split[1])) {
		throw invalid_argument(ExceptionMessages::invalidIdentifier);
	}

	shared_ptr<ProcessedSemicolonStmt> out;
	shared_ptr<Token> second = TokenFactory::generateTokenForSimple(trimWhitespaces(split[1]));
	int temp = SimpleProcessor::statementNumber; // store it here and increment first in case the two-word-stmt is invalid
	temp++;
	// if the substring "read " exists and it is the first word occurrence in the statement
	if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find(READ_WHITESPACE_STRING)) {
		out = make_shared<ProcessedReadStmt>(temp, second);
	}
	// if the substring "print " exists and it is the first word occurrence in the statement
	else if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find(PRINT_WHITESPACE_STRING)) {
		out = make_shared<ProcessedPrintStmt>(temp, second);
	}
	// if the substring "call " exists and it is the first word occurrence in the statement
	else if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find(CALL_WHITESPACE_STRING)) {
		out = make_shared<ProcessedCallStmt>(temp, second);
	}
	else {
		throw invalid_argument(ExceptionMessages::invalidSemicolonStmt);
	}
	SimpleProcessor::statementNumber++; // valid, therefore increment the static variable
	return out;
}

shared_ptr<ProcessedAssignStmt> SimpleProcessor::processAssignmentStmt(string assStmt) {
	vector<shared_ptr<Token>> output;
	vector<string> splitByEquals = splitString(assStmt, EQUALS_STRING);
	if (splitByEquals.size() != 2) {
		// Invalid assignment statement
		throw invalid_argument(ExceptionMessages::invalidAssignmentStmt);
	}
	string trimmedLeft = trimWhitespaces(splitByEquals[0]);
	if (!isValidName(trimmedLeft)) {
		throw invalid_argument(ExceptionMessages::invalidIdentifier);
	}
	shared_ptr<Token> left = TokenFactory::generateTokenForSimple(trimmedLeft, true);
	SimpleProcessor::statementNumber++;
	ExpressionProcessor ep = ExpressionProcessor(SimpleProcessor::statementNumber);
	shared_ptr<Node> right = ep.nodifyArithmeticExpression(splitByEquals[1]);

	return make_shared<ProcessedAssignStmt>(SimpleProcessor::statementNumber, left, right);
}

string SimpleProcessor::processPreProcedureDetails(string input) {
	string trimmed = trimWhitespaces(input);
	vector<string> beforeOpenSplit = splitString(trimmed);
	if (beforeOpenSplit.size() != 2) {
		throw invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	string procedureKeyword = trimWhitespaces(beforeOpenSplit[0]);
	if (procedureKeyword != PROCEDURE_STRING) {
		throw invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	string procName = trimWhitespaces(beforeOpenSplit[1]);
	if (!isValidName(procName)) {
		throw invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	return procName;
}

shared_ptr<ProcessedProgram> SimpleProcessor::processProgram(string_view srcCode) {
	SimpleProcessor::statementNumber = 0;
	string trimmed = trimWhitespaces(string(srcCode));
	if (trimmed.empty()) {
		throw invalid_argument(ExceptionMessages::emptyProgramGiven);
	}
	vector<shared_ptr<ProcessedProcedure>> out;

	vector<shared_ptr<pair<int, int>>> curlyPairs = outermostSepDetector(trimmed, Separator::CURLY);
	int previousEnd = 0;
	for (shared_ptr<pair<int, int>> shared_ptr_p : curlyPairs) {
		pair<int, int> p = *shared_ptr_p;
		if (p.second == p.first + 1) {
			throw invalid_argument(ExceptionMessages::emptyStatementListGiven);
		}

		// Process Procedure Name using string indexes
		string beforeOpen = substring(trimmed, previousEnd, p.first - SEPARATOR_LEN);
		string procName = processPreProcedureDetails(beforeOpen);
		if (procName == "") {
			return nullptr;
		}

		// Process Procedure Body using string indexes and function
		string body = substring(trimmed, p.first + SEPARATOR_LEN, p.second - SEPARATOR_LEN);
		shared_ptr<ProcessedStmtList> stmtList = processStmtList(body);

		out.push_back(make_shared<ProcessedProcedure>(procName, stmtList));

		previousEnd = p.second + SEPARATOR_LEN;
	}
	// There are leftover characters. Which should not be the case since the program was trimmed of whitespaces
	// already.
	if (previousEnd != trimmed.size()) {
		throw invalid_argument(ExceptionMessages::invalidProgramDefinition);
	}
	return make_shared<ProcessedProgram>(out);
}
