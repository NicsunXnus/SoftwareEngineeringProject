#ifndef CFG_H
#define CFG_H

#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "../Constants/DesignEnums.h"
#include "DataAccessLayer.h"

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
  vector<unordered_set<string>> nextStar;
  StringMap nextStar; // <Line Number, Next*>

public:
  // 1. Take out Next via DataAccessLayer::getClause(NEXT) -> map<string, unordered_set<string>>
  // 2a. Take out all procedures using DataAccessLayer::getAllProcedures() -> unordered_set<string>
  // 2b. For each procedure in 2a, do DataAccessLayer::getProcLines(string procName) -> pair<string, string> to get start/end lines of procedures
  // 3. Dump out (1) into a new data structure == nextStar[], array of nextStar, each element belongs to one procedure
  ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer);

  // Usage:
  // NextStar wrapper
  // Next*(1, x) is true for all x > 1
  bool isReachable(string from, string to);

  // Affects wrapper
  // 1. Checks if node
  bool isAffecting(string from, string to);

  void inversedDFS() {
    // Case 1: no branched CFG, 1 node per procedure, easy
    // Case 2: if-branch CFG, multiple node per procedure, still ok as node.next is blank
    // Case 3: while-branch CFG, potentially 0 nodes per procedure. In which case, take last statement in procedure.
    // Case 4: if+while branch CFG, potentially multiple 0 nodes per procedure, that have multiple "last statements".
    //          Case 4 cannot be deterministically solved  
    // Initialise
    startNodes = all nodes in NextAdjList that (Next(s, end) is true)
    for stmt in stmts:
      stmt.nextStar.add(stmt.children)
    for ifStmt in filtered_for_if(stmts):
      ifStmt.childrenLeft = ifStmt.children
    for whileStmt in filtered_for_while(stmts):
      whileStmt.parentsLeft = whileStmt.parents

    // for loop to handle the different starts from 
    // terminal nodes
    for startNode in startNodes:
      incompleteIf = false
      curr = NONE
      to_traverse = stack<>{startNode}
      while to_traverse is not empty:
        curr = to_traverse.pop()
        if curr.isDone
          continue
        for parent in curr.parents:
          to_traverse.push(parent)
          parent.nextStar.add(curr.nextStar)
          if parent is ifStmt:
            parent.childrenLeft.remove(curr)

        if curr is whileStmt:
          if curr.parentsLeft.size == 2 :
            bottomParent = curr.parents[1]
            topChild = curr.children[0]
            for stmt in range(bottomParent, topChild) :
              stmt.isDone = false // reset isDone because encountered whileStmt
            curr.parentsLeft.remove(bottomParent)
            // force push to top of stack so traverse loop body first
            to_traverse.push(curr.bottomParent) 
            continue // don't set current as done yet
        if curr is ifStmt :
          if curr.childrenLeft is not empty:
            incompleteIf = true
        else if not incompleteIf:
          curr.isDone = true
  }
};

#endif