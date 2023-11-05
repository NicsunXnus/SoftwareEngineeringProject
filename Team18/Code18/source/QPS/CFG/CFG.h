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
#include "CFGNode.h"

using namespace std;

class ExtendedCFG {
  // Idea:
  // Constructor that takes in whatever PKB produces for Next
  // Keeps an internal structure similar to that
  // Keeps track of Next*
  // Keeps track of which statements uses and modifies

  // Forward Traversal once only
  // Implement memoisation
  // Access through: shared_ptr<DataAccessLayer> dataAccessLayer
  // call: StringMap DataAccessLayer::getClause(ABSTRACTION abstraction), use NEXT as defined in DesignEnums
  // StringMap == map<string, unordered_set<string>>
private:
  set<pair<string, string>> stmtNumBoundaries; // boundaries of each procedure in terms of statement numbers
  StringMap next; // children nodes
  StringMap inverseNext; // parent nodes

  StringMap nextStar; // <Line Number, Next*>, internally map<string, unordered_set<string>>
  StringMap modifies; // variables that this statement number modifies
  StringMap uses; // variables that this statement number uses
  unordered_set<string> assigns; // assignment statements
  unordered_set<string> whiles; // while statements
  unordered_set<string> ifs;
  unordered_set<string> completed; // Stores line numbers of statements that have been evaluated completely
  StringMap modifiesAfter; // 
  StringMap affects; // Affects(s, collection<s>) is True
  map<ENTITY, unordered_set<string>> entityMap;

public:
  // 1. Take out Next via DataAccessLayer::getClause(NEXT) -> map<string, unordered_set<string>>
  // 2a. Take out all procedures using DataAccessLayer::getAllProcedures() -> unordered_set<string>
  // 2b. For each procedure in 2a, do DataAccessLayer::getProcLines(string procName) -> pair<string, string> to get start/end lines of procedures
  // 3. Dump out (1) into a new data structure == nextStar[], array of nextStar, each element belongs to one procedure
  ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer) {
    unordered_set<string> procNames = dataAccessLayer->getAllProcedures();
    for (string procName : procNames) {
      pair<string, string> procBoundaries = dataAccessLayer->getProcLines(procName);
      stmtNumBoundaries.insert(procBoundaries);
    }
    next = dataAccessLayer->getClause(NEXT);
    inverseNext = dataAccessLayer->getClauseInverse(NEXT);
    nextStar = next; // initially set nextStar as next
    modifies = dataAccessLayer->getClause(MODIFIES);
    uses = dataAccessLayer->getClause(USES);
    assigns = dataAccessLayer->getEntity(ASSIGN);
    whiles = dataAccessLayer->getEntity(WHILE);
    ifs = dataAccessLayer->getEntity(IF);
    entityMap = {
      {ASSIGN, assigns},
      {IF, ifs},
      {WHILE, whiles},
      {READ, dataAccessLayer->getEntity(READ)},
      {CALL, dataAccessLayer->getEntity(CALL)},
    };
  }

  // Usage:
  // NextStar wrapper
  // Next*(1, x) is true for all x > 1
  bool isReachable(string from, string to);

  // Affects wrapper
  // 1. Checks if node
  bool isAffecting(string from, string to);

  void DFS(string start) {
    stack<shared_ptr<DFSPathNode>> incompletePaths;
    unordered_set<shared_ptr<DFSPathNode>> finishedPaths;
    incompletePaths.push(make_shared<DFSPathNode>(start));
    shared_ptr<DFSPathNode> curr = nullptr;
    unordered_map<string, int> whileVisitCount;
    for (string whileStmt : whiles) {
      whileVisitCount.emplace(whileStmt, 0);
    }
    while (!incompletePaths.empty()) {
      curr = incompletePaths.top(); incompletePaths.pop();
      string currLineNumber = curr->getLineNumber();
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

  void UnrollPathsIteratorHelper(string currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr) {
    completed.insert(currLineNumber);
    descendants.push_front(currLineNumber); // closely related descendants are at the front of the queue
    curr = curr->getParent();
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

  //void inversedDFS() {
  //  // Case 1: no branched CFG, 1 node per procedure, easy
  //  // Case 2: if-branch CFG, multiple node per procedure, still ok as node.next is blank
  //  // Case 3: while-branch CFG, potentially 0 nodes per procedure. In which case, take last statement in procedure.
  //  // Case 4: if+while branch CFG, potentially multiple 0 nodes per procedure, that have multiple "last statements".
  //  //          Case 4 cannot be deterministically solved  
  //  // Initialise
  //  startNodes = all nodes in NextAdjList that (Next(s, end) is true)
  //  for stmt in stmts:
  //    stmt.nextStar.add(stmt.children)
  //  for ifStmt in filtered_for_if(stmts):
  //    ifStmt.childrenLeft = ifStmt.children
  //  for whileStmt in filtered_for_while(stmts):
  //    whileStmt.parentsLeft = whileStmt.parents

  //  // for loop to handle the different starts from 
  //  // terminal nodes
  //  for startNode in startNodes:
  //    incompleteIf = false
  //    curr = NONE
  //    to_traverse = stack<>{startNode}
  //    while to_traverse is not empty:
  //      curr = to_traverse.pop()
  //      if curr.isDone
  //        continue
  //      for parent in curr.parents:
  //        to_traverse.push(parent)
  //        parent.nextStar.add(curr.nextStar)
  //        if parent is ifStmt:
  //          parent.childrenLeft.remove(curr)

  //      if curr is whileStmt:
  //        if curr.parentsLeft.size == 2 :
  //          bottomParent = curr.parents[1]
  //          topChild = curr.children[0]
  //          for stmt in range(bottomParent, topChild) :
  //            stmt.isDone = false // reset isDone because encountered whileStmt
  //          curr.parentsLeft.remove(bottomParent)
  //          // force push to top of stack so traverse loop body first
  //          to_traverse.push(curr.bottomParent) 
  //          continue // don't set current as done yet
  //      if curr is ifStmt :
  //        if curr.childrenLeft is not empty:
  //          incompleteIf = true
  //      else if not incompleteIf:
  //        curr.isDone = true
  //}
};

#endif