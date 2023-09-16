#ifndef CLAUSEARG_H
#define CLAUSEARG_H

#include <string>
#include <string_view>
#include "SynonymObjects.h"
#include "../../Constants/QPSPKB.h"


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

public:
	ClauseArg(string_view arg, std::shared_ptr<SynonymObject> synonym) : arg{ arg }, synonym{ synonym } {};

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
		string_view identifierName = arg.substr(1, arg.size() - 2);
		return (arg[0] == '"') && (arg.back() == '"') && (SynonymObject::isValid(identifierName));
	}

	// function to check if clauseArg is an entity
	bool isEntityRef() {

	}
};


#endif