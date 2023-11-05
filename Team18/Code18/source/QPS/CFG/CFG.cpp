#include "CFG.h"

using namespace std;

bool ExtendedCFG::isStmtType(string stmtNumber, ENTITY entity) {
  unordered_set<string> entitySet = statementEntities.at(entity);
  return entitySet.find(stmtNumber) != entitySet.end();
}

// DFS FUNCTIONS
void ExtendedCFG::DFSIfHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes) {
  unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
  for (shared_ptr<DFSPathNode> path : newPaths) {
    incompletePaths.push(path);
  }
}

void ExtendedCFG::DFSWhileHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes, string& currLineNumber) {
  curr->incrementWhileVisitCount(currLineNumber);
  int thisWhileVisitCount = curr->getWhileVisitCount(currLineNumber);
  if (thisWhileVisitCount == 1) {
    // First time visiting. Diverge paths one towards while block, one towards next stmt in same stmtList if applicable.
    unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
    for (shared_ptr<DFSPathNode> path : newPaths) {
      incompletePaths.push(path);
    }
  }
  else if (thisWhileVisitCount == 2) {
    // Second time visiting. This is only possible for the path that went into the loop in the first time visiting.
    // Continue moving into the loop to ensure Next* and Affects correctness.
    // The Child node that has the smaller line number is the child node inside the while block.
    unsigned long minimum = -1; // should be maximum long value
    for (string nextNodeStr : nextNodes) {
      minimum = std::min(stoul(nextNodeStr), minimum);
    }
    incompletePaths.push(DFSPathNode::traverse(curr, to_string(minimum)));
  }
  else if (thisWhileVisitCount == 3) {
    // Third time visiting. This is only possible for the path that went into the loop, and had went into the loop again once.
    // Now, skip the loop and move to the child node outside the while block (in the same stmtList as the while)
    // This child node has the larger line number.
    size_t numNextNodes = nextNodes.size();
    if (numNextNodes == 1) {
      // This while node is terminal
      finishedPaths.insert(curr);
      return;
    }
    // numNextNodes == 2
    unsigned long maximum = 0; // should be minimum long value
    for (string nextNodeStr : nextNodes) {
      maximum = std::max(stoul(nextNodeStr), maximum);
    }
    string nextLineNumber = to_string(maximum);
    incompletePaths.push(DFSPathNode::traverse(curr, nextLineNumber));
  }

}

void ExtendedCFG::DFSEncounteredCacheHelper(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr) {
  string currLineNum = curr->getLineNumber();
  unordered_set<shared_ptr<DFSPathNode>> cachedPaths = frozenPaths.at(currLineNum);
  unordered_set<shared_ptr<DFSPathNode>> restoredPaths = DFSPathNode::joinDescendants(curr, cachedPaths);
  finishedPaths.insert(restoredPaths.begin(), restoredPaths.end());
}

void ExtendedCFG::DFS(string start) {
  stack<shared_ptr<DFSPathNode>> incompletePaths;
  unordered_set<shared_ptr<DFSPathNode>> finishedPaths;
  shared_ptr<DFSPathNode> startNode = make_shared<DFSPathNode>(start); // holds on to the start node
  incompletePaths.push(startNode);
  shared_ptr<DFSPathNode> curr = nullptr;

  while (!incompletePaths.empty()) {
    curr = incompletePaths.top(); incompletePaths.pop();
    string currLineNumber = curr->getLineNumber();
    if (isCompleted(currLineNumber)) {
      // skip traversing, allow unrolling from completed node upwards
      DFSEncounteredCacheHelper(finishedPaths, curr);
      continue;
    }
    bool isWhile = isStmtType(currLineNumber, WHILE);
    bool isIf = isStmtType(currLineNumber, IF);

    unordered_set<string> nextNodes = next.at(currLineNumber);
    if (nextNodes.size() == 0) {
      // no more nextNodes, so this path is complete.
      finishedPaths.insert(curr);
      continue;
    }
    if (isIf) {
      // Should have exactly 2 nextNodes
      DFSIfHelper(incompletePaths, curr, nextNodes);
      continue;
    }
    if (isWhile) {
      DFSWhileHelper(incompletePaths, finishedPaths, curr, nextNodes, currLineNumber);
    }
  }
  unrollPaths(finishedPaths);
}

// UNROLL FUNCTIONS
void ExtendedCFG::storePath(string lineNumber, shared_ptr<DFSPathNode>& curr) {
  if (frozenPaths.find(lineNumber) == frozenPaths.end()) {
    frozenPaths.emplace(lineNumber, unordered_set<shared_ptr<DFSPathNode>>{ curr });
  }
  else {
    frozenPaths.at(lineNumber).insert(curr);
  }
}

void ExtendedCFG::restoreDescendants(deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
  bool skippedFirst = false;
  while (curr != nullptr) {
    if (!skippedFirst) { // so that the current is not added to the descendant list in a duplicate way (since iteratorHelper does it already)
      skippedFirst = true;
    }
    else {
      descendants.push_back(curr->getLineNumber());
    }
    curr = curr->getChild();
  }
}

void ExtendedCFG::unrollPathsIteratorHelper(string& currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
  completed.insert(currLineNumber);
  storePath(currLineNumber, curr);
  descendants.push_front(currLineNumber); // closely related descendants are at the front of the queue
  curr = curr->getParent();
}

void ExtendedCFG::addNextStar(string lineNumber, unordered_set<string> toAdd) {
  for (string s : toAdd) {
    addNextStar(lineNumber, s);
  }
}

void ExtendedCFG::addNextStar(string lineNumber, string toAdd) {
  if (nextStar.find(lineNumber) == nextStar.end()) {
    nextStar.emplace(lineNumber, unordered_set<string>{ toAdd });
  }
  else {
    nextStar.at(lineNumber).insert(toAdd);
  }

  if (nextStarInverse.find(lineNumber) == nextStarInverse.end()) {
    nextStarInverse.emplace(toAdd, unordered_set<string>{lineNumber});
  }
  else {
    nextStarInverse.at(toAdd).insert(lineNumber);
  }
}

void ExtendedCFG::addAffects(string lineNumber, string toAdd) {
  if (affects.find(lineNumber) == affects.end()) {
    affects.emplace(lineNumber, unordered_set<string>{ toAdd });
  }
  else {
    affects.at(lineNumber).insert(toAdd);
  }

  if (affectsInverse.find(lineNumber) == affectsInverse.end()) {
    affectsInverse.emplace(toAdd, unordered_set<string>{lineNumber});
  }
  else {
    affectsInverse.at(toAdd).insert(lineNumber);
  }
}

// returns true if the loop iteration should be skipped.
void ExtendedCFG::unrollAffectsHelper(deque<string>& descendants, shared_ptr<DFSPathNode>& curr, string& currLineNumber) {
  if (!isStmtType(currLineNumber, ASSIGN)) {
    return;
  }
  // Assign Statements
  string currVarModified = *(modifies.at(currLineNumber).begin());
  bool encounteredModifies = false;
  auto iter = descendants.begin(); //iterate from most recent descendant downwards
  for (; iter != descendants.end() && !encounteredModifies; iter++) {
    string descLineNumber = *iter;
    if (isStmtType(descLineNumber, ASSIGN)) {
      unordered_set<string> descVarUsed = uses.at(descLineNumber);
      bool varExistsInUsed = descVarUsed.find(currVarModified) != descVarUsed.end();
      if (varExistsInUsed) { //encounteredModifies should be false here due to for loop condition
        addAffects(currLineNumber, descLineNumber);
      }
    }
    bool isReadAssignCall = isStmtType(descLineNumber, READ) ||
      isStmtType(descLineNumber, ASSIGN) || isStmtType(descLineNumber, CALL);
    if (!isReadAssignCall) {
      continue;
    }
    unordered_set<string> descVarModified = modifies.at(descLineNumber);
    bool varExistsInModified = descVarModified.find(currVarModified) != descVarModified.end();
    if (varExistsInModified) {
      encounteredModifies = true;
    }
  }
  return;

}

void ExtendedCFG::unrollPaths(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths) {
  for (shared_ptr<DFSPathNode> currPath : finishedPaths) {
    bool terminalNode = true;
    deque<string> descendants;
    shared_ptr<DFSPathNode> curr = currPath;
    while (curr != nullptr) {
      string currLineNumber = curr->getLineNumber();
      if (terminalNode) {
        terminalNode = false;
        restoreDescendants(descendants, curr);
        unrollPathsIteratorHelper(currLineNumber, descendants, curr);
        continue;
      }
      // Not terminal Node
      addNextStar(currLineNumber, descendants[0]); // for Next*
      unrollAffectsHelper(descendants, curr, currLineNumber); // for affects
      unrollPathsIteratorHelper(currLineNumber, descendants, curr); // to iterate
    }

  }
}

// Constructor
ExtendedCFG::ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer) {
  procNames = dataAccessLayer->getAllProcedures();
  for (string procName : procNames) {
    pair<string, string> procBoundaries = dataAccessLayer->getProcLines(procName);
    stmtNumBoundaries.emplace(procName, procBoundaries);
  }
  next = dataAccessLayer->getClause(NEXT);
  nextStar = next; // initially set nextStar as next
  modifies = dataAccessLayer->getClause(MODIFIES);
  uses = dataAccessLayer->getClause(USES);
  statementEntities = {
    {ASSIGN, dataAccessLayer->getEntity(ASSIGN)},
    {IF, dataAccessLayer->getEntity(IF)},
    {WHILE, dataAccessLayer->getEntity(WHILE)},
    {READ, dataAccessLayer->getEntity(READ)},
    {CALL, dataAccessLayer->getEntity(CALL)},
  };
}

// Helper functions to the query functions
string ExtendedCFG::procedureNameContainingThis(string lineNumber) {
  for (const auto& p : stmtNumBoundaries) {
    string procName = p.first;
    pair<string, string> boundaries = p.second;
    int low = stoi(boundaries.first);
    int high = stoi(boundaries.second);
    int lineNumInt = stoi(lineNumber);
    if (lineNumInt >= low && lineNumInt <= high) {
      return procName;
    }
  }
  return "";
}

bool ExtendedCFG::isInSameProcedure(string lineNumber1, string lineNumber2) {
  return procedureNameContainingThis(lineNumber1) == procedureNameContainingThis(lineNumber2);
}

string ExtendedCFG::getFirstLineNumberOfProc(string procName) {
  return stmtNumBoundaries.at(procName).first;
}

bool ExtendedCFG::isCompleted(string stmtNumber) {
  return completed.find(stmtNumber) != completed.end();
}

void ExtendedCFG::ensureCompleted(string lineNumber) {
  if (!isCompleted(lineNumber)) {
    DFS(lineNumber);
  }
}

void ExtendedCFG::ensureProcCompleted(string procName) {
  string firstLine = getFirstLineNumberOfProc(procName);
  ensureCompleted(firstLine);
}

bool ExtendedCFG::intIntHelper(string from, string to) {
  if (!isInSameProcedure(from, to)) {
    return false;
  }
  ensureCompleted(from);
  return true;
}

void ExtendedCFG::synIntHelper(string lineNumber) {
  string procName = procedureNameContainingThis(lineNumber);
  ensureProcCompleted(procName);
}

// NEXT STAR
bool ExtendedCFG::nextStarIntInt(string from, string to) {
  if (!intIntHelper(from, to)) return false;
  unordered_set<string> fromNextStar = nextStar.at(from);
  return fromNextStar.find(to) != fromNextStar.end();
}

unordered_set<string> ExtendedCFG::nextStarSynInt(string to) {
  synIntHelper(to);
  unordered_set<string> toNextStarInv = nextStarInverse.at(to);
  return toNextStarInv;
}

unordered_set<string> ExtendedCFG::nextStarIntSyn(string from) {
  ensureCompleted(from);
  unordered_set<string> fromNextStar = nextStar.at(from);
  return fromNextStar;
}

StringMap ExtendedCFG::nextStarSynSyn() {
  for (string procName : procNames) {
    ensureProcCompleted(procName);
  }
  return nextStar;
}

// AFFECTS
bool ExtendedCFG::affectsIntInt(string from, string to) {
  if (!intIntHelper(from, to)) return false;
  unordered_set<string> fromAffects = affects.at(from);
  return fromAffects.find(to) != fromAffects.end();
}

unordered_set<string> ExtendedCFG::affectsSynInt(string to) {
  synIntHelper(to);
  unordered_set<string> toAffectsInverse = affectsInverse.at(to);
  return toAffectsInverse;
}

unordered_set<string> ExtendedCFG::affectsIntSyn(string from) {
  ensureCompleted(from);
  unordered_set<string> fromAffects = affects.at(from);
  return fromAffects;
}

StringMap ExtendedCFG::affectsSynSyn() {
  for (string procName : procNames) {
    ensureProcCompleted(procName);
  }
  return affects;
}