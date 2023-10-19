#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H

#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_set>

#include "../SimpleTokens/IdentifierToken.h"
#include "../SimpleTokens/KeywordToken.h"
#include "../SimpleTokens/LiteralToken.h"
#include "../SimpleTokens/OperatorToken.h"
#include "../SimpleTokens/SeparatorToken.h"
#include "../../HelperFunctions.h"
#include "../../ExceptionMessages.h"

using namespace std::string_view_literals;

/**
* Factory class that produces tokens.
*/
class TokenFactory {
private:
	// Generates a Token with a name that is common between SIMPLE and PQL
	static std::shared_ptr<Token> generateReservedKWToken(std::string_view tokenName);

	// Generates a Identifier Token. REMINDER that this does not validate the argument to ensure that it is a valid name
	static std::shared_ptr<Token> generateIdentifier(std::string_view tokenName);

	// Generates an Integer Literal Token. REMINDER that this does not validate the argument to ensure that it is a valid number
	static std::shared_ptr<Token> generateIntLiteral(std::string_view number);
public:
	/// <summary>
	/// Generates SIMPLE tokens based on a token name and if the
	/// token should be forced to be an identifier or not.
	/// 
	/// Outputs to std err if anything supplied in the token is invalid.
	/// This can arise due to invalid names for identifiers, or other ways.
	/// </summary>
	/// 
	/// <param name="tokenName">The string to be passed to be turned into a token</param>
	/// <param name="forceIdentifier">Whether to force the token to be an IdentifierToken</param>
	/// <exception cref="std::invalid_argument">Thrown when the supplied tokenName is invalid</exception>
	/// <returns>a shared pointer to the generated token</returns>
	static std::shared_ptr<Token> generateTokenForSimple(std::string tokenName, bool forceIdentifier = false);

	// Overloaded method to take in string views instead
	static std::shared_ptr<Token> generateTokenForSimple(std::string_view tokenName, bool forceIdentifier = false);

};

#endif
