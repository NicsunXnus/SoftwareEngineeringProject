#ifndef QUERYVALIDATOR_H
#define QUERYVALIDATOR_H

#include <vector>
#include <string>
#include <string_view>
#include <unordered_set>

using namespace std;

/**
* This class encapsulates an object that validates the structure of a query is valid for parsing
*/
class QueryValidator {
private:
	// Valid relational references
	unordered_set<string_view> relationalReferences
	{ "Follows"sv, "Follows*"sv, "Parent"sv, "Parent*"sv, "Uses"sv, "Modifies"sv, "Calls"sv, "Calls*"sv, "Next"sv, "Next*"sv, "Affects"sv };

	int SUCH_THAT_CLAUSE_TOKEN_COUNT{ 6 };
	int MIN_PATTERN_CLAUSE_TOKEN_COUNT{ 6 };
	int MAX_PATTERN_CLAUSE_TOKEN_COUNT{ 8 };
	int ATTR_REF_TOKEN_COUNT{ 3 }; // e.g., 'p', '.', 'procName'
	int MIN_WITH_CLAUSE_TOKEN_COUNT{ 3 }; // e.g., '"ident"', '=', '15'

public:
	// Checks if the query's result clause is a tuple
	bool isSelectTuple(vector<string_view>& query, int index, int& tokenCount);

	// Checks if the query's result clause is a select element
	bool isSelectElem(vector<string_view>& query, int index, int& tokenCount);

	// Checks if a sequence of tokens is an attribute reference at the specified index
	bool isAttrRef(vector<string_view>& query, int index, int& tokenCount);

	// Checks if the query has a relational reference starting at the specified index
	bool hasRelationalReference(vector<string_view>& query, int index);

	// Checks if the query has a pattern clause starting at the specified index
	bool hasPatternClause(vector<string_view>& query, int index, int& tokenCount, bool& isIfPattern);

	// Checks if the query has a with clause starting at the specified index
	bool hasWithClause(vector<string_view>& query, int index, int& tokenCount, bool& is1stRefAttrRef);
};

#endif