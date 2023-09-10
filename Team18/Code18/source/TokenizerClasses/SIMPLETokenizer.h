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
		if (stmt.empty()) {
			throw std::invalid_argument("Statement provided is empty.");
		}
		std::vector<std::shared_ptr<Token>> output;
		bool validStatement = false;
		if (stmt.find("=") != std::string::npos) { // Assignment Statement
			output = tokenizeAssignment(stmt);
			validStatement = true;
		}	
		// if the substring "read " exists and it is the first word occurence in the statement
		else if (stmt.find_first_not_of(whitespaces) == stmt.find("read ")) {
			output = tokenizeRead(stmt);
			validStatement = true;
		}
		// if the substring "print " exists and it is the first word occurence in the statement
		else if (stmt.find_first_not_of(whitespaces) == stmt.find("print ")) {
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
		std::vector<std::string> splitByEquals = splitString(assStmt, "=");
		if (splitByEquals.size() != 2) {
			// Invalid assignment statement
			throw std::invalid_argument("Assignment Statement expected one equals sign. Got: " + std::to_string(splitByEquals.size() - 1));
		}
		std::string trimmedLeft = trimWhitespaces(splitByEquals[0]);
		if (!isValidName(trimmedLeft)) {
			throw std::invalid_argument("Identifier provided in assignment statement is invalid");
		}
		std::shared_ptr<Token> left = TokenFactory::generateToken(trimmedLeft, true, true);
		std::vector<std::shared_ptr<Token>> right = tokenizeExpression(splitByEquals[1]);
		output.push_back(left);
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
			// ignore whitespaces. equality check is basically saying: not cannot find so == can find
			// whitespaces are declared in HelperFunctions.h
			if (whitespaces.find(s) != std::string::npos) continue;
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
		std::string orig = std::string(src);
		std::string trimmed = trimWhitespaces(orig);
		// TODO: NEED TO REDO THIS WHEN INCORPORATING PROCEDURES AND IF WHILES
		std::cout << trimmed << std::endl;
		if (!trimmed.empty() && trimmed.back() != ';') {
			for (char c : whitespaces) {
				std::cout << int(c) << std::endl;
			}
			std::cout << "left" << unsigned(uint8_t(trimmed.back())) << "right" << std::endl;
			throw std::invalid_argument("Last statement provided is invalid");
		}
		std::cout << "2" << std::endl;
		std::vector<std::string> statements = splitString(trimmed, ";", false);
		std::cout << "3" << std::endl;
		if (statements.size() == 0 && !trimmed.empty()) {
			throw std::invalid_argument("Empty statements are provided");
		}
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
