#ifndef QPSCONSTANTS_H
#define QPSCONSTANTS_H

#include <string>

#include "DesignEnums.h"

using namespace std;
// BOOLEANS
static inline const string TRUE_STRING = "TRUE";
static inline const string FALSE_STRING = "FALSE";

// ERRORS
static inline const string QPS_ERROR = "QPS Internal Error";
static inline const string SEMANTIC_ERROR = "SemanticError";
static inline const string SYNTAX_ERROR = "SyntaxError";

// CFG
static inline const string CFG_CACHE_STRING = "CFG";

// Query Optimisation
static inline const map<ABSTRACTION, int> abstraction_weights_map = {
	{USES, 2},
	{MODIFIES, 2},
	{PARENT, 2},
	{PARENTSTAR, 2},
	{FOLLOWS, 2},
	{FOLLOWSSTAR, 2},
	{CALLS, 1},
	{CALLSSTAR, 1},
	{NEXT, 2},
	{NEXTSTAR, 8},
	{AFFECTS, 10},
};

static inline const int abstraction_criterion_weight = 3;

static inline const int synonym_count_weight = 2;

#endif