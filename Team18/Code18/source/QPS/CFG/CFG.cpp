#include "CFG.h"

#include "../../ThreadPool.h"

using namespace std;

bool ExtendedCFG::isStmtType(string stmtNumber, ENTITY entity) {
  unordered_set<string> entitySet = statementEntities.at(entity);
  return containerHasKey(entitySet, stmtNumber);
}

bool ExtendedCFG::hasNext(string stmtNumber) {
  bool exists = containerHasKey(next, stmtNumber);
  if (!exists) {
    return false;
  }
  return next.at(stmtNumber).size() != 0;
}

unordered_set<string> ExtendedCFG::getUsedVariables(string stmtNumber) {
  bool usesAnything = containerHasKey(uses, stmtNumber);
  if (!usesAnything) {
    return {};
  }
  return uses.at(stmtNumber);
}

// DFS FUNCTIONS
void ExtendedCFG::DFSIncompletePathsHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, 
  shared_ptr<DFSPathNode> toAdd, string currLineNum) {
  string nextLineNumber = toAdd->getLineNumber();
  addNextStarInv(currLineNum, nextLineNumber);
  incompletePaths.push(toAdd);
}

void ExtendedCFG::DFSIfHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes) {
  unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
  for (shared_ptr<DFSPathNode> path : newPaths) {
    DFSIncompletePathsHelper(incompletePaths, path, curr->getLineNumber());
  }
}

void ExtendedCFG::DFSWhileHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes, string& currLineNumber) {
  curr->incrementWhileVisitCount(currLineNumber);
  int thisWhileVisitCount = curr->getWhileVisitCount(currLineNumber);
  if (thisWhileVisitCount == 1) {
    // First time visiting. Diverge paths one towards while block, one towards next stmt in same stmtList if applicable.
    unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
    for (shared_ptr<DFSPathNode> path : newPaths) {
      DFSIncompletePathsHelper(incompletePaths, path, currLineNumber);
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
    DFSIncompletePathsHelper(incompletePaths, DFSPathNode::traverse(curr, to_string(minimum)), currLineNumber);
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
    DFSIncompletePathsHelper(incompletePaths, DFSPathNode::traverse(curr, nextLineNumber), currLineNumber);
  }

}

void ExtendedCFG::DFSEncounteredCacheHelper(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr) {
  string currLineNum = curr->getLineNumber();
  unordered_set<shared_ptr<DFSPathNode>> cachedPaths = frozenPaths.at(currLineNum);
  unordered_set<shared_ptr<DFSPathNode>> restoredPaths = DFSPathNode::joinDescendants(curr, cachedPaths);
  finishedPaths.insert(restoredPaths.begin(), restoredPaths.end());
}

void ExtendedCFG::DFS(string start, bool calculateAffects, bool calcInverse) {
  stack<shared_ptr<DFSPathNode>> incompletePaths;
  unordered_set<shared_ptr<DFSPathNode>> finishedPaths;
  shared_ptr<DFSPathNode> startNode = make_shared<DFSPathNode>(start); // holds on to the start node
  incompletePaths.push(startNode);
  shared_ptr<DFSPathNode> curr = nullptr;

  while (!incompletePaths.empty()) {
    curr = incompletePaths.top(); incompletePaths.pop();
    string currLineNumber = curr->getLineNumber();
    bool affectsCompletedCheck = !calculateAffects || isAffectsCompleted(currLineNumber);
    bool nextStarCompletedCheck = isNextStarCompleted(currLineNumber);
    if (nextStarCompletedCheck && affectsCompletedCheck) {
      // skip traversing, allow unrolling from completed node upwards
      DFSEncounteredCacheHelper(finishedPaths, curr);
      continue;
    }
    bool isWhile = isStmtType(currLineNumber, WHILE);
    bool isIf = isStmtType(currLineNumber, IF);

    bool hasNextNode = hasNext(currLineNumber);
    if (!hasNextNode) {
      // no more nextNodes, so this path is complete.
      finishedPaths.insert(curr);
      continue;
    }
    unordered_set<string> nextNodes = next.at(currLineNumber);
    if (isIf) {
      // Should have exactly 2 nextNodes
      DFSIfHelper(incompletePaths, curr, nextNodes);
      continue;
    }
    if (isWhile) {
      DFSWhileHelper(incompletePaths, finishedPaths, curr, nextNodes, currLineNumber);
      continue;
    }
    // regular statements should only have 1 next node
    
    DFSIncompletePathsHelper(incompletePaths, DFSPathNode::traverse(curr, *(nextNodes.begin())), currLineNumber);
  }
  unrollPaths(finishedPaths, calculateAffects);
}


void ExtendedCFG::InverseDFS(string end, bool calculateAffects) {

}

// UNROLL FUNCTIONS
void ExtendedCFG::storePath(string lineNumber, shared_ptr<DFSPathNode> curr) {
  if (!containerHasKey(frozenPaths, lineNumber)) {
    frozenPaths.emplace(lineNumber, unordered_set<shared_ptr<DFSPathNode>>{ curr });
  }
  else {
    frozenPaths.at(lineNumber).insert(curr);
  }
}

void ExtendedCFG::restoreDescendants(deque<string>& descendants, shared_ptr<DFSPathNode> curr) {
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

void ExtendedCFG::unrollPathsIteratorHelper(string currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
  storePath(currLineNumber, curr);
  descendants.push_front(currLineNumber); // closely related descendants are at the front of the queue
  curr = curr->getParent();
}

void ExtendedCFG::addNextStar(string lineNumber, string toAdd) {
  if (containerHasKey(nextStar, lineNumber)) {
    nextStar.at(lineNumber).insert(toAdd);
  }
  else {
    nextStar.emplace(lineNumber, unordered_set<string>{ toAdd });
  }
}
void ExtendedCFG::addNextStarInv(string lineNumber, string toAdd) {
  if (containerHasKey(nextStarInverse, toAdd)) {
    nextStarInverse.at(toAdd).insert(lineNumber);
  }
  else {
    nextStarInverse.emplace(toAdd, unordered_set<string>{lineNumber});
  }
}

void ExtendedCFG::addAffects(string lineNumber, string toAdd) {
  if (containerHasKey(affects, lineNumber)) {
    affects.at(lineNumber).insert(toAdd);
  }
  else {
    affects.emplace(lineNumber, unordered_set<string>{ toAdd });
  }

  if (containerHasKey(affectsInverse, toAdd)) {
    affectsInverse.at(toAdd).insert(lineNumber);
  }
  else {
    affectsInverse.emplace(toAdd, unordered_set<string>{lineNumber});
  }
}

void ExtendedCFG::unrollNextStarHelper(string currLineNumber, deque<string>& descendants) {
  nextStarCompleted.insert(currLineNumber);
  addNextStar(currLineNumber, descendants[0]);
  //for (string descendant : descendants) {
  //  addNextStar(currLineNumber, descendant);
  //}
}

// returns true if the loop iteration should be skipped.
void ExtendedCFG::unrollAffectsHelper(deque<string>& descendants, shared_ptr<DFSPathNode>& curr, string currLineNumber) {
  affectsCompleted.insert(currLineNumber);
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
      unordered_set<string> descVarUsed = getUsedVariables(descLineNumber);
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
    bool lineNumberModifiesSomething = containerHasKey(modifies, descLineNumber) && !modifies.at(descLineNumber).empty();
    if (!lineNumberModifiesSomething) {
      continue;
    }
    unordered_set<string> descVarModified = modifies.at(descLineNumber);
    bool varExistsInModified = containerHasKey(descVarModified, currVarModified);
    if (varExistsInModified) {
      encounteredModifies = true;
    }
  }
  return;

}
void ExtendedCFG::unrollOnePath(shared_ptr<DFSPathNode> currPath, bool calculateAffects) {
  bool terminalNode = true;
  deque<string> descendants;
  shared_ptr<DFSPathNode> curr = currPath;
  while (curr != nullptr) {
    string currLineNumber = curr->getLineNumber();
    if (terminalNode) {
      terminalNode = false;
      if (calculateAffects) {
        restoreDescendants(descendants, curr);
      }
      unrollPathsIteratorHelper(currLineNumber, descendants, curr);
      nextStarCompleted.insert(currLineNumber);
      affectsCompleted.insert(currLineNumber);
      continue;
    }
    // Not terminal Node
    unrollNextStarHelper(currLineNumber, descendants); // for Next*
    if (calculateAffects) {
      unrollAffectsHelper(descendants, curr, currLineNumber); // for affects
    }
    unrollPathsIteratorHelper(currLineNumber, descendants, curr); // to iterate
  }
}

void ExtendedCFG::unrollPaths(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, bool calculateAffects) {
  ThreadPool tp;
  for (shared_ptr<DFSPathNode> currPath : finishedPaths) {
    tp.addTask([this, currPath, calculateAffects]() {unrollOnePath(currPath, calculateAffects); });
    //unrollOnePath(currPath, calculateAffects);
    //bool terminalNode = true;
    //deque<string> descendants;
    //shared_ptr<DFSPathNode> curr = currPath;
    //while (curr != nullptr) {
    //  string currLineNumber = curr->getLineNumber();
    //  if (terminalNode) {
    //    terminalNode = false;
    //    if (calculateAffects) {
    //      restoreDescendants(descendants, curr);
    //    }
    //    unrollPathsIteratorHelper(currLineNumber, descendants, curr);
    //    nextStarCompleted.insert(currLineNumber);
    //    affectsCompleted.insert(currLineNumber);
    //    continue;
    //  }
    //  // Not terminal Node
    //  unrollNextStarHelper(currLineNumber, descendants); // for Next*
    //  if (calculateAffects) {
    //    unrollAffectsHelper(descendants, curr, currLineNumber); // for affects
    //  }
    //  unrollPathsIteratorHelper(currLineNumber, descendants, curr); // to iterate
    //}
  }
  tp.wait();
}

// Constructor
ExtendedCFG::ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer) {
  procNames = dataAccessLayer->getAllProcedures();
  maxStmtNum = 0;
  for (string procName : procNames) {
    pair<string, string> procBoundaries = dataAccessLayer->getProcLines(procName);
    if (stoi(procBoundaries.second) > maxStmtNum) {
      maxStmtNum = stoi(procBoundaries.second);
    }
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

string INVALID_PROCEDURE = "-1";
// Helper functions to the query functions
bool ExtendedCFG::isValidLineNumber(string lineNumber) {
  size_t lineNumInt = stoi(lineNumber);
  if (lineNumInt < 1 || lineNumInt > maxStmtNum) {
    return false;
  }
  return true;
}

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
  return INVALID_PROCEDURE;
}

bool ExtendedCFG::isInSameProcedure(string lineNumber1, string lineNumber2) {
  string proc1 = procedureNameContainingThis(lineNumber1);
  string proc2 = procedureNameContainingThis(lineNumber2);
  if (proc1 == INVALID_PROCEDURE || proc2 == INVALID_PROCEDURE) {
    return false;
  }
  return proc1 == proc2;
}

string ExtendedCFG::getFirstLineNumberOfProc(string procName) {
  return stmtNumBoundaries.at(procName).first;
}

bool ExtendedCFG::isNextStarCompleted(string stmtNumber) {
  return containerHasKey(nextStarCompleted, stmtNumber);
}

bool ExtendedCFG::isAffectsCompleted(string stmtNumber) {
  return containerHasKey(affectsCompleted, stmtNumber);
}

bool ExtendedCFG::isCompleted(string stmtNumber) {
  return isNextStarCompleted(stmtNumber) && isAffectsCompleted(stmtNumber);
}

bool ExtendedCFG::ensureCompleted(string lineNumber, bool calculateAffects) {
  if (!isValidLineNumber(lineNumber)) return false;
  if (!isCompleted(lineNumber)) {
    DFS(lineNumber, calculateAffects, true);
  }
  return true;
}

void ExtendedCFG::ensureProcCompleted(string procName, bool calculateAffects) {
  string firstLine = getFirstLineNumberOfProc(procName);
  ensureCompleted(firstLine, calculateAffects);
}


bool ExtendedCFG::hasAffects(string from) {
  return containerHasKey(affects, from);
}

bool ExtendedCFG::hasAffectsInv(string to) {
  return containerHasKey(affectsInverse, to);
}

bool ExtendedCFG::hasNextStarInv(string to) {
  return containerHasKey(nextStarInverse, to);
}

bool ExtendedCFG::intIntHelper(string from, string to, bool calculateAffects) {
  if (!isInSameProcedure(from, to)) {
    return false;
  }
  ensureCompleted(from, calculateAffects);
  return true;
}

bool ExtendedCFG::synIntHelper(string lineNumber, bool calculateAffects) {
  string procName = procedureNameContainingThis(lineNumber);
  if (procName == INVALID_PROCEDURE) {
    return false;
  }
  ensureProcCompleted(procName, calculateAffects);
  return true;
}

// NEXT STAR
bool ExtendedCFG::nextStarIntInt(string from, string to) {
  if (!intIntHelper(from, to, false)) return false;
  bool resultsExist = hasNext(from);
  if (!resultsExist) {
    return false;
  }
  unordered_set<string> fromNextStar = nextStar.at(from);
  return containerHasKey(fromNextStar, to);
}

unordered_set<string> ExtendedCFG::nextStarSynInt(string to) {
  bool isValid = synIntHelper(to, false);
  if (!isValid) {
    return {};
  }
  bool resultsExist = hasNextStarInv(to);
  if (!resultsExist) {
    return {};
  }
  unordered_set<string> toNextStarInv = nextStarInverse.at(to);
  return toNextStarInv;
}

unordered_set<string> ExtendedCFG::nextStarIntSyn(string from) {
  bool isValid = ensureCompleted(from, false);
  if (!isValid) {
    return {};
  }
  bool resultsExist = hasNext(from);
  if (!resultsExist) {
    return {};
  }
  unordered_set<string> fromNextStar = nextStar.at(from);
  return fromNextStar;
}

StringMap ExtendedCFG::nextStarSynSyn() {
  ThreadPool tp;
  for (string procName : procNames) {
    tp.addTask([this, procName]() {ensureProcCompleted(procName, false); });
    //ensureProcCompleted(procName, false);
  }
  tp.wait();
  return nextStar;
}

// AFFECTS
bool ExtendedCFG::affectsIntInt(string from, string to) {
  if (!intIntHelper(from, to, true)) return false;
  bool resultsExist = hasAffects(from);
  if (!resultsExist) {
    return {};
  }
  unordered_set<string> fromAffects = affects.at(from);
  return containerHasKey(fromAffects, to);
}

unordered_set<string> ExtendedCFG::affectsSynInt(string to) {
  bool isValid = synIntHelper(to, true);
  if (!isValid) {
    return {};
  }
  bool resultsExist = hasAffectsInv(to);
  if (!resultsExist) {
    return {};
  }
  unordered_set<string> toAffectsInverse = affectsInverse.at(to);
  return toAffectsInverse;
}

unordered_set<string> ExtendedCFG::affectsIntSyn(string from) {
  bool isValid = ensureCompleted(from, true);
  if (!isValid) {
    return {};
  }
  bool resultsExist = hasAffects(from);
  if (!resultsExist) {
    return {};
  }
  unordered_set<string> fromAffects = affects.at(from);
  return fromAffects;
}

StringMap ExtendedCFG::affectsSynSyn() {
  ThreadPool tp;
  for (string procName : procNames) {
    tp.addTask([this, procName]() {ensureProcCompleted(procName, true); });
    //ensureProcCompleted(procName, true);
  }
  tp.wait();
  return affects;
}