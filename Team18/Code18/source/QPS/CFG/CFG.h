#ifndef CFG_H
#define CFG_H

#include <deque>
#include <map>
#include <string>
#include <set>
#include <stack>
#include <unordered_set>
#include <memory>

#include "../../Constants/DesignEnums.h"
#include "../DataAccessLayer.h"
#include "DFSPathNode.h"

using namespace std;

class ExtendedCFG {
private:
  map<string, pair<string, string>> stmtNumBoundaries; // boundaries of each procedure in terms of statement numbers
  unordered_set<string> procNames;
  StringMap next; // children nodes
  StringMap modifies; // variables that this statement number modifies
  StringMap uses; // variables that this statement number uses
  map<ENTITY, unordered_set<string>> entityMap;

  StringMap nextStar; // <Line Number, Next*>, internally map<string, unordered_set<string>>
  StringMap nextStarInverse;
  StringMap affects; // Affects(s, collection<other_s>) is True
  StringMap affectsInverse; // AffectedBy(s, other_s) is True; 

  unordered_set<string> completed; // Stores line numbers of statements that have been evaluated completely
  unordered_map<string, unordered_set<shared_ptr<DFSPathNode>>> frozenPaths; // line number : set<frozen paths>

  string procedureNameContainingThis(string lineNumber);

  bool isInSameProcedure(string lineNumber1, string lineNumber2);

  string getFirstLineNumberOfProc(string procName);

  bool isCompleted(string stmtNumber);

  void ensureCompleted(string lineNumber);

  void ensureProcCompleted(string procName);

  bool intIntHelper(string from, string to);

  void synIntHelper(string lineNumber);

  bool isStmtType(string stmtNumber, ENTITY entity);

  void addNextStar(string lineNumber, unordered_set<string> toAdd);

  void addNextStar(string lineNumber, string toAdd);

  void addAffects(string lineNumber, string toAdd);

  void DFSIfHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes);

  // Returns true if the while node is terminal, and visit count == 3
  bool DFSWhileHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths,
    unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes, string& currLineNumber);

  void DFS(string start, bool forceTraverse = false);

  void storePath(string lineNumber, shared_ptr<DFSPathNode>& curr);

  void restoreDescendants(deque<string>& descendants, shared_ptr<DFSPathNode>& curr);

  void unrollPathsIteratorHelper(string currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr);

  // returns true if this loop iteration should be skipped.
  bool unrollAffectsHelper(deque<string>& descendants, shared_ptr<DFSPathNode>& curr, string& currLineNumber);

  void unrollPaths(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths);
public:
  ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer);
  // NEXT STAR
  bool nextStarIntInt(string from, string to);

  unordered_set<string> nextStarSynInt(string to);

  unordered_set<string> nextStarIntSyn(string from);

  StringMap nextStarSynSyn();

  // AFFECTS
  bool affectsIntInt(string from, string to);

  unordered_set<string> affectsSynInt(string to);

  unordered_set<string> affectsIntSyn(string from);

  StringMap affectsSynSyn();
};

#endif