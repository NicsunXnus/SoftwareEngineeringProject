#ifndef SIMPLETOKENIZER_H
#define SIMPLETOKENIZER_H

#include <string>
#include <string_view>
#include <vector>

#include "TokenFactory.h"
#include "TokenizerFunctions.h"

using namespace std::string_view_literals;

class SimpleTokenizer {
private:
	// Trims leading and trailing whitespaces.
	// Original code from https://www.positioniseverything.net/cpp-string-trim/
	static std::string TrimWhitespaces(std::string str)
	{
		const char* typeOfWhitespaces = " tnrfv";
		str.erase(str.find_last_not_of(typeOfWhitespaces) + 1);
		str.erase(0, str.find_first_not_of(typeOfWhitespaces));
		return str;
	}

	// Tokenizes a statement
	static std::vector<std::shared_ptr<Token>> tokenize_statement(std::string stmt) {
		std::vector<std::shared_ptr<Token>> output;
		bool validStatement = false;
		if (stmt.find("=") != std::string::npos) { // Assignment Statement
			output = tokenize_assignment(stmt);
			validStatement = true;
		}	
		if (stmt.substr(0, 5) == "read ") {
			output = tokenize_read(stmt);
			validStatement = true;
		}
		if (stmt.substr(0, 6) == "print ") {
			output = tokenize_print(stmt);
			validStatement = true;
		}
		if (!validStatement) {
			throw std::invalid_argument("Statement provided is invalid. Statement supplied: " + stmt);
		}
		output.push_back(TokenFactory::generateToken(";"sv, true));
		return output;
	}

	// Tokenizes an assignment statement
	static std::vector<std::shared_ptr<Token>> tokenize_assignment(std::string ass_stmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> splitMain = TokenizerFunctions::splitString(ass_stmt, "=");
		if (splitMain.size() != 2) {
			// Invalid assignment statement
			throw std::invalid_argument("Assignment Statement expected one equals sign. Got: " + std::to_string(splitMain.size()));
		}
		std::vector<std::shared_ptr<Token>> left = tokenize_expression(splitMain[0]);
		std::vector<std::shared_ptr<Token>> right = tokenize_expression(splitMain[1]);
		output.insert(output.end(), left.begin(), left.end());
		output.push_back(TokenFactory::generateToken("="sv, true));
		output.insert(output.end(), right.begin(), right.end());

		return output;
	}

	// Tokenizes an expression, split by whitespaces. Prioritises identifiers for valid names
	static std::vector<std::shared_ptr<Token>> tokenize_expression(std::string input) {
		input = TrimWhitespaces(input);
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> split = TokenizerFunctions::splitString(input); // using default delimiter of whitespace
		for (std::string s : split) {
			s = TrimWhitespaces(s);
			output.push_back(TokenFactory::generateToken(s, true, true));
		}
		return output;
	}

	// Tokenizes a read statement
	static std::vector<std::shared_ptr<Token>> tokenize_read(std::string read_stmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> split = TokenizerFunctions::splitString(read_stmt);
		if (split.size() != 2) {
			// Invalid read statement
			throw std::invalid_argument("Read Statement expected two words. Got: " + std::to_string(split.size()));
		}
		output.push_back(TokenFactory::generateToken("read"sv, true));
		std::string right = TrimWhitespaces(split[1]);
		output.push_back(TokenFactory::generateToken(right, true, true));
		return output;
	}

	// Tokenizes a print statement
	static std::vector<std::shared_ptr<Token>> tokenize_print(std::string print_stmt) {
		std::vector<std::shared_ptr<Token>> output;
		std::vector<std::string> split = TokenizerFunctions::splitString(print_stmt);
		if (split.size() != 2) {
			// Invalid print statement
			throw std::invalid_argument("Print Statement expected two words. Got: " + std::to_string(split.size()));
		}
		output.push_back(TokenFactory::generateToken("print"sv, true));
		std::string right = TrimWhitespaces(split[1]);
		output.push_back(TokenFactory::generateToken(right, true, true));
		return output;
	}

public:
	/// <summary>
	/// Tokenizes a given SIMPLE source code.
	/// 
	/// CURRENTLY ONLY SUPPORTS STATEMENT LISTS WITHOUT CURLY BRACES.
	/// 
	/// CURRENTLY DOES NOT SUPPORT:
	/// - Procedure
	/// - Call Statements
	/// - If-Then-Else Statements
	/// - While Statements
	/// </summary>
	/// <param name="src">input SIMPLE source code</param>
	/// <returns>a 1D list of shared pointers to Tokens generated</returns>
	static std::vector<std::shared_ptr<Token>> tokenize(std::string_view src) {
		std::vector<std::string> statements = TokenizerFunctions::splitString(std::string(src), ";");
		std::vector<std::shared_ptr<Token>> output;
		for (std::string stmt : statements) {
			stmt = TrimWhitespaces(stmt);
			std::vector<std::shared_ptr<Token>> tokens = tokenize_statement(stmt);
			output.insert(output.end(), tokens.begin(), tokens.end());
		}
		return output;
	}
};
#endif