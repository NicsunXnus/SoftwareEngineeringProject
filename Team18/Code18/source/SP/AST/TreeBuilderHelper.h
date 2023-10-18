#ifndef TREEBUILDERHELPER_H
#define TREEBUILDERHELPER_H

#include <unordered_map>

#include "Node.h"

using namespace std;

// Originally done by Nicholas

bool isComparisonOp(string op);

bool isBoolOp(string op);

bool isArithOp(string op);

// Precedence of operations
static const inline std::unordered_map<std::string, int> precedence = {
  {"(", 0},
  {"||", 1},
  {"&&", 2},
  {"!", 3},
  {"==", 4}, {"!=", 4}, {">", 4}, {">=", 4}, {"<", 4}, {"<=", 4},
  {"+", 5}, {"-", 5},
  {"*", 6}, {"/", 6}, {"%", 6}
};

#endif