#ifndef SIMPLETOKENIZER_H
#define SIMPLETOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "TokenFactory.h"
#include "../HelperFunctions.h"

using namespace std::string_view_literals;

class SimpleTokenizer {
private:
	// Tokenizes a statement
	static std::vector<std::shared_ptr<Token>> tokenizeStatement(std::string stmt) {
		std::vector<std::shared_ptr<Token>> output;
		bool validStatement = false;
		if (stmt.find("=") != std::string::npos) { // Assignment Statement
			output = tokenizeAssignment(stmt);
			validStatement = true;
		}	
		if (stmt.substr(0, 5) == "read ") {
			output = tokenizeRead(stmt);
			validStatement = true;
		}
		if (stmt.substr(0, 6) == "print ") {
			output = tokenizePrint(stmt);
			validStatement = true;
		}
		if (!validStatement) {
			throw std::invalid_argument("Statement provided is invalid. Statement supplied: " + stmt);
		}
		output.push_back(TokenFactory::generateToken(";"sv, true));
		return output;
	}

	// Tokenizes an assignment statement
	static std::vector<std::shared_ptr<Token>> tokenizeAssignment(std::string assStmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> splitMain = splitString(assStmt, "=");
		if (splitMain.size() != 2) {
			// Invalid assignment statement
			throw std::invalid_argument("Assignment Statement expected one equals sign. Got: " + std::to_string(splitMain.size()));
		}
		std::vector<std::shared_ptr<Token>> left = tokenizeExpression(splitMain[0]);
		std::vector<std::shared_ptr<Token>> right = tokenizeExpression(splitMain[1]);
		output.insert(output.end(), left.begin(), left.end());
		output.push_back(TokenFactory::generateToken("="sv, true));
		output.insert(output.end(), right.begin(), right.end());

		return output;
	}

	// Tokenizes an expression, split by whitespaces. Prioritises identifiers for valid names
	static std::vector<std::shared_ptr<Token>> tokenizeExpression(std::string input) {
		input = trimWhitespaces(input);
		std::vector<std::shared_ptr<Token>> output;
		// The regex matches all separators and operations that can be found in expressions: 
		// ( ) + - * / % and whitespace
		std::vector<std::string> split = splitString(input, "([()+\\-/*%\\s])", true);
		for (std::string s : split) {
			if (s == " ") continue;
			s = trimWhitespaces(s);
			output.push_back(TokenFactory::generateToken(s, true, true));
		}
		return output;
	}

	// Tokenizes a read statement
	static std::vector<std::shared_ptr<Token>> tokenizeRead(std::string readStmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> split = splitString(readStmt);
		if (split.size() != 2) {
			// Invalid read statement
			throw std::invalid_argument("Read Statement expected two words. Got: " + std::to_string(split.size()));
		}
		output.push_back(TokenFactory::generateToken("read"sv, true));
		std::string right = trimWhitespaces(split[1]);
		output.push_back(TokenFactory::generateToken(right, true, true));
		return output;
	}

	// Tokenizes a print statement
	static std::vector<std::shared_ptr<Token>> tokenizePrint(std::string printStmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> split = splitString(printStmt);
		if (split.size() != 2) {
			// Invalid print statement
			throw std::invalid_argument("Print Statement expected two words. Got: " + std::to_string(split.size()));
		}
		output.push_back(TokenFactory::generateToken("print"sv, true));
		std::string right = trimWhitespaces(split[1]);
		output.push_back(TokenFactory::generateToken(right, true, true));
		return output;
	}

public:
	/// <summary>
	/// Tokenizes a given SIMPLE source code.
	/// 
	/// CURRENTLY ONLY SUPPORTS STATEMENTS WITHOUT CURLY BRACES.
	/// 
	/// CURRENTLY DOES NOT SUPPORT:
	/// - Procedure
	/// - Call Statements
	/// - If-Then-Else Statements
	/// - While Statements
	/// 
	/// Throws std::invalid_argument exception if anything supplied in the stmt list is invalid.
	/// This can arise due to invalid names for identifiers, or invalid formatting like two "=" in an assign statement
	/// 
	/// </summary>
	/// <param name="src">input SIMPLE source code</param>
	/// <returns>a 1D list of shared pointers to Tokens generated</returns>
	static std::vector<std::vector<std::shared_ptr<Token>>> tokenize(std::string_view src) {
		std::vector<std::string> statements = splitString(std::string(src), ";");
		std::vector<std::vector<std::shared_ptr<Token>>> output;
		for (std::string stmt : statements) {
			stmt = trimWhitespaces(stmt);
			std::vector<std::shared_ptr<Token>> tokens = tokenizeStatement(stmt);
			output.push_back(tokens);
		}
		return output;
	}
};
#endif
