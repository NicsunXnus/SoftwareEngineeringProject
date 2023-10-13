#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "QueryParser.h"
#include "Errors/SyntaxError.h"
#include "Errors/SemanticError.h"
#include "QueryObjects/ClauseArg.h"
#include "QueryObjects/ClauseObject.h"
#include "QueryObjects/InvalidQueryObject.h"


using namespace std;

QueryParser::QueryParser() {};

vector<shared_ptr<QueryObject>> QueryParser::parsePQL(vector<string_view> tokens) {
	tuple<vector<string_view>, vector<string_view>> declarationQuery = splitDeclarationQuery(tokens);
	vector<shared_ptr<QueryObject>> declarationParser = validateDeclaration(get<0>(declarationQuery));
	vector<shared_ptr<QueryObject>> queryObjects = validateQuery(get<1>(declarationQuery));

	if (static_cast<int>(semanticErrors.size()) > 0) {
		// throw the first semantic error we encountered
		shared_ptr<SemanticErrorException> s = semanticErrors[0];
		std::string se = s->what();
		throw *semanticErrors[0];
	}

	return queryObjects;
}

/*
	* This function splits the string into two groups: the declaration (variable v, assign a etc..) clause and the query (select...) clause
*/
tuple<vector<string_view>, vector<string_view>> QueryParser::splitDeclarationQuery(vector<string_view> words) {
	vector<string_view> declarations;
	vector<string_view> query;
	int indexSemiColon = -1; // start at -1 so all tokens get pushed into query vector in the case of 0 declarations
	for (int i = words.size() - 1; i >= 0; --i) { // get index of the split, first semicolon from the back
		string_view current = words[i];
		if (current == ";") {
			indexSemiColon = i;
			break;
		}
	}
	for (int i = 0; i < words.size(); ++i) {
		string_view current = words[i];
		if (i > indexSemiColon) {
			query.push_back(current);
		} else {
			declarations.push_back(current);
		}
		
	}
	if (query.size() == 0) {
		throw SyntaxErrorException("No Query clause found");
	}
	return make_tuple(declarations, query);

}

/*
* This function splits the declarations into each individual declaration (up till ";")
* delcarations = delclaration*
*/
vector<vector<string_view>> QueryParser::splitDeclarations(vector<string_view> declarations) {
	
	vector<vector<string_view>> result;
	for (int i = 0; i < declarations.size(); i++) {
		vector<string_view> currentDeclaration;
		while (i < declarations.size()) {
			string_view currStr = declarations[i];
			if (currStr == ";") {
				break;
			}
			
			currentDeclaration.push_back(currStr);
			
			i += 1;
		}
		result.push_back(currentDeclaration);
	}
	return result;
}

/*
* This function validates and converts the string tokens to QueryObjects
* declaration : design-entity synonym (',' synonym)* ';'
*/
vector<shared_ptr<QueryObject>> QueryParser::validateDeclaration(vector<string_view> declarations) {
	vector<shared_ptr<QueryObject>> converted;
	vector<vector<string_view>> splittedDeclarations = splitDeclarations(declarations);
	int totalDeclarations = splittedDeclarations.size();

	// Check if declarations is empty. If so, return an empty query object
	int declarationCount = declarations.size();
	if (declarationCount == 0) {
		return converted;
	}

	for (const vector<string_view>declaration : splittedDeclarations) {
		int size = declaration.size();
		if (size < 2) { // declarations must contain at least 2 items; design-entity synonym
			throw SyntaxErrorException("Invalid declaration");
		}
		// first token must be a design entity, then a synonym, exception thrown by createDesignFactory otherwise
		// get the respective factory required to create QueryObjects
		shared_ptr<QueryObjectFactory> designFactory = QueryObjectFactory::createFactory(declaration[0]);
		// rest of declaration must be the synonyms themselves, convert them to Design query objects
		bool wasDeclaration = false;
		for (int i = 1; i < size; i++) {
			string_view currentDeclaration = declaration[i];
			if (wasDeclaration) {
				wasDeclaration = false;
				if (currentDeclaration != ",") {
					throw SyntaxErrorException("Missing comma");
				}
				if (i == size - 1) {
					throw SyntaxErrorException("Extra comma");
				}
				continue; // at a ",", continue parsing
			}
			if (!SynonymObject::isValid(currentDeclaration)) { // check if valid synonym
				throw SyntaxErrorException("Invalid synonym");
			}
			shared_ptr<QueryObject> queryObject = designFactory->create(currentDeclaration);
			if (synonyms.find(currentDeclaration) != synonyms.end()) { // synonym already declared
				storeSemanticError(make_shared<SemanticErrorException>("Repeated synonym declaration"));
			}
			else {
				synonyms[currentDeclaration] = queryObject;
				std::string entityType(declaration[0].begin(), declaration[0].end());
				synonymToEntity[currentDeclaration] = EntityStringToEnum(entityType);
			}
			wasDeclaration = true;
			converted.push_back(queryObject);
		}
	}
	
	
	return converted;

}

vector<shared_ptr<QueryObject>> QueryParser::validateQuery(vector<string_view> query) {
	vector<shared_ptr<QueryObject>> result;

	int currentWordIndex = 0;

	// check 'Select' keyword is present
	if (!hasSelect(query, currentWordIndex)) {
		throw SyntaxErrorException("'Select' keyword not present");
	}

	// check if query has ended
	if (currentWordIndex >= static_cast<int>(query.size())) {
		throw SyntaxErrorException("Result clause not present");
	}

	int selectTupleTokenCount{};
	// check if is tuple
	if (isSelectTuple(query, currentWordIndex, selectTupleTokenCount)) {
		std::vector<shared_ptr<QueryObject>> selectQueryObjects{ createTupleObjects(query, currentWordIndex, selectTupleTokenCount) };

		result.insert(result.end(), selectQueryObjects.begin(), selectQueryObjects.end());
	}
	else if (isSelectElem(query, currentWordIndex, selectTupleTokenCount)) {
		if (selectTupleTokenCount == 1 && !isDeclared(query, currentWordIndex)) {
			storeSemanticError(make_shared<SemanticErrorException>("Synonym not present in select clause, or synonym not declared"));
			result.push_back(make_shared<StmtObject>("Placeholder query object, synonym not declared"));

			++currentWordIndex;
		}
		else if (selectTupleTokenCount == 1) {
			shared_ptr<QueryObject> selectQuery{ synonyms.find(query[currentWordIndex])->second };
			result.push_back(selectQuery);

			++currentWordIndex;
		
		} else { // tuple token count is 3, element is a attrRef
			shared_ptr<QueryObject> attrRefQuery{ createAttrRefObject(query, currentWordIndex) };

			result.push_back(attrRefQuery);
		}
	}
	else {
		throw SyntaxErrorException("Invalid syntax for result clause");
	}

	// return select query if query has ended
	if (query.size() == currentWordIndex) {
		return result;
	}

	// loop to check for such that clauses and pattern clauses
	while (currentWordIndex < static_cast<int>(query.size())) {
		bool isSuchThat{ hasSuchThat(query, currentWordIndex) }; // checks the current clause is a such that clause
		bool isPattern{ hasPattern(query, currentWordIndex) }; // checks the current clause is a pattern clause
		bool isWith{ hasWith(query, currentWordIndex) }; // checks the current clause is a with clause
		
		
		if (isSuchThat) {
			currentWordIndex += 2;

			if (!hasRelationalReference(query, currentWordIndex)) {
				throw SyntaxErrorException("such that clause has invalid syntax");
			}

			// Construct such that query object
			shared_ptr<QueryObject> suchThatClauseObj{ createClauseObj(query, currentWordIndex) };
			result.push_back(suchThatClauseObj);

		} else if (isPattern) {
			currentWordIndex += 1;
			int patternTokenCount {}; // tracks the number of tokens the clause has if it was a pattern clause

			if (!hasPatternClause(query, currentWordIndex, patternTokenCount)) {
				throw SyntaxErrorException("such that clause has invalid syntax");
			}

			// construct pattern query object
			shared_ptr<QueryObject> patternClauseObj{ createPatternObject(query, currentWordIndex, patternTokenCount) };
			result.push_back(patternClauseObj);
		}
		else if (isWith) {
			currentWordIndex += 1;
			int withTokenCount{};
			bool is1stArgAttrRef{ false };

			if (!hasWithClause(query, currentWordIndex, withTokenCount, is1stArgAttrRef)) {
				throw SyntaxErrorException("with clause has invalid syntax");
			}

			// construct comparison query object
			shared_ptr<QueryObject> comparisonClauseObj{ createComparisonObject(query, currentWordIndex, withTokenCount, is1stArgAttrRef) };
			result.push_back(comparisonClauseObj);
		} else {
			throw SyntaxErrorException("Unidentifiable clause in query");
		}
	}
	return result;
}

bool QueryParser::isDeclared(std::vector<string_view>& query, int index) {
	if (index >= static_cast<int>(query.size())) {
		return false;
	}
	string_view synonym = query[index];
	return this->synonyms.find(synonym) != this->synonyms.end();
}

bool QueryParser::hasSelect(std::vector<string_view>& query, int& index) {
	if (index < static_cast<int>(query.size()) && query[index] == "Select"sv) {
		index++;
		return true;
	}
	return false;
}

bool QueryParser::hasPattern(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size()) && query[index] == "pattern"sv;
}

bool QueryParser::hasSuchThat(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size() - 1) && query[index] == "such"sv && query[index+1] == "that"sv;
}

bool QueryParser::hasRelationalReference(std::vector<string_view>& query, int index) {
	// Eg., "Uses" "(" "a" "," "b" ")"
	if (index > static_cast<int>(query.size()) - SUCH_THAT_CLAUSE_TOKEN_COUNT) {
		return false;
	}

	bool hasRelationalKeyword{ this->relationalReferences.find(query[index]) != this->relationalReferences.end()};
	bool hasOpenBracket{ query[index + 1] == "("sv };
	bool hasComma{ query[index + 3] == ","sv };
	bool hasCloseBracket{ query[index + 5] == ")"sv };

	bool hasRR{ hasRelationalKeyword && hasOpenBracket && hasComma && hasCloseBracket };
	return hasRR;
}

shared_ptr<QueryObject> QueryParser::createClauseObj(std::vector<string_view>& query, int& index) {
	string_view relationalReference{ query[index] };

	shared_ptr<QueryObjectFactory> clauseFactory{ QueryObjectFactory::createFactory(relationalReference) };

	// create a vector of args for the clause objects
	std::vector<shared_ptr<ClauseArg>> argVector;

	// create clauseArg object for first argument
	string_view arg1Name{ query[index + 2] };
	shared_ptr<SynonymObject> synonym1;
	if (SynonymObject::isValid(arg1Name)) {
		if (synonyms.find(arg1Name) == synonyms.end()) { // argument is an undeclared synonym
			storeSemanticError(make_shared<SemanticErrorException>("Semantic error: Synonym in clause is undeclared"));
		}
		synonym1 = make_shared<SynonymObject>(arg1Name, synonymToEntity[arg1Name]);
	}
	argVector.push_back(make_shared<ClauseArg>(arg1Name, synonym1));

	// create clauseArg object for second argument
	string_view arg2Name{ query[index + 4] };
	shared_ptr<SynonymObject> synonym2;
	if (SynonymObject::isValid(arg2Name)) {
		if (synonyms.find(arg2Name) == synonyms.end()) { // argument is an undeclared synonym
			storeSemanticError(make_shared<SemanticErrorException>("Semantic error: Synonym in clause is undeclared"));
		}
		synonym2 = make_shared<SynonymObject>(arg2Name, synonymToEntity[arg2Name]);
	}
	argVector.push_back(make_shared<ClauseArg>(arg2Name, synonym2));

	index += SUCH_THAT_CLAUSE_TOKEN_COUNT;
	try {
		return clauseFactory->create(relationalReference, argVector);
	} catch (const SemanticErrorException& ex) {
		storeSemanticError(make_shared<SemanticErrorException>(ex));
		return make_shared<StmtObject>("Clause object has semantic error, query not evaluated");
	}
}

bool QueryParser::hasPatternClause(std::vector<string_view>& query, int index, int& tokenCount) {
	// pattern clause has a variable number of tokens
	// E.g., "a", "(", "_", ",", "_", "x", "_", ")": a(_,_"x"_)has 8
	// "a", "(", "_", ",", "_", ")": a(_,_) has 6

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
	} else { // pattern is looking for partial match
		if (index > (static_cast<int>(query.size()) - MAX_PATTERN_CLAUSE_TOKEN_COUNT)) {
			return false;
		}

		tokenCount = MAX_PATTERN_CLAUSE_TOKEN_COUNT;
		hasCloseBracket = query[index + 7] == ")"sv;
	}
	
	bool hasPC{ isSynonym && hasOpenBracket && hasComma && hasCloseBracket };
	return hasPC;
}

shared_ptr<QueryObject> QueryParser::createPatternObject(std::vector<string_view>& query, int& index, int tokenCount) {
	shared_ptr<QueryObjectFactory> patternFactory{ QueryObjectFactory::createFactory("pattern"sv)};

	std::vector<shared_ptr<ClauseArg>> argVector;

	// create ClauseArg for pattern synonym
	string_view patternSynonymArg{ query[index] };
	if (!SynonymObject::isValid(patternSynonymArg)) {
		throw SyntaxErrorException("Syntax error: Pattern synonym is not syntactically correct");
	} else if (synonyms.find(patternSynonymArg) == synonyms.end()) { // synonym is undeclared
		storeSemanticError(make_shared<SemanticErrorException>("Semantic error: Pattern synonym is undeclared"));
	}
	shared_ptr<SynonymObject> patternSynonymObj{ make_shared<SynonymObject>(patternSynonymArg, synonymToEntity[patternSynonymArg]) };
	shared_ptr<ClauseArg> patternSynonym{ make_shared<ClauseArg>(patternSynonymArg, patternSynonymObj) };
	argVector.push_back(patternSynonym);

	// create ClauseArg for arg 1 of pattern clause
	string_view arg1Name{ query[index + 2] };
	shared_ptr<SynonymObject> synonym1{};
	if (SynonymObject::isValid(arg1Name)) {
		if (synonyms.find(arg1Name) == synonyms.end()) { // argument is an undeclared synonym
			storeSemanticError(make_shared<SemanticErrorException>("Semantic error: Pattern synonym is undeclared"));
		}
		synonym1 = make_shared<SynonymObject>(arg1Name, synonymToEntity[arg1Name]);
	}
	argVector.push_back(make_shared<ClauseArg>(arg1Name, synonym1));

	// create ClauseArg for arg 2 of pattern clause
	if (tokenCount == MIN_PATTERN_CLAUSE_TOKEN_COUNT) { // pattern clause is an exact match
		string_view arg2Name{ query[index + 4] };
		argVector.push_back(make_shared<ClauseArg>(arg2Name, nullptr, false));
	} else if (tokenCount == MAX_PATTERN_CLAUSE_TOKEN_COUNT) { // pattern clause is a partial match
		string_view arg2Name{ query[index + 5] };
		argVector.push_back(make_shared<ClauseArg>(arg2Name, nullptr, true));
	} else {
		std::cout << "Invalid token count in pattern object creation\n";
	}

	index += tokenCount;
	try {
		return patternFactory->create("pattern"sv, argVector);
	} catch (const SemanticErrorException& ex) {
		storeSemanticError(make_shared<SemanticErrorException>(ex));
		return make_shared<StmtObject>("Pattern clause has semantic error, query not evaluated");
	}
}

bool QueryParser::isSelectTuple(std::vector<string_view>& query, int index, int& tokenCount) {
	bool hasOpenBracket{ query[index] == "<"sv };
	tokenCount = 0;

	if (!hasOpenBracket) {
		return false;
	}

	++tokenCount;

	bool isSynonymOrAttrName{ true };
	bool isDotOrComma{ false };

	while (index + tokenCount < static_cast<int>(query.size()) && query[index + tokenCount] != ">"sv) {
		string_view token{ query[index + tokenCount] };
		
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

bool QueryParser::isSelectElem(std::vector<string_view>& query, int index, int& tokenCount) {
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

bool QueryParser::isAttrRef(std::vector<string_view>& query, int index, int& tokenCount) {
	if (query[index + 1] == "."sv) {
		tokenCount = 3;
		return true;
	}
	return false;
}

shared_ptr<QueryObject> QueryParser::createAttrRefObject(std::vector<string_view>& query, int& index) {
	string_view synonymName{ query[index] };
	string_view attrRef{ query[index + 2] };

	shared_ptr<QueryObjectFactory> attrRefFactory{ QueryObjectFactory::createFactory(attrRef) };

	if (!SynonymObject::isValid(synonymName)) {
		throw SyntaxErrorException("Invalid synonym grammar syntax in attrRef");
	}

	if (synonyms.find(synonymName) == synonyms.end()) { // synonym is not declared
		storeSemanticError(make_shared<SemanticErrorException>("Synonym in attrRef is undeclared"));

		index += 3;
		return make_shared<StmtObject>("Placeholder, synonym in attrRef is undeclared");
	}
	
	shared_ptr<SynonymObject> synonym{ make_shared<SynonymObject>(synonymName, synonymToEntity[synonymName]) };
	shared_ptr<ClauseArg> synonymArg{ make_shared<ClauseArg>(synonymName, synonym) };
	vector<shared_ptr<ClauseArg>> argVector{ synonymArg };

	// increment index by 3 to move index to next tokens to be parsed
	index += 3;

	return attrRefFactory->create(attrRef, argVector);
}

shared_ptr<QueryObject> QueryParser::createAttrRefObjectInTuple(string_view synonymName, string_view attrName) {
	shared_ptr<QueryObjectFactory> attrRefFactory{ QueryObjectFactory::createFactory(attrName) };

	if (!SynonymObject::isValid(synonymName)) {
		throw SyntaxErrorException("Invalid synonym grammar syntax in attrRef");
	}

	if (synonyms.find(synonymName) == synonyms.end()) { // synonym is not declared
		storeSemanticError(make_shared<SemanticErrorException>("Synonym in attrRef is undeclared"));
		return make_shared<StmtObject>("Placeholder, synonym in attrRef is undeclared");
	}

	shared_ptr<SynonymObject> synonym{ make_shared<SynonymObject>(synonymName, synonymToEntity[synonymName]) };
	shared_ptr<ClauseArg> synonymArg{ make_shared<ClauseArg>(synonymName, synonym) };
	vector<shared_ptr<ClauseArg>> argVector{ synonymArg };

	return attrRefFactory->create(attrName, argVector);
}

std::vector<shared_ptr<QueryObject>> QueryParser::createTupleObjects(std::vector<string_view>& query, int& index, int tokenCount) {
	// TODO: implement create tuple objects for attr refs. Current iteration only supports tuples of synonyms
	vector<shared_ptr<QueryObject>> resultClauseObjects;

	string_view synonym{};
	string_view attrName{};
	bool isAttrRef{ false };

	for (int i = 1; i < tokenCount; ++i) {
		string_view token{ query[index + i] };
		
		if (token == ","sv || token == ">"sv) {
			if (!isAttrRef) { // element before this token is a synonym
				if (this->synonyms.find(synonym) == this->synonyms.end()) { // synonym undeclared
					storeSemanticError(make_shared<SemanticErrorException>("Undeclared synonym in select tuple"));
					resultClauseObjects.push_back(make_shared<StmtObject>("Placeholder, undeclared synonym in result clause"));
				}
				else {
					resultClauseObjects.push_back(synonyms.find(synonym)->second);
				}
			}
			else { // element before is a attrRef, create a attrRef queryObject
				shared_ptr<QueryObject> attrRefObject{ createAttrRefObjectInTuple(synonym, attrName) };
				resultClauseObjects.push_back(attrRefObject);

				isAttrRef = false;
			}
		}
		else if (token == "."sv) { // token before is a synonym, and now we're within a attrRef element
			isAttrRef = true;
		}
		else if (isAttrRef) { // current token is attrName token
			attrName = token;
		}
		else { // current token is a synonym
			synonym = token;
		}
	}

	index += tokenCount;
	return resultClauseObjects;

}

bool QueryParser::hasWith(std::vector<string_view>& query, int index) {
	return index < static_cast<int>(query.size()) && query[index] == "with"sv;
}

bool QueryParser::hasWithClause(std::vector<string_view>& query, int index, int& tokenCount, bool& is1stRefAttrRef) {
	// with clause has a variable number of tokens. General structure: ref = ref
	// a ref could have 1 or 3 tokens: "1" or "a", ".", "value"

	// check there are at least the minimum number of tokens remaining
	if (index + MIN_WITH_CLAUSE_TOKEN_COUNT > static_cast<int>(query.size())) {
		return false;
	}
	else if (index + MIN_WITH_CLAUSE_TOKEN_COUNT == static_cast<int>(query.size())) { // with clause is a static = static
		return true;
	}

	tokenCount = MIN_WITH_CLAUSE_TOKEN_COUNT;

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

	bool isRef2AttrRef{ query[index + 3] == "."sv };

	if (isRef2AttrRef) {
		if (index + MIN_WITH_CLAUSE_TOKEN_COUNT + 2 > static_cast<int>(query.size())) {
			return false;
		}
		tokenCount += 2;
	}

	return true;
}

shared_ptr<QueryObject> QueryParser::createComparisonObject(std::vector<string_view>& query, 
	int& index, int tokenCount, bool is1stArgAttrRef) {
	if (tokenCount == MIN_WITH_CLAUSE_TOKEN_COUNT) { // Comparison is made between 2 static args
		shared_ptr<QueryObjectFactory> staticStaticFactory{ QueryObjectFactory::createFactory("Static=Static"sv) };
		std::vector<shared_ptr<ClauseArg>> argVector;

		// Create clause args with the two arguments
		string_view arg1{ query[index] };
		string_view arg2{ query[index + 2] };
		argVector.emplace_back(make_shared<ClauseArg>(arg1));
		argVector.emplace_back(make_shared<ClauseArg>(arg2));

		index += tokenCount;
		return staticStaticFactory->create("Static=Static"sv, argVector);
	}
	else if (tokenCount == WITH_CLAUSE_ONE_ATTR_REF_TOKEN_COUNT) { // Comparison is made between 1 static arg and 1 attrRef
		shared_ptr<QueryObjectFactory> staticAttrRefFactory{ QueryObjectFactory::createFactory("Static=AttrRef"sv) };
		std::vector<shared_ptr<ClauseArg>> argVector;

		string_view staticArg, synonymName, attrName;

		if (!is1stArgAttrRef) { // order of token is staticVal = attrRef. e.g., '15', '=', 'a', '.', 'procName'
			staticArg = query[index];
			synonymName = query[index + 2];
			attrName = query[index + 4];
		}
		else { // order of token is attrRef = staticVal. e.g., 'a', '.', 'procName', '=', '15'
			synonymName = query[index];
			attrName = query[index + 2];
			staticArg = query[index + 4];
		}

		// check synonym is valid and synonym has been declared
		if (!SynonymObject::isValid(synonymName)) {
			throw SyntaxErrorException("Invalid synonym grammar syntax in comparison");
		}
		if (synonyms.find(synonymName) == synonyms.end()) { // synonym is not declared
			storeSemanticError(make_shared<SemanticErrorException>("Synonym in comparison is undeclared"));
			return make_shared<StmtObject>("Placeholder, synonym in comparison is undeclared");
		}

		// add synonym clause arg
		shared_ptr<SynonymObject> synonym{ make_shared<SynonymObject>(synonymName, synonymToEntity[synonymName]) };
		shared_ptr<ClauseArg> synonymArg{ make_shared<ClauseArg>(synonymName, synonym) };
		argVector.emplace_back(synonymArg);

		// add synonym attr name
		argVector.emplace_back(make_shared<ClauseArg>(attrName));

		// add static argument compared to
		argVector.emplace_back(make_shared<ClauseArg>(staticArg));

		index += tokenCount;
		return staticAttrRefFactory->create("Static=AttrRef"sv, argVector);
	}
	else if (tokenCount == MAX_WITH_CLAUSE_TOKEN_COUNT) { // Comparison is made between 2 attrRefs
		// e.g., 'a', '.', 'procName', '=', 'b', '.', 'stmt#'

		shared_ptr<QueryObjectFactory> attrRefAttrRefFactory{ QueryObjectFactory::createFactory("AttrRef=AttrRef"sv) };
		std::vector<shared_ptr<ClauseArg>> argVector;

		string_view synonymName1{ query[index] };
		string_view attrName1{ query[index + 2] };
		string_view synonymName2{ query[index + 4] };
		string_view attrName2{ query[index + 6] };

		// check synonym 1 is valid and has been declared
		if (!SynonymObject::isValid(synonymName1)) {
			throw SyntaxErrorException("Invalid synonym 1 grammar syntax in comparison");
		}
		if (synonyms.find(synonymName1) == synonyms.end()) { // synonym is not declared
			storeSemanticError(make_shared<SemanticErrorException>("Synonym 1 in comparison is undeclared"));
			return make_shared<StmtObject>("Placeholder, synonym 1 in comparison is undeclared");
		}

		// add synonym1 clause arg
		shared_ptr<SynonymObject> synonym1{ make_shared<SynonymObject>(synonymName1, synonymToEntity[synonymName1]) };
		shared_ptr<ClauseArg> synonymArg1{ make_shared<ClauseArg>(synonymName1, synonym1) };
		argVector.emplace_back(synonymArg1);

		// add synonym1 attr name
		argVector.emplace_back(make_shared<ClauseArg>(attrName1));

		// check synonym 2 is valid and has been declared
		if (!SynonymObject::isValid(synonymName2)) {
			throw SyntaxErrorException("Invalid synonym 2 grammar syntax in comparison");
		}
		if (synonyms.find(synonymName2) == synonyms.end()) { // synonym is not declared
			storeSemanticError(make_shared<SemanticErrorException>("Synonym 2 in comparison is undeclared"));
			return make_shared<StmtObject>("Placeholder, synonym 2 in comparison is undeclared");
		}

		// add synonym2 clause arg
		shared_ptr<SynonymObject> synonym2{ make_shared<SynonymObject>(synonymName2, synonymToEntity[synonymName2]) };
		shared_ptr<ClauseArg> synonymArg2{ make_shared<ClauseArg>(synonymName2, synonym2) };
		argVector.emplace_back(synonymArg2);

		// add synonym2 attr name
		argVector.emplace_back(make_shared<ClauseArg>(attrName2));

		index += tokenCount;
		return attrRefAttrRefFactory->create("AttrRef=AttrRef"sv, argVector);
	}

	throw SyntaxErrorException("Error creating comparison object, invalid token count passed");
}

void QueryParser::storeSemanticError(shared_ptr<SemanticErrorException> semanticError) {
	semanticErrors.push_back(semanticError);
}
