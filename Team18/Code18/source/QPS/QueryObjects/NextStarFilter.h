#pragma once
#ifndef NEXTSTARFILTER_H
#define NEXTSTARFILTER_H

#include "QueryEval.h"



/*
* This class encapsulates the logic handler for Next*
* Reuses some classes from ClauseTableFilter for certain argument combinations
*/
class NextStarFilter : public QueryEval {
protected:
	shared_ptr<ClauseArg> arg1;
	shared_ptr<ClauseArg> arg2;
public:
	NextStarFilter(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : arg1(argument1), arg2(argument2) {

	}
};

class NextStarSynSyn : public NextStarFilter {
public:
	NextStarSynSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : NextStarFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class NextStarSynInt : public NextStarFilter {
public:
	NextStarSynInt(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : NextStarFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class NextStarIntSyn : public NextStarFilter {
public:
	NextStarIntSyn(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : NextStarFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class NextStarIntInt : public NextStarFilter {
public:
	NextStarIntInt(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2) : NextStarFilter{ argument1 , argument2 } {

	}
	shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) override;
};

class NextStarFilterFactory {
public:
	static shared_ptr<QueryEval> create(shared_ptr<ClauseArg> argument1, shared_ptr<ClauseArg> argument2);
};

#endif