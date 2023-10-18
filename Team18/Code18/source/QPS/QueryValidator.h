#ifndef QUERYVALIDATOR_H
#define QUERYVALIDATOR_H

#include <vector>
#include <string>
#include <string_view>

/**
* This class encapsulates an object that validates the structure of a query is valid for parsing
*/
class QueryValidator {
public:
	// Checks if the query's result clause is a tuple
	bool isSelectTuple(std::vector<string_view>& query, int index, int& tokenCount);

	// Checks if the query's result clause is a select element
	bool isSelectElem(std::vector<string_view>& query, int index, int& tokenCount);

	// Checks if a sequence of tokens is an attribute reference at the specified index
	bool isAttrRef(std::vector<string_view>& query, int index, int& tokenCount);

	// Checks if the query has a relational reference starting at the specified index
	bool hasRelationalReference(std::vector<string_view>& query, int index);

	// Checks if the query has a pattern clause starting at the specified index
	bool hasPatternClause(std::vector<string_view>& query, int index, int& tokenCount);

	// Checks if the query has a with clause starting at the specified index
	bool hasWithClause(std::vector<string_view>& query, int index, int& tokenCount, bool& is1stRefAttrRef);
};

#endif