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
		
		}else {
			// TODO: Create a attrRef query object
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

	if (query[index + 1] == "."sv) { // elem is a attrRef
		tokenCount = 3;
	}

	// elem is a synonym
	tokenCount = 1;
	return true;
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
				// TODO
				isAttrRef = false;
			}
		}
		else if (token == "."sv) {
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

void QueryParser::storeSemanticError(shared_ptr<SemanticErrorException> semanticError) {
	semanticErrors.push_back(semanticError);
}
