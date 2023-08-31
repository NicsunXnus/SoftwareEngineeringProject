#include "SIMPLETokenizer.h"
#include "TokenizerFunctions.h"

static Token generateToken(std::string input) {
}

/**
* Splits a SIMPLE query into tokens.
* 
* Returns a std::vector of tokens.
*/
static std::vector<Token> tokenize(std::string_view simple) {
	//return std::vector<Token>();
	std::vector<std::string> split = TokenizerFunctions::splitString(std::string(simple));
	std::vector<Token> output;
	for (std::string s : split) {
		//if (s[0] == char("!")) {
		//	output.push_back(generateToken("!"));
		//	s = s.substr(1)
		//}

		// ASSUME THAT THERE IS A SPACE BETWEEN EVERY TOKENISABLE THING
		//  TODO: REMOVE THIS ASSUMPTION
		output.push_back(generateToken(s));
	}
	return output;
}

