#ifndef CFG_H
#define CFG_H

#include <deque>
#include <map>
#include <string>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>

#include "../../Constants/DesignEnums.h"
#include "../DataAccessLayer.h"
#include "DFSPathNode.h"

using namespace std;

class ExtendedCFG {
private:
  map<string, pair<string, string>> stmtNumBoundaries; // boundaries of each procedure in terms of statement numbers
  StringMap next; // children nodes
  StringMap modifies; // variables that this statement number modifies
  StringMap uses; // variables that this statement number uses
  map<ENTITY, unordered_set<string>> entityMap;

  StringMap nextStar; // <Line Number, Next*>, internally map<string, unordered_set<string>>
  StringMap affects; // Affects(s, collection<s>) is True

  unordered_set<string> completed; // Stores line numbers of statements that have been evaluated completely
  unordered_map<string, unordered_set<shared_ptr<DFSPathNode>>> frozenPaths; // line number : set<frozen paths>

  void DFS(string start, bool forceTraverse=false) {
    stack<shared_ptr<DFSPathNode>> incompletePaths;
    unordered_set<shared_ptr<DFSPathNode>> finishedPaths;
    shared_ptr<DFSPathNode> startNode = make_shared<DFSPathNode>(start);
    incompletePaths.push(startNode);
    shared_ptr<DFSPathNode> curr = nullptr;
    while (!incompletePaths.empty()) {
      curr = incompletePaths.top(); incompletePaths.pop();
      string currLineNumber = curr->getLineNumber();
      if (!forceTraverse && isCompleted(currLineNumber)) {
        // skip traversing, allow unrolling from completed node upwards
        finishedPaths.insert(curr);
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
        unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
        for (shared_ptr<DFSPathNode> path : newPaths) {
          incompletePaths.push(path);
        }
        continue;
      }
      if (isWhile) {
        curr->incrementWhileVisitCount(currLineNumber);
        int thisWhileVisitCount = curr->getWhileVisitCount(currLineNumber);
        if (thisWhileVisitCount == 1) {
          // First time visiting. Diverge paths one towards while block, one towards next stmt in same stmtList if applicable.
          unordered_set<shared_ptr<DFSPathNode>> newPaths = DFSPathNode::diverge(curr, nextNodes);
          for (shared_ptr<DFSPathNode> path : newPaths) {
            incompletePaths.push(path);
          }
          continue;
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
            continue;
          }
          // numNextNodes == 2
          unsigned long maximum = 0; // should be minimum long value
          for (string nextNodeStr : nextNodes) {
            maximum = std::max(stoul(nextNodeStr), maximum);
          }
          string nextLineNumber = to_string(maximum);
          incompletePaths.push(DFSPathNode::traverse(curr, nextLineNumber));
          continue;
        }
      }

      unrollPaths(finishedPaths);
    }
  }

  bool isStmtType(string stmtNumber, ENTITY entity) {
    unordered_set<string> entitySet = entityMap.at(entity);
    return entitySet.find(stmtNumber) != entitySet.end();
  }

  void addToAffects(string lineNumber, string toAdd) {
    if (affects.find(lineNumber) == affects.end()) {
      affects.emplace(lineNumber, unordered_set<string>{ toAdd });
    }
    else {
      affects.at(lineNumber).insert(toAdd);
    }
  }

  void storePath(string lineNumber, shared_ptr<DFSPathNode>& curr) {
    if (frozenPaths.find(lineNumber) == frozenPaths.end()) {
      frozenPaths.emplace(lineNumber, unordered_set<shared_ptr<DFSPathNode>>{ curr });
    }
    else {
      frozenPaths.at(lineNumber).insert(curr);
    }    
  }
  void RestoreDescendants(deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
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
  void UnrollPathsIteratorHelper(string currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
    completed.insert(currLineNumber);
    storePath(currLineNumber, curr);
    descendants.push_front(currLineNumber); // closely related descendants are at the front of the queue
    curr = curr->getParent().lock(); // convert to strong pointer.
    // The above conversion is guaranteed to not be a null pointer because unrollPaths is called by DFS, and DFS keeps the start node as a strong pointer.
    // Hence, all children spawned off, despite having weak pointers to their ancestors, the root ancestor, the start node is kept strong.
    // During unroll paths, the path is unrolled back to the start and then stored, and function returns, and DFS returns, so no pointers are lost.
  }
  void unrollPaths(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths) {
    for (shared_ptr<DFSPathNode> currPath : finishedPaths) {
      bool terminalNode = true;
      deque<string> descendants;
      shared_ptr<DFSPathNode> curr = currPath;
      while (curr != nullptr) {
        string currLineNumber = curr->getLineNumber();
        if (terminalNode) {
          terminalNode = false;
          RestoreDescendants(descendants, curr);
          UnrollPathsIteratorHelper(currLineNumber, descendants, curr);
          continue;
        }
        // Not terminal Node
        // For Next*
        unordered_set<string>& currNextStar = nextStar[currLineNumber];
        unordered_set<string>& prevNextStar = nextStar[descendants[0]];
        currNextStar.insert(prevNextStar.begin(), prevNextStar.end());
        // For Affects
        if (!isStmtType(currLineNumber, ASSIGN)) {
          UnrollPathsIteratorHelper(currLineNumber, descendants, curr);
          continue;
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
              addToAffects(currLineNumber, descLineNumber);
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
        UnrollPathsIteratorHelper(currLineNumber, descendants, curr);
      }

    }
  }
public:
  ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer) {
    unordered_set<string> procNames = dataAccessLayer->getAllProcedures();
    for (string procName : procNames) {
      pair<string, string> procBoundaries = dataAccessLayer->getProcLines(procName);
      stmtNumBoundaries.emplace(procName, procBoundaries);
    }
    next = dataAccessLayer->getClause(NEXT);
    nextStar = next; // initially set nextStar as next
    modifies = dataAccessLayer->getClause(MODIFIES);
    uses = dataAccessLayer->getClause(USES);
    entityMap = {
      {ASSIGN, dataAccessLayer->getEntity(ASSIGN)},
      {IF, dataAccessLayer->getEntity(IF)},
      {WHILE, dataAccessLayer->getEntity(WHILE)},
      {READ, dataAccessLayer->getEntity(READ)},
      {CALL, dataAccessLayer->getEntity(CALL)},
    };
  }
  string procedureNameContainingThis(string lineNumber) {

  }

  bool isInSameProcedure(string lineNumber1, string lineNumber2) {
    return procedureNameContainingThis(lineNumber1) == procedureNameContainingThis(lineNumber2);
  }

  bool isCompleted(string stmtNumber) {
    return completed.find(stmtNumber) != completed.end();
  }

  bool nextStarIntInt(string from, string to) {
    if (!isInSameProcedure(from, to)) {
      return false;
    }
    if (!isCompleted(from)) {
      DFS(from);
    }
    unordered_set<string> fromNextStar = nextStar.at(from);
    return fromNextStar.find(to) != fromNextStar.end();
  }
  unordered_set<string> nextStarSynInt(string to) {

  }
  unordered_set<string> nextStarIntSyn(string from) {
    if (!isCompleted(from)) {
      DFS(from);
    }
    unordered_set<string> fromNextStar = nextStar.at(from);
    return fromNextStar;
  }
  unordered_set<pair<string, string>> nextStarSynSyn() {

  }
  

  // Affects wrapper
  bool affectsIntInt(string from, string to) {


  }
  unordered_set<string> affectsSynInt(string to) {

  }
  unordered_set<string> affectsIntSyn(string from) {

  }
  unordered_set<pair<string, string>> affectsSynSyn() {

  }
};

#endif