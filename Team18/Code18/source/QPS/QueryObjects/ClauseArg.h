#ifndef CLAUSEARG_H
#define CLAUSEARG_H

#include <string>
#include <string_view>
#include "SynonymObjects.h"
#include "../../HelperFunctions.h"
#include "../../Constants/DesignEnums.h"


using namespace std::string_view_literals;
/*
* This class represents a Query argument, which represents the query arguments in a clause
*/
class ClauseArg {
private:
	// the argument name
	string_view arg;

	// is a null ptr if the argument is not a synonym
	std::shared_ptr<SynonymObject> synonym;

	// each identifier arg holds an open and close quote marks
	int IDENTIFIER_MIN_CHARS{ 2 };

	// indicates if the clauseArg is a partial pattern match
	bool isPartialMatch;

	bool isNum;

public:
	ClauseArg(string_view arg, std::shared_ptr<SynonymObject> synonym, bool isPartialMatch=false)
		: arg{ arg }, synonym{ synonym }, isPartialMatch{ isPartialMatch } {};

	string_view getArg() {
		return this->arg;
	}

	// function to check if argument is an integer
	bool isInteger() {
		bool isNum{ true };
		for (int i = 0; i < static_cast<int>(arg.size()); ++i) {
			isNum = isNum && isdigit(arg[i]);
		}
		return isNum;
	}

	// function to check if clauseArg is a wildcard
	bool isWildcard() {
		return arg == "_"sv;
	}

	// function to check if clauseArg is a synonym
	bool isSynonym() {
		return synonym.get() != nullptr;
	}

	std::shared_ptr<SynonymObject> getSynonym() {
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
	// returns the identifier without "
	string_view getIdentifier() {
		string_view identifierName = arg.substr(1, arg.size() - 2);
		return identifierName;
	}

	// used for validation of pattern clauses
	bool isExpr() {
		if (static_cast<int>(arg.size()) <= IDENTIFIER_MIN_CHARS) {
			return false;
		}
		string_view expr = arg.substr(1, arg.size() - 2);
		return (arg[0] == '"') && (arg.back() == '"') && (SynonymObject::isValid(expr) || isNumber(std::string(expr)));
	}

	// function to check if clauseArg is a partial matching expression-spec
	bool isPartialMatchingExprSpec() {
		return isExpr() && isPartialMatch;
	}
};


#endif