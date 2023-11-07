#ifndef TOKENFACTORY_H
#define TOKENFACTORY_H

#include <memory>

#include "Token.h"

using namespace std;

/**
* Factory class that produces tokens.
*/
class TokenFactory {
private:
	// Generates a Token with for arithmetic operations and parentheses
	static shared_ptr<Token> generateArithmeticOpToken(string_view tokenName);

	// Generates a Identifier Token. REMINDER that this does not validate the argument to ensure that it is a valid name
	static shared_ptr<Token> generateIdentifier(string_view tokenName);

	// Generates an Integer Literal Token. REMINDER that this does not validate the argument to ensure that it is a valid number
	static shared_ptr<Token> generateIntLiteral(string_view number);
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
	/// <exception cref="invalid_argument">Thrown when the supplied tokenName is invalid</exception>
	/// <returns>a shared pointer to the generated token</returns>
	static shared_ptr<Token> generateTokenForSimple(string tokenName, bool forceIdentifier = false);

	// Overloaded method to take in string views instead
	static shared_ptr<Token> generateTokenForSimple(string_view tokenName, bool forceIdentifier = false);

};

#endif
