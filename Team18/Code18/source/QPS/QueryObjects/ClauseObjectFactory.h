#pragma once
#ifndef CLAUSEOBJECTFACTORY_H
#define CLAUSEOBJECTFACTORY_H

#include <string_view>
#include "ClauseObject.h"
#include "QueryObjectFactory.h"


class ClauseObjectFactory : public QueryObjectFactory {
private:

public:
	ClauseObjectFactory() : QueryObjectFactory{} {}

	
};

/*   
* This class represents a Query Object Factory, for clause Uses
*/
class UsesObjectFactory : public ClauseObjectFactory {
public:
	UsesObjectFactory() {};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg1SynonymEntityRef{ arg1->isSynonym() && (validOtherEntities.find(arg1->getSynonym()->getEntityType()) != validOtherEntities.end()) };
		bool isArg2SynonymVariableRef{ arg2->isSynonym() && arg2->getSynonym()->getEntityType() == VARIABLE };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() };
		bool isArg1ValidEntRef{ isArg1SynonymEntityRef || arg1->isIdentifier() };
		bool isArg2ValidEntRef{ isArg2SynonymVariableRef || arg2->isWildcard() || arg2->isIdentifier() };

		if (isArg1ValidStmtRef && isArg2ValidEntRef) { // Uses has a stmt ref as the first argument
			return make_shared<UsesObject>(clauseName, arg1, arg2);
		}
		else if (isArg1ValidEntRef && isArg2ValidEntRef) { // Uses has an entity ref as the first argument
			return make_shared<UsesEntityObject>(clauseName, arg1, arg2);
		}

		// check if first arg is wildcard or if arg1 is a declared entity that is not semantically accepted by Uses
		if (arg1->isWildcard() || (arg1->isSynonym() && !isArg1SynonymEntityRef && !isArg1SynonymStmtRef)) {
			throw SemanticErrorException("Semantic error: invalid 1st argument in clause");
		}
		else if (arg2->isSynonym() && arg2->getSynonym()->getEntityType() != VARIABLE) { // check if arg2 is a declared entity thats not a variable
			throw SemanticErrorException("Semantic error: 2nd argument is not a variable entity");
		}
		else { // should be a syntactic error in the query
			throw SyntaxErrorException("Syntax error: Error in validating the clause");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ PRINT, WHILE, IF, ASSIGN };
	const unordered_set<ENTITY> validOtherEntities{ CALL, PROCEDURE };
};

/*
* This class represents a Query Object Factory, for clause Modifies
*/
class ModifiesObjectFactory : public ClauseObjectFactory {
public:
	ModifiesObjectFactory() {

	}; 

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg1SynonymEntityRef{ arg1->isSynonym() && (validOtherEntities.find(arg1->getSynonym()->getEntityType()) != validOtherEntities.end()) };
		bool isArg2SynonymVariableRef{ arg2->isSynonym() && arg2->getSynonym()->getEntityType() == VARIABLE };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() };
		bool isArg1ValidEntRef{ isArg1SynonymEntityRef || arg1->isIdentifier() };
		bool isArg2ValidEntRef{ isArg2SynonymVariableRef || arg2->isWildcard() || arg2->isIdentifier() };

		if (isArg1ValidStmtRef && isArg2ValidEntRef) { // Uses has a stmt ref as the first argument
			return make_shared<ModifiesObject>(clauseName, arg1, arg2);
		}
		else if (isArg1ValidEntRef && isArg2ValidEntRef) { // Uses has an entity ref as the first argument
			return make_shared<ModifiesEntityObject>(clauseName, arg1, arg2);
		}

		// check if first arg is wildcard or if arg1 is a declared entity that is not semantically accepted by Uses
		if (arg1->isWildcard() || (arg1->isSynonym() && !isArg1SynonymEntityRef && !isArg1SynonymStmtRef)) {
			throw SemanticErrorException("Semantic error: invalid 1st argument in clause");
		}
		else if (arg2->isSynonym() && arg2->getSynonym()->getEntityType() != VARIABLE) { // check if arg2 is a declared entity thats not a variable
			throw SemanticErrorException("Semantic error: 2nd argument is not a variable entity");
		}
		else { // should be a syntactic error in the query
			throw SyntaxErrorException("Syntax error: Error in validating the clause");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ READ, WHILE, IF, ASSIGN };
	const unordered_set<ENTITY> validOtherEntities{ CALL, PROCEDURE };

};

/*
* This class represents a Query Object Factory, for clause Follows: 'Follows' '(' stmtRef ',' stmtRef ')'
*/
class FollowsObjectFactory : public ClauseObjectFactory {
public:
	FollowsObjectFactory() {

	};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg2SynonymStmtRef{ arg2->isSynonym() && (validStmtEntities.find(arg2->getSynonym()->getEntityType()) != validStmtEntities.end()) };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() || arg1->isWildcard()};
		bool isArg2ValidStmtRef{ isArg2SynonymStmtRef || arg2->isInteger() || arg2->isWildcard()};


		if (isArg1ValidStmtRef && isArg2ValidStmtRef) {
			return make_shared<FollowsObject>(clauseName, arg1, arg2);
		}
		else {
			throw SemanticErrorException("Error constructing Follows Object");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN };

};

/*
* This class represents a Query Object Factory, for clause Follows*: 'Follows' '(' stmtRef ',' stmtRef ')'
*/
class FollowsStarObjectFactory : public ClauseObjectFactory {
public:
	FollowsStarObjectFactory() {

	};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg2SynonymStmtRef{ arg2->isSynonym() && (validStmtEntities.find(arg2->getSynonym()->getEntityType()) != validStmtEntities.end()) };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() || arg1->isWildcard() };
		bool isArg2ValidStmtRef{ isArg2SynonymStmtRef || arg2->isInteger() || arg2->isWildcard() };


		if (isArg1ValidStmtRef && isArg2ValidStmtRef) {
			return make_shared<FollowsStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SemanticErrorException("Error constructing Follows* Object");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN };

};

/*
* This class represents a Query Object Factory, for clause Parent: 'Parent' '(' stmtRef ',' stmtRef ')'
*/
class ParentObjectFactory : public ClauseObjectFactory {
public:
	ParentObjectFactory() {

	};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg2SynonymStmtRef{ arg2->isSynonym() && (validStmtEntities.find(arg2->getSynonym()->getEntityType()) != validStmtEntities.end()) };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() || arg1->isWildcard() };
		bool isArg2ValidStmtRef{ isArg2SynonymStmtRef || arg2->isInteger() || arg2->isWildcard() };


		if (isArg1ValidStmtRef && isArg2ValidStmtRef) {
			return make_shared<ParentObject>(clauseName, arg1, arg2);
		}
		else {
			throw SemanticErrorException("Error constructing Parent Object");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN };

};

/*
* This class represents a Query Object Factory, for clause Parent*: 'Parent' '(' stmtRef ',' stmtRef ')'
*/
class ParentStarObjectFactory : public ClauseObjectFactory {
public:
	ParentStarObjectFactory() {

	};

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		bool isArg1SynonymStmtRef{ arg1->isSynonym() && (validStmtEntities.find(arg1->getSynonym()->getEntityType()) != validStmtEntities.end()) };
		bool isArg2SynonymStmtRef{ arg2->isSynonym() && (validStmtEntities.find(arg2->getSynonym()->getEntityType()) != validStmtEntities.end()) };

		bool isArg1ValidStmtRef{ isArg1SynonymStmtRef || arg1->isInteger() || arg1->isWildcard() };
		bool isArg2ValidStmtRef{ isArg2SynonymStmtRef || arg2->isInteger() || arg2->isWildcard() };


		if (isArg1ValidStmtRef && isArg2ValidStmtRef) {
			return make_shared<ParentStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SemanticErrorException("Error constructing Parent* Object");
		}
	}
private:
	const unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, CALL, WHILE, IF, ASSIGN };

};

#endif