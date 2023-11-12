#pragma once
#ifndef CLAUSEOBJECTFACTORY_H
#define CLAUSEOBJECTFACTORY_H

#include <string_view>
#include "ClauseObject.h"
#include "QueryObjectFactory.h"

class ClauseObjectFactory : public QueryObjectFactory {
protected:
	const unordered_set<ENTITY> validStmtEntities{ STMT, READ, PRINT, ASSIGN, CALL, WHILE, IF };

public:
	ClauseObjectFactory() : QueryObjectFactory{} {}

	
};

/*   
* This class represents a Query Object Factory, for clause Uses
*/
class UsesObjectFactory : public ClauseObjectFactory {
public:
	UsesObjectFactory() {};

	// Check if the arguments are valid for a UsesStmt object
	bool validateUsesStmtArgs(vector<shared_ptr<ClauseArg>> arguments);

	// Check if the arguments are valid for a UsesP object
	bool validateUsesPArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		bool isUsesS{ validateUsesStmtArgs(arguments) };
		bool isUsesP{ validateUsesPArgs(arguments) };

		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (isUsesS) {
			return make_shared<UsesObject>(clauseName, arg1, arg2);
		}
		else if (isUsesP) {
			return make_shared<UsesEntityObject>(clauseName, arg1, arg2);
		}
		else if (!isUsesS || !isUsesP) {
			throw SemanticErrorException("Argument entities are invalid for Uses");
		}
		
	}
private:
	const unordered_set<ENTITY> validEntRefsArg1{ PROCEDURE };
	const unordered_set<ENTITY> validEntRefsArg2{ VARIABLE };
};

/*
* This class represents a Query Object Factory, for clause Modifies
*/
class ModifiesObjectFactory : public ClauseObjectFactory {
public:
	ModifiesObjectFactory() {};

	// Check if the arguments are valid for a UsesStmt object
	bool validateModifiesStmtArgs(vector<shared_ptr<ClauseArg>> arguments);

	// Check if the arguments are valid for a UsesP object
	bool validateModifiesPArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		bool isModifiesS{ validateModifiesStmtArgs(arguments) };
		bool isModifiesP{ validateModifiesPArgs(arguments) };

		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (isModifiesS) {
			return make_shared<ModifiesObject>(clauseName, arg1, arg2);
		}
		else if (isModifiesP) {
			return make_shared<ModifiesEntityObject>(clauseName, arg1, arg2);
		}
		else if (!isModifiesS || !isModifiesP) {
			throw SemanticErrorException("Argument entities are invalid for Modifies");
		}
	}
private:
	const unordered_set<ENTITY> validEntRefsArg1{ PROCEDURE };
	const unordered_set<ENTITY> validEntRefsArg2{ VARIABLE };
};

/*
* This class represents a Query Object Factory, for clause Follows: 'Follows' '(' stmtRef ',' stmtRef ')'
*/
class FollowsObjectFactory : public ClauseObjectFactory {
public:
	FollowsObjectFactory() {};

	bool validateFollowsArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateFollowsArgs(arguments)) {
			return make_shared<FollowsObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Follows clause");
		}
	}

};

/*
* This class represents a Query Object Factory, for clause Follows*: 'Follows' '(' stmtRef ',' stmtRef ')'
*/
class FollowsStarObjectFactory : public ClauseObjectFactory {
public:
	FollowsStarObjectFactory() {};

	bool validateFollowsStarArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateFollowsStarArgs(arguments)) {
			return make_shared<FollowsStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Follows* clause");
		}
	}

};

/*
* This class represents a Query Object Factory, for clause Parent: 'Parent' '(' stmtRef ',' stmtRef ')'
*/
class ParentObjectFactory : public ClauseObjectFactory {
public:
	ParentObjectFactory() {};

	bool validateParentArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateParentArgs(arguments)) {
			return make_shared<ParentObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Parent clause");
		}
	}

};

/*
* This class represents a Query Object Factory, for clause Parent*: 'Parent' '(' stmtRef ',' stmtRef ')'
*/
class ParentStarObjectFactory : public ClauseObjectFactory {
public:
	ParentStarObjectFactory() {};

	bool validateParentStarArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateParentStarArgs(arguments)) {
			return make_shared<ParentStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Parent* clause");
		}
	}

};

/*
* This class represents a Query Object Factory, for clause Calls: 'Calls' '(' entRef ',' entRef ')'
*/
class CallsObjectFactory : public ClauseObjectFactory {
public:
	CallsObjectFactory() {};

	bool validateCallsArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateCallsArgs(arguments)) {
			return make_shared<CallsObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Calls clause");
		}
	}
private:
	const unordered_set<ENTITY> validEntRefsArgs{ PROCEDURE };
};

/*
* This class represents a Query Object Factory, for clause Calls*: 'Calls*' '(' entRef ',' entRef ')'
*/
class CallsStarObjectFactory : public ClauseObjectFactory {
public:
	CallsStarObjectFactory() {};

	bool validateCallsStarArgs(vector<shared_ptr<ClauseArg>> arguments);


	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateCallsStarArgs(arguments)) {
			return make_shared<CallsStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Calls* clause");
		}
	}
private:
	const unordered_set<ENTITY> validEntRefsArgs{ PROCEDURE };
};

/*
* This class represents a Query Object Factory, for clause Next: 'Next' '(' stmtRef ',' stmtRef ')'
*/
class NextObjectFactory : public ClauseObjectFactory {
public:
	NextObjectFactory() {};

	bool validateNextArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateNextArgs(arguments)) {
			return make_shared<NextObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Next clause");
		}
	}



};

/*
* This class represents a Query Object Factory, for clause Next*: 'Next*' '(' stmtRef ',' stmtRef ')'
*/
class NextStarObjectFactory : public ClauseObjectFactory {
public:
	NextStarObjectFactory() {};

	bool validateNextStarArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateNextStarArgs(arguments)) {
			return make_shared<NextStarObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Next* clause");
		}
	}

};

/*
* This class represents a Query Object Factory, for clause Affects: 'Affects' '(' stmtRef ',' stmtRef ')'
*/
class AffectsObjectFactory : public ClauseObjectFactory {
public:
	AffectsObjectFactory() {};

	bool validateAffectsArgs(vector<shared_ptr<ClauseArg>> arguments);

	shared_ptr<QueryObject> create(string_view clauseName, vector<std::shared_ptr<ClauseArg>> arguments) override {
		shared_ptr<ClauseArg> arg1 = arguments[0];
		shared_ptr<ClauseArg> arg2 = arguments[1];
		if (validateAffectsArgs(arguments)) {
			return make_shared<AffectsObject>(clauseName, arg1, arg2);
		}
		else {
			throw SyntaxErrorException("Error encountered validating Affects clause");
		}
	}
};

#endif