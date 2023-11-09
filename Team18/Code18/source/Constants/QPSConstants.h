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
    {USES, 6},    {MODIFIES, 6},    {PARENT, 6},   {PARENTSTAR, 6},
    {FOLLOWS, 6}, {FOLLOWSSTAR, 6}, {CALLS, 3},    {CALLSSTAR, 3},
    {NEXT, 6},    {NEXTSTAR, 24},   {AFFECTS, 27},
};

static inline const int WITH_CLAUSE_SCORE = 8;

static inline const int NOT_CLAUSE_SCORE = 4;  // additional

static inline const int PATTERN_CLAUSE_SCORE = 8;

static inline const int SYNONYM_COUNT_WEIGHT = 2;

#endif