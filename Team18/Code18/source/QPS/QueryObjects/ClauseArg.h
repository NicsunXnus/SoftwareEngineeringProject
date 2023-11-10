#ifndef CLAUSEARG_H
#define CLAUSEARG_H

#include <string>
#include <string_view>
#include "SynonymObjects.h"
#include "../../HelperFunctions.h"
#include "../../Constants/DesignEnums.h"



using namespace string_view_literals;
/*
* This class represents a Query argument, which represents the query arguments in a clause
*/
class ClauseArg {
private:
	// the argument name
	string_view arg;

	// is a null ptr if the argument is not a synonym
	shared_ptr<SynonymObject> synonym;

	// each identifier arg holds an open and close quote marks
	int IDENTIFIER_MIN_CHARS{ 2 };

	// indicates if the clauseArg is a partial pattern match
	bool isPartialMatch;

public:
	ClauseArg(string_view arg, shared_ptr<SynonymObject> synonym=nullptr, bool isPartialMatch=false)
		: arg{ arg }, synonym{ synonym }, isPartialMatch{ isPartialMatch } {};

	string_view getArg() {
		return this->arg;
	}

	string_view getArgValue() {
		if (isIdentifier() || isExpr()) {
			return getQuotesContent();
		}
		return this->arg;
	}

	// function to check if argument is an integer
	bool isInteger() {
		for (int i = 0; i < static_cast<int>(arg.size()); ++i) {
			if (i == 0 && arg[i] == '0' && static_cast<int>(arg.size()) != 1) { // number has a leading 0
				return false;
			}
			if (!isdigit(arg[i])) { // number has a non digit
				return false;
			}
		}
		return true;
	}

	// function to check if clauseArg is a wildcard
	bool isWildcard() {
		return arg == "_"sv;
	}

	// function to check if clauseArg is a synonym
	bool isSynonym() {
		return synonym.get() != nullptr;
	}

	shared_ptr<SynonymObject> getSynonym() {
		return this->synonym;
	}

	// function to check if clauseArg is an identifier
	bool isIdentifier() {
		if (static_cast<int>(arg.size()) <= IDENTIFIER_MIN_CHARS) {
			return false;
		}
		string_view identifierName = arg.substr(1, arg.size() - 2);
		return (arg[0] == '"') && (arg.back() == '"') && (SynonymObject::isValid(identifierName));
	}

	// returns the argument without "
	string_view getQuotesContent() {
		string_view identifierName = arg.substr(1, arg.size() - 2);
		return identifierName;
	}

	// checks for open close quotes in argument
	bool isExpr() {
		if (static_cast<int>(arg.size()) <= IDENTIFIER_MIN_CHARS) {
			return false;
		}
		string_view expr = arg.substr(1, arg.size() - 2);
		return (arg[0] == '"') && (arg.back() == '"');
	}

	// function to check if clauseArg is a partial matching expression-spec
	bool isPartialMatchingExprSpec() {
		return isExpr() && isPartialMatch;
	}
};


#endif