#pragma once
#include "DataAccessLayerAffectsStub.h"

#include <list>

#include "../SP/SimpleProcessor/ExpressionProcessor.h"

// Program is as follows:

/*
 1  procedure First{read x; 
 2    read z;
 3    call Second; }
 4  procedure Second{x = 0;
 5    i = 5;
 6    while (i != 0) {
 7      x = x + 2 * y;
 8      call Third;
 9      i = i - 1;}
10    if (x == 1) then {
11      x = x + 1; }
12    else {z = 1;}
13    z = z + x + i;
14    y = z + 2;
15    x = x * y + z; }
16  procedure Third{z = 5;
17    v = z;
18    print v;
19    read Third; }

*/


unordered_set<string> DataAccessLayerAffectsStub::getEntity(ENTITY type) {
    unordered_set<string> test;
    if (type == ASSIGN) {
        unordered_set<string> assignments = { "4", "5", "7", "9", "11", "12", "13", "14", "15", "16", "17" };
        return assignments;
    }
    if (type == IF) {
      unordered_set<string> assignments = { "10", };
      return assignments;
    }
    if (type == WHILE) {
      unordered_set<string> assignments = { "6" };
      return assignments;
    }
    if (type == READ) {
      unordered_set<string> assignments = { "1", "2", "19"};
      return assignments;
    }
    if (type == CALL) {
      unordered_set<string> assignments = { "3", "8"};
      return assignments;
    }
    if (type == PRINT) {
      unordered_set<string> assignments = { "18" };
      return assignments;
    }
    if (type == PROCEDURE) {
      unordered_set<string> assignments = { "First", "Second", "Third"};
      return assignments;
    }

    return test;
}

unordered_set<string> DataAccessLayerAffectsStub::getAllProcedures() {
    return {"First", "Second", "Third"};
}

unordered_set<string> DataAccessLayerAffectsStub::getAllVariables() {
    unordered_set<string> test;

    return test;
}

unordered_set<string> DataAccessLayerAffectsStub::getAllConstants() {
    unordered_set<string> test;

    return test;
}

StringMap DataAccessLayerAffectsStub::getVariableMap() { 
    StringMap test;
    return test;
}

StringMap DataAccessLayerAffectsStub::getConstantMap() {
    StringMap test;
    return test;
}

shared_ptr<Node> DataAccessLayerAffectsStub::getPatternTree(string statement_number) {
    ExpressionProcessor ep = ExpressionProcessor();

    map<string, shared_ptr<Node>> patternTree = {
        {"1", ep.nodifyArithmeticExpression("a + 100 - b")},
        {"2", ep.nodifyArithmeticExpression("d / c")},
        {"3", ep.nodifyArithmeticExpression("300")},
        {"4", ep.nodifyArithmeticExpression("b * (c * 300)")},
    };
    return patternTree.at(statement_number);
}

StringMap DataAccessLayerAffectsStub::getClause(ABSTRACTION abstraction) {
    StringMap test;
    if (abstraction == USES) {
        StringMap uses = { 
            {"10", {"x"}}, 
            {"11", {"x"}}, 
            {"13", {"i", "x","z"}}, 
            {"14", {"z"}}, 
            {"15", {"x", "y", "z"}}, 
            {"17", {"z"}}, 
            {"18", {"v"}}, 
            {"3", {"y","i","v","x","z"}},
            {"6", {"y","i","v","x","z"}},
            {"7", {"x","y"}},
            {"8", {"v","z"}},
            {"9", {"i"}} 
        };
        return uses;
    }
    else if (abstraction == MODIFIES) {
        StringMap modifies = {
{"1", {"x"}},
{"10", {"x", "z"}},
{"11", {"x"}},
{"12", {"z"}},
{"13", {"z"}},
{"14", {"y"}},
{"15", {"x"}},
{"16", {"z"}},
{"17", {"v"}},
{"19", {"Third"}},
{"1", {"x"}},
{"2", {"z"}},
{"3", {"Third","y","i","v","x","z"}},
{"4", {"x"}},
{"5", {"i"}},
{"6", {"Third","i","v","x","z"}},
{"7", {"x"}},
{"8", {"Third","v","z"}},
{"9", {"i"}},
        };
        return modifies;
    }
    else if (abstraction == NEXT) {
        StringMap next = { {"1", {"2"}},
{"10", {"11","12"}},
{"11", {"12","13"}},
{"12", {"13"}},
{"13", {"14"}},
{"14", {"15"}},
{"16", {"17"}},
{"17", {"18"}},
{"18", {"19"}},
{"2", {"3"}},
{"4", {"5"}},
{"5", {"6"}},
{"6", {"7","10"}},
{"7", {"8"}},
{"8", {"9"}},
{"9", {"6","10"}} };
        return next;
    }
    
    return test;
}

StringMap DataAccessLayerAffectsStub::getClauseInverse(ABSTRACTION abstraction) {
    if (abstraction == USES) {
        StringMap uses = { { "i", {"First","6","13","Second","9","3"} },
        { "v", {"18","Third","8","6","First","Second","3"} },
        { "x", {"7","Second","13","10","11","15","3","First","6"} },
        { "y", {"7","Second","15","3","6","First"} },
        { "z", {"13","Second","14","15","First","17","Third","3","8","6"} } };
        return uses;
    }
    StringMap test;
    return test;
}

StringMap DataAccessLayerAffectsStub::getCallProcNames() {
    StringMap test;
    return test;
}
StringMap DataAccessLayerAffectsStub::getReadVarNames() {
    StringMap test;
    return test;
}
StringMap DataAccessLayerAffectsStub::getPrintVarNames() {
    StringMap test;
    return test;
}

pair<string, string> DataAccessLayerAffectsStub::getProcLines(string procName) {
    map<string, pair<string, string>> procLines = { {"First", {"1", "3"}},
                                                    {"Second", {"4", "15"}},
                                                    {"Third", {"16", "19"}} };
    return procLines[procName];
}
