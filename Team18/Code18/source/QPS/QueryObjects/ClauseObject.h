#pragma once
#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a Query object, for such that clause entities
*/
class ClauseObject : public QueryObject {
private:
	shared_ptr<ClauseArg> argument1;
	shared_ptr<ClauseArg> argument2;

public:
	ClauseObject(string_view clauseName, shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2)
		: QueryObject{ clauseName }, argument1{ argument1 }, argument2{ argument2 } {
	}


	shared_ptr<ClauseArg> getArg1() {
		return argument1;
	}

	shared_ptr<ClauseArg> getArg2() {
		return argument2;
	}

	shared_ptr<unordered_set<string>> getSynonyms() {
		unordered_set<string> synonyms;

		if (argument1->isSynonym()) {
			string s1(argument1->getArg());
			synonyms.insert(s1);
		}
		if (argument2->isSynonym()) {
			string s2(argument2->getArg());
			synonyms.insert(s2);
		}

		return make_shared<unordered_set<string>>(synonyms);
	}
protected:
	bool isValidSynonymType(unordered_set<ENTITY> validEntitiesArg1, unordered_set<ENTITY> validEntitiesArg2) {
		if (argument1->isSynonym() && validEntitiesArg1.find(argument1->getSynonym()->getEntityType()) == validEntitiesArg1.end()) {
			return false;
		}
		if (argument2->isSynonym() && validEntitiesArg2.find(argument2->getSynonym()->getEntityType()) == validEntitiesArg2.end()) {
			return false;
		}
		return true;
	}

	shared_ptr<QueryResultsTable> getEmptyTable() {
		vector<string> headers;
		if (argument1->isSynonym()) {
			headers.push_back(svToString(argument1->getArgValue()));
		}
		if (argument2->isSynonym()) {
			headers.push_back(svToString(argument2->getArgValue()));
		}
		return QueryResultsTable::createEmptyTableWithHeaders(headers);
	}

	shared_ptr<QueryResultsTable> handleCallsCallsStar(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleUses(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleModifies(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleFollowsParents(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleNext(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleNextStar(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
	shared_ptr<QueryResultsTable> handleAffects(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause);
};

/*
* This class represents a Query object, for clause Uses with a statement ref as the first argument
*/
class UsesObject : public ClauseObject {
public:
	UsesObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

	
};

/*
* This class represents a Query object, for clause Uses with an entity ref as the first argument
*/
class UsesEntityObject : public ClauseObject {
public:
	UsesEntityObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};

	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause Modifies with a statement ref as the first argument
*/
class ModifiesObject : public ClauseObject {
public:
	ModifiesObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause Modifies with an entity ref as the first argument
*/
class ModifiesEntityObject : public ClauseObject {
public:
	ModifiesEntityObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};



/*
* This class represents a Query object, for clause Follows
*/
class FollowsObject : public ClauseObject {
public:
	FollowsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause Follows*
*/
class FollowsStarObject : public ClauseObject {
public:
	FollowsStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

};

/*
* This class represents a Query object, for clause Parent
*/
class ParentObject : public ClauseObject {
public:
	ParentObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause Parent*
*/
class ParentStarObject : public ClauseObject {
public:
	ParentStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause Calls
*/
class CallsObject : public ClauseObject {
public:
	CallsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;
};

/*
* This class represents a Query object, for clause CallsStar
*/
class CallsStarObject : public ClauseObject {
public:
	CallsStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

};

/*
* This class represents a Query object, for clause Next
*/
class NextObject : public ClauseObject {
public:
	NextObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

};

/*
* This class represents a Query object, for clause Next*
*/
class NextStarObject : public ClauseObject {
public:
	NextStarObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

	string getCacheName() override;

	bool shouldCache() override;

};

/*
* This class represents a Query object, for clause Affects
*/
class AffectsObject : public ClauseObject {
public:
	AffectsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override;

	string getCacheName() override;

	bool shouldCache() override;

};
#endif
