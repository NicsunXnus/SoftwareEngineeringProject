#include "TreeBuilderHelper.h"

bool isComparisonOp(string op) {
  vector<string> ops = { "<", "<=", ">", ">=", "==", "!=" };
  auto it = find(ops.begin(), ops.end(), op);
  return it != ops.end();
}

bool isBoolOp(string op) {
  vector<string> ops = { "!", "&&", "||" };
  auto it = find(ops.begin(), ops.end(), op);
  return it != ops.end();
}

bool isArithOp(string op) {
  vector<string> ops = { "+", "-", "/", "*", "%" };
  auto it = find(ops.begin(), ops.end(), op);
  return it != ops.end();
}