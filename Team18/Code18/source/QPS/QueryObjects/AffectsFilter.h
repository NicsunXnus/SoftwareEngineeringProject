#pragma once
#ifndef AFFECTSFILTER_H
#define AFFECTSFILTER_H

#include "QueryEval.h"

/*
* This class encapsulates the logic handler for Affects, filtering the PKB data
*/
class AffectsFilter : public QueryEval {
protected:
	shared_ptr<ClauseArg> arg1;
	shared_ptr<ClauseArg> arg2;
public:
	AffectsFilter(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : arg1(argument1), arg2(argument2) {

	}
};

class AffectsSynSyn : public AffectsFilter {
public:
	AffectsSynSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsSynInt : public AffectsFilter {
public:
	AffectsSynInt(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsSynWildcard : public AffectsFilter {
public:
	AffectsSynWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsIntSyn : public AffectsFilter {
public:
	AffectsIntSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsIntInt : public AffectsFilter {
public:
	AffectsIntInt(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsIntWildcard : public AffectsFilter {
public:
	AffectsIntWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsWildcardSyn : public AffectsFilter {
public:
	AffectsWildcardSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsWildcardInt : public AffectsFilter {
public:
	AffectsWildcardInt(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsWildcardWildcard : public AffectsFilter {
public:
	AffectsWildcardWildcard(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : AffectsFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class AffectsFilterFactory {
public:
	static shared_ptr<QueryEval> create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2);
};



#endif