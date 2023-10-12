#pragma once
#ifndef CLAUSEOBJECT_H
#define CLAUSEOBJECT_H

#include <string_view>
#include "QueryObject.h"
#include "ClauseArg.h"

/*
* This class represents a Query object, for clause entities
*/
class ClauseObject : public QueryObject {
private:
	shared_ptr<ClauseArg> argument0;
	shared_ptr<ClauseArg> argument1;

public:
	ClauseObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: QueryObject{ clauseName }, argument0{ argument0 }, argument1{ argument1 } {
	}

	// Constructor for creating an empty clause object, to be used in gracefully handling clauses with errors.
	ClauseObject(string_view name) : QueryObject{ name } {}


	shared_ptr<ClauseArg> getArg1() {
		return argument0;
	}

	shared_ptr<ClauseArg> getArg2() {
		return argument1;
	}

	
	
};
inline string col1 = "col1";
inline string col2 = "col2";

inline StringMap filterMapKeyReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	ENTITY typeArg1 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg1EntityData = dataAccessLayer->getEntity(typeArg1);
	StringMap filteredPKBClauseData;
	for (const auto& entry : PKBClauseData) {
		string leftTuple = entry.first;
		auto it = PKBArg1EntityData.find(leftTuple);
		if (it != PKBArg1EntityData.end()) {
			filteredPKBClauseData[entry.first] = entry.second;
		}
	}
	return filteredPKBClauseData;
}

inline StringMap filterMapValueReturnMap(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	ENTITY typeArg2 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg2EntityData = dataAccessLayer->getEntity(typeArg2);
	StringMap filteredPKBClauseData;
	for (const auto& entry : PKBClauseData) {
		unordered_set<string> intersection;
		unordered_set<string> rightTuple = entry.second;
		for (const string& element : rightTuple) {
			if (PKBArg2EntityData.find(element) != PKBArg2EntityData.end()) {
				intersection.insert(element);
			}
		}
		filteredPKBClauseData[entry.first] = intersection;
	}
	return filteredPKBClauseData;
}


inline unordered_set<string> filterSetReturnSet(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_set<string> PKBClauseData) {
	ENTITY typeArg2 = arg->getSynonym()->getEntityType();
	unordered_set<string> PKBArg2EntityData = dataAccessLayer->getEntity(typeArg2);
	unordered_set<string> filteredPKBClauseData;
	for (const string& element : PKBClauseData) {
		if (PKBArg2EntityData.find(element) != PKBArg2EntityData.end()) {
			filteredPKBClauseData.insert(element);
		}
	}
	return filteredPKBClauseData;
}

inline unordered_set<string> filterMapKeyReturnSetValues(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> filteredPKBClauseData;
	auto it = PKBClauseData.find(svToString(arg->getArgValue()));
	if (it != PKBClauseData.end()) {
		return PKBClauseData[svToString(arg->getArgValue())];
	}
	return filteredPKBClauseData;
}

inline unordered_set<string> filterMapKeyReturnSetValues(string arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> filteredPKBClauseData;
	auto it = PKBClauseData.find(arg);
	if (it != PKBClauseData.end()) {
		return PKBClauseData[arg];
	}
	return filteredPKBClauseData;
}


inline bool filterSetReturnBool(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, unordered_set<string> PKBClauseData) {
	auto it = PKBClauseData.find(svToString(arg->getArgValue()));
	if (it != PKBClauseData.end()) {
		return true;
	}

	return false;
}

inline unordered_set<string> removeMapValuesReturnSet(shared_ptr<ClauseArg> arg, shared_ptr<DataAccessLayer> dataAccessLayer, StringMap PKBClauseData) {
	unordered_set<string> keySet;

	// Extract keys from the map and insert them into the set
	for (const auto& pair : PKBClauseData) {
		keySet.insert(pair.first);
	}
	return keySet;
}

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

};

/*
* This class represents a Query object, for clause Affects
*/
class AffectsObject : public ClauseObject {
public:
	AffectsObject(string_view clauseName, shared_ptr<ClauseArg> argument0, shared_ptr<ClauseArg> argument1)
		: ClauseObject{ clauseName, argument0, argument1 } {
	};
	shared_ptr<QueryResultsTable> callAndProcess(shared_ptr<DataAccessLayer> dataAccessLayer) override {
		// TODO
		return make_shared<QueryResultsTable>();
	}

};
#endif
