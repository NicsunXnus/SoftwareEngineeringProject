#pragma once
#ifndef QUERYEVAL_H
#define QUERYEVAL_H

using namespace std;

#include "../../Constants/DesignEnums.h"
#include "../DataAccessLayer.h"
#include "ClauseArg.h"
#include "../Errors/SemanticError.h"

/*
* This abstract class encapsulates the logic handler for query objects
*/
class QueryEval {
public:
	virtual shared_ptr<QueryResultsTable> evaluate(shared_ptr<DataAccessLayer> dataAccessLayer, ABSTRACTION clause) = 0;
};



#endif