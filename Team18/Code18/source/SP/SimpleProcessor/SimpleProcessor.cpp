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

// evaluates the conditional expression, returning the type of conditional statement and a vector containing the conditional expression
std::pair<SimpleProcessor::ConditionalDeclaration, std::shared_ptr<Node>> SimpleProcessor::processConditionalExpression(std::string trimmed, bool isIf, int stmtNum) {
	ConditionalDeclaration dec;
	int afterKeyword;
	int beforeLast;
	if (isIf) {
		dec = ConditionalDeclaration::IF;
		afterKeyword = trimmed.find("if") + 2; // because if is 2 characters long
		beforeLast = trimmed.rfind("then") - 1; // to get to the character before "then"
	}
	else {
		dec = ConditionalDeclaration::WHILE;
		afterKeyword = trimmed.find("while") + 5; // because while is 5 characters long
		beforeLast = trimmed.size() - 1; // last character 
	}
	std::string inBetween = trimWhitespaces(substring(trimmed, afterKeyword, beforeLast));
	bool parenFormatting = inBetween[0] == '(' && inBetween.back() == ')';
	if (!parenFormatting) {
		throw std::invalid_argument(ExceptionMessages::invalidConditionalDeclaration);
	}

	// 1 and -2 to remove the parentheses
	ExpressionProcessor ep = ExpressionProcessor(stmtNum);
	std::shared_ptr<Node> condExp = ep.nodifyConditionalExpression(substring(inBetween, 1, inBetween.size() - 2));
	return std::pair(dec, condExp);
}

std::pair<SimpleProcessor::ConditionalDeclaration, std::shared_ptr<Node>> SimpleProcessor::processConditionalDeclaration(std::string declaration, int stmtNum) {
	std::string trimmed = trimWhitespaces(declaration);
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::emptyStatementGiven);
	}

	// find first word
	// if "if", find last word, it should be "then"
	// if "while", do nothing
	// finally process conditional exp

	// Determine first word and type of conditional statement
	bool firstWordWhile = trimmed.find_first_not_of(whitespaces) == trimmed.find("while");
	bool firstWordIf = trimmed.find_first_not_of(whitespaces) == trimmed.find("if");
	bool isIf = false;
	if (firstWordIf) {
		// minus 3 because rfind will find the position of the "t" in "then", but find_last_not_of will find position of "n".
		// minus exactly 3 because "then" is 4 letters, so 3 letter diff between "t" and "n"
		bool lastWordThen = (trimmed.find_last_not_of(whitespaces) - 3) == trimmed.rfind("then");
		if (!lastWordThen) {
			throw std::invalid_argument(ExceptionMessages::invalidIfDeclaration);
		}
		isIf = true;
	}
	if (!firstWordIf && !firstWordWhile) {
		throw std::invalid_argument(ExceptionMessages::invalidConditionalDeclaration);
	}

	return processConditionalExpression(trimmed, isIf, stmtNum);
}

std::shared_ptr<ProcessedStmtList> SimpleProcessor::processStmtList(std::string stmtList) {
	std::string trimmed = trimWhitespaces(std::string(stmtList));
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::emptyStatementListGiven);
	}
	std::vector<std::shared_ptr<std::pair<int, int>>> curlyPairs = outermostSepDetector(trimmed, Separator::CURLY); // this should detect { and } for if-then-else and while
	std::vector<std::shared_ptr<ProcessedStmt>> out;

	int prevEnd = 0;
	int pairIndex = 0;
	while (pairIndex < curlyPairs.size()) {
		std::pair<int, int> currPair = *(curlyPairs[pairIndex]);

		///// Process the part that is before the conditional statement: <IF|WHILE> <conditional> { }
		std::string preCurly = substring(trimmed, prevEnd, currPair.first - 1);
		// this should contain no semicolons and no { }. Should also be at least size 1 due to conditional declaration
		std::vector<std::string> preCurlySplit = splitString(preCurly, ";", false);
		if (preCurlySplit.empty()) {
			throw std::invalid_argument(ExceptionMessages::missingConditionalDeclaration);
		}
		// process each semicolon statement. Last element is not a semicolon statement due to conditional declaration
		for (int stmtIndex = 0; stmtIndex < preCurlySplit.size() - 1; stmtIndex++) {
			out.push_back(processSemicolonStatement(preCurlySplit[stmtIndex]));
		}

		// Increment and save it here first. Will be used once the IF/WHILE statements' body stmtLists are completely defined.
		SimpleProcessor::statementNumber += 1;
		const int conditionalStmtNumber = SimpleProcessor::statementNumber;

		///// Process the part that is within the conditional statement <IF|WHILE> <conditional> { }
		std::string lastPreCurly = preCurlySplit.back();
		std::pair<ConditionalDeclaration, std::shared_ptr<Node>> processed = processConditionalDeclaration(lastPreCurly, conditionalStmtNumber);
		ConditionalDeclaration dec = processed.first;

		// Processed StmtList for either the THEN-BLOCK or WHILE-BLOCK
		std::shared_ptr<ProcessedStmtList> firstBody = processStmtList(substring(trimmed, currPair.first + 1, currPair.second - 1));
		if (dec == ConditionalDeclaration::WHILE) {
			// process statementList within while body
			out.push_back(std::make_shared<ProcessedWhileStmt>(conditionalStmtNumber, processed.second, firstBody));
			prevEnd = currPair.second + 1;
			pairIndex += 1;
			continue;
		}

		///// Look ahead to the next pair to process the ELSE-BLOCK
		if (dec == ConditionalDeclaration::IF) {
			pairIndex += 1;
			if (pairIndex >= curlyPairs.size()) {
				throw std::invalid_argument(ExceptionMessages::missingElseBlock);
			}
			std::pair<int, int> nextPair = *(curlyPairs[pairIndex]);
			std::string inBetweenCurly = substring(trimmed, currPair.second + 1, nextPair.first - 1);
			bool isElseString = trimWhitespaces(inBetweenCurly) == "else";
			if (!isElseString) {
				throw std::invalid_argument(ExceptionMessages::missingElseKeyword);
			}
			std::shared_ptr<ProcessedStmtList> elseBody = processStmtList(substring(trimmed, nextPair.first + 1, nextPair.second - 1));
			out.push_back(std::make_shared<ProcessedIfStmt>(conditionalStmtNumber, processed.second, firstBody, elseBody));
			prevEnd = nextPair.second + 1;
			pairIndex += 1;
			continue; // not needed but added for clarity
		}
	}
	if (prevEnd < trimmed.size()) {
		// process any leftover code after all curly braces
		std::string afterAllCurly = substring(trimmed, prevEnd, trimmed.size() - 1);
		// this should contain no semicolons and no { }
		std::vector<std::string> afterCurlySplit = splitString(afterAllCurly, ";", false);
		// process each semicolon statement.
		for (std::string stmt : afterCurlySplit) {
			out.push_back(processSemicolonStatement(stmt));
		}
	}
	if (out.empty()) {
		throw std::invalid_argument(ExceptionMessages::emptyStatementListGiven);
	}

	return std::make_shared<ProcessedStmtList>(out);
}

std::shared_ptr<ProcessedSemicolonStmt> SimpleProcessor::processSemicolonStatement(std::string stmt) {
	std::string trimmed = trimWhitespaces(stmt);
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::emptyStatementGiven);
	}

	// Prioritise Assignment statement processing first
	if (trimmed.find("=") != std::string::npos) {
		return processAssignmentStmt(trimmed);
	}
	else {
		return processTwoWordStmt(trimmed);
	}
}

std::shared_ptr<ProcessedSemicolonStmt> SimpleProcessor::processTwoWordStmt(std::string twoWordStmt) {
	std::vector<std::string> split = splitString(twoWordStmt);
	if (split.size() != 2) {
		throw std::invalid_argument(ExceptionMessages::invalidSemicolonStmt);
	}
	if (!isValidName(split[1])) {
		throw std::invalid_argument(ExceptionMessages::invalidIdentifier);
	}

	std::shared_ptr<ProcessedSemicolonStmt> out;
	std::shared_ptr<Token> second = TokenFactory::generateTokenForSimple(trimWhitespaces(split[1]));
	int temp = SimpleProcessor::statementNumber; // store it here and increment first in case the two-word-stmt is invalid
	temp += 1;
	// if the substring "read " exists and it is the first word occurence in the statement
	if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find("read ")) {
		out = std::make_shared<ProcessedReadStmt>(temp, second);
	}
	// if the substring "print " exists and it is the first word occurence in the statement
	else if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find("print ")) {
		out = std::make_shared<ProcessedPrintStmt>(temp, second);
	}
	// if the substring "call " exists and it is the first word occurence in the statement
	else if (twoWordStmt.find_first_not_of(whitespaces) == twoWordStmt.find("call ")) {
		out = std::make_shared<ProcessedCallStmt>(temp, second);
	}
	else {
		throw std::invalid_argument(ExceptionMessages::invalidSemicolonStmt);
	}
	SimpleProcessor::statementNumber += 1; // valid, therefore increment the static variable
	return out;
}

std::shared_ptr<ProcessedAssignStmt> SimpleProcessor::processAssignmentStmt(std::string assStmt) {
	std::vector<std::shared_ptr<Token>> output;
	std::vector<std::string> splitByEquals = splitString(assStmt, "=");
	if (splitByEquals.size() != 2) {
		// Invalid assignment statement
		throw std::invalid_argument(ExceptionMessages::invalidAssignmentStmt);
	}
	std::string trimmedLeft = trimWhitespaces(splitByEquals[0]);
	if (!isValidName(trimmedLeft)) {
		throw std::invalid_argument(ExceptionMessages::invalidIdentifier);
	}
	std::shared_ptr<Token> left = TokenFactory::generateTokenForSimple(trimmedLeft, true);
	SimpleProcessor::statementNumber += 1;
	ExpressionProcessor ep = ExpressionProcessor(SimpleProcessor::statementNumber);
	std::shared_ptr<Node> right = ep.nodifyArithmeticExpression(splitByEquals[1]);

	return std::make_shared<ProcessedAssignStmt>(SimpleProcessor::statementNumber, left, right);
}

std::string SimpleProcessor::processPreProcedureDetails(std::string input) {
	std::string trimmed = trimWhitespaces(input);
	std::vector<std::string> beforeOpenSplit = splitString(trimmed);
	if (beforeOpenSplit.size() != 2) {
		throw std::invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	std::string procedureKeyword = trimWhitespaces(beforeOpenSplit[0]);
	if (procedureKeyword != "procedure") {
		throw std::invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	std::string procName = trimWhitespaces(beforeOpenSplit[1]);
	if (!isValidName(procName)) {
		throw std::invalid_argument(ExceptionMessages::invalidProcedureDefinition);
	}
	return procName;
}

std::shared_ptr<ProcessedProgram> SimpleProcessor::processProgram(std::string_view srcCode) {
	SimpleProcessor::statementNumber = 0;
	std::string trimmed = trimWhitespaces(std::string(srcCode));
	if (trimmed.empty()) {
		throw std::invalid_argument(ExceptionMessages::emptyProgramGiven);
	}
	std::vector<std::shared_ptr<ProcessedProcedure>> out;

	std::vector<std::shared_ptr<std::pair<int, int>>> curlyPairs = outermostSepDetector(trimmed, Separator::CURLY);
	int previousEnd = 0;
	for (std::shared_ptr<std::pair<int, int>> shared_ptr_p : curlyPairs) {
		std::pair<int, int> p = *shared_ptr_p;
		if (p.second == p.first + 1) {
			throw std::invalid_argument(ExceptionMessages::emptyStatementListGiven);
		}

		// Process Procedure Name using string indexes
		std::string beforeOpen = substring(trimmed, previousEnd, p.first - 1); // -1 because we do not want the open curly to be included
		std::string procName = processPreProcedureDetails(beforeOpen);
		if (procName == "") {
			return nullptr;
		}

		// Process Procedure Body using string indexes and function
		std::string body = substring(trimmed, p.first + 1, p.second - 1); // +1 -1 because we do not want the curly to be included
		std::shared_ptr<ProcessedStmtList> stmtList = processStmtList(body);

		out.push_back(std::make_shared<ProcessedProcedure>(procName, stmtList));

		previousEnd = p.second + 1; // + 1 to not include the close curly in the next iteration
	}
	// There are leftover chracters. Which should not be the case since the program was trimmed of whitespaces
	// already.
	if (previousEnd != trimmed.size()) {
		throw std::invalid_argument(ExceptionMessages::invalidProgramDefinition);
	}
	return std::make_shared<ProcessedProgram>(out);
}
