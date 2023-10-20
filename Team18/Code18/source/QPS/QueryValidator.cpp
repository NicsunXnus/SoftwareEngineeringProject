#include "QueryValidator.h"
#include "QueryObjects/SynonymObjects.h"

// Checks if the query's result clause is a tuple
bool QueryValidator::isSelectTuple(std::vector<std::string_view>& query, int index, int& tokenCount) {
	bool hasOpenBracket{ query[index] == "<"sv };
	tokenCount = 0;

	if (!hasOpenBracket) {
		return false;
	}

	++tokenCount;

	bool isSynonymOrAttrName{ true };
	bool isDotOrComma{ false };

	while (index + tokenCount < static_cast<int>(query.size()) && query[index + tokenCount] != ">"sv) {
		std::string_view token{ query[index + tokenCount] };

		if (isSynonymOrAttrName) {
			++tokenCount;
			isDotOrComma = true;
			isSynonymOrAttrName = false;
			continue;
		}
		else if (isDotOrComma && (token == "."sv || token == ","sv)) {
			++tokenCount;
			isSynonymOrAttrName = true;
			isDotOrComma = false;
			continue;
		}
		else {
			// invalid tuple structure
			return false;
		}
	}

	if (isSynonymOrAttrName) { // expecting a word as prev token was , or . However we ended the loop. Thus invalid structure
		return false;
	}

	if (index + tokenCount == static_cast<int>(query.size())) { // broke out of loop due to reaching query end, invalid tuple structure
		return false;
	}
	else if (tokenCount == 1) { // select tuple is empty. E.g Select <>
		return false;
	}

	// include ">" in tokenCount
	++tokenCount;

	return true;
}

// Checks if the query's result clause is a select element
bool QueryValidator::isSelectElem(std::vector<std::string_view>& query, int index, int& tokenCount) {
	// check if query has only 1 token left
	if (static_cast<int>(query.size()) < index + ATTR_REF_TOKEN_COUNT) { // query not long enough to contain a attrRef, must be a synonym
		tokenCount = 1;
		return true;
	}

	if (isAttrRef(query, index, tokenCount)) { // elem is a attrRef
		return true;
	}

	// elem is a synonym
	tokenCount = 1;
	return true;
}

// Checks if a sequence of tokens is an attribute reference at the specified index
bool QueryValidator::isAttrRef(std::vector<std::string_view>& query, int index, int& tokenCount) {
	if (query[index + 1] == "."sv) {
		tokenCount = 3;
		return true;
	}
	return false;
}

// Checks if the query has a relational reference starting at the specified index
bool QueryValidator::hasRelationalReference(std::vector<std::string_view>& query, int index) {
	// Eg., "Uses" "(" "a" "," "b" ")"
	if (index > static_cast<int>(query.size()) - SUCH_THAT_CLAUSE_TOKEN_COUNT) {
		return false;
	}

	bool hasRelationalKeyword{ this->relationalReferences.find(query[index]) != this->relationalReferences.end() };
	bool hasOpenBracket{ query[index + 1] == "("sv };
	bool hasComma{ query[index + 3] == ","sv };
	bool hasCloseBracket{ query[index + 5] == ")"sv };

	bool hasRR{ hasRelationalKeyword && hasOpenBracket && hasComma && hasCloseBracket };
	return hasRR;
}

// Checks if the query has a pattern clause starting at the specified index
bool QueryValidator::hasPatternClause(std::vector<string_view>& query, int index, int& tokenCount, bool& isIfPattern) {
	// pattern clause has a variable number of tokens
	// E.g., "a", "(", "_", ",", "_", "x", "_", ")": a(_,_"x"_)has 8
	// "a", "(", "_", ",", "_", ")": a(_,_) has 6
	// "a", "(", "_", ",", "_", ",", "_", ")": a(_,_,_) has 8

	if (index > (static_cast<int>(query.size()) - MIN_PATTERN_CLAUSE_TOKEN_COUNT)) {
		return false;
	}

	bool isSynonym{ SynonymObject::isValid(query[index]) };
	bool hasOpenBracket{ query[index + 1] == "("sv };
	bool hasComma{ query[index + 3] == ","sv };
	bool hasCloseBracket{ false };
	if (query[index + 5] == ")"sv) { // pattern is looking for an exact match
		tokenCount = MIN_PATTERN_CLAUSE_TOKEN_COUNT;
		hasCloseBracket = true;
		return true;
	}

	if (index > (static_cast<int>(query.size()) - MAX_PATTERN_CLAUSE_TOKEN_COUNT)) {
		return false;
	}

	// pattern might be an if pattern or partial match
	// check if tokens remaining: _ , _ )
	if (query[index + 4] == "_"sv && query[index + 5] == ","sv && query[index + 6] == "_"sv && query[index + 7] == ")"sv) {
		// is if pattern
		isIfPattern = true;
		tokenCount = MAX_PATTERN_CLAUSE_TOKEN_COUNT;
		return true;
	}

	// check if tokens remaining: _ "blabla" _ )
	if (query[index + 4] == "_"sv && query[index + 6] == "_"sv && query[index + 7] == ")"sv) {
		// pattern is partial match
		tokenCount = MAX_PATTERN_CLAUSE_TOKEN_COUNT;
		return true;
	}

	// not a pattern clause
	return false;
}

// Checks if the query has a with clause starting at the specified index
bool QueryValidator::hasWithClause(std::vector<std::string_view>& query, int index, int& tokenCount, bool& is1stRefAttrRef) {
	// with clause has a variable number of tokens. General structure: ref = ref
	// a ref could have 1 or 3 tokens: "1" or "a", ".", "value"

	tokenCount = MIN_WITH_CLAUSE_TOKEN_COUNT;


	// check there are at least the minimum number of tokens remaining
	if (index + MIN_WITH_CLAUSE_TOKEN_COUNT > static_cast<int>(query.size())) {
		return false;
	}
	else if (index + MIN_WITH_CLAUSE_TOKEN_COUNT == static_cast<int>(query.size())) { // with clause is a static = static
		return true;
	}

	// check if first ref is attrRef
	bool isRef1AttrRef{ query[index + 1] == "."sv };

	if (isRef1AttrRef) {
		if (index + MIN_WITH_CLAUSE_TOKEN_COUNT + 2 > static_cast<int>(query.size())) {
			return false;
		}
		is1stRefAttrRef = true;
		tokenCount += 2;
		index += 2;
	}

	bool hasEqualSign{ query[index + 1] == "="sv };
	if (!hasEqualSign) {
		return false;
	}

	// check if there are at least the minimum number of tokens remaining for the second ref to be a static value
	if (index + 3 > static_cast<int>(query.size())) {
		return false;
	}

	// check if there are at least the minimum number of tokens remaining for the second ref to be a attrRef
	if (index + 5 > static_cast<int>(query.size())) {
		// has to be a static value
		return true;
	}

	// could be static or attrRef at this point

	bool isRef2AttrRef{ query[index + 3] == "."sv };

	if (!isRef2AttrRef) {// second ref is a static value
		return true;
	}

	tokenCount += 2;
	return true;
}