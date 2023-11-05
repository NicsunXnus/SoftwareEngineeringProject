#ifndef CFGNODE_H
#define CFGNODE_H

#include <unordered_set>
#include <string>
#include <memory>

using namespace std;

class DFSPathNode {
private:
  shared_ptr<DFSPathNode> parent = nullptr;
  string lineNumber;
  map<string, int> whileVisitCount;
public:
  DFSPathNode(string lineNumber) : lineNumber{ lineNumber } {}
  DFSPathNode(string lineNumber, shared_ptr<DFSPathNode> parent, map<string, int> whileVisitCount) 
    : lineNumber{ lineNumber }, parent{ parent }, whileVisitCount{ whileVisitCount } {};

  shared_ptr<DFSPathNode> getParent() {
    return this->parent;
  }
  string getLineNumber() {
    return this->lineNumber;
  }
  int getWhileVisitCount(string stmtNumber) {
    if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
      return 0;
    }
    return this->whileVisitCount.at(stmtNumber);
  }
  void incrementWhileVisitCount(string stmtNumber) {
    if (whileVisitCount.find(stmtNumber) == whileVisitCount.end()) {
      whileVisitCount.emplace(stmtNumber, 0);
    }
    else {
      whileVisitCount[stmtNumber]++;
    }
  }
  static shared_ptr<DFSPathNode> traverse(shared_ptr<DFSPathNode> current, string nextLineNumber) {
    return make_shared<DFSPathNode>(nextLineNumber, current, current->whileVisitCount);
  }
  static unordered_set<shared_ptr<DFSPathNode>> diverge(shared_ptr<DFSPathNode> current, unordered_set<string> lineNumbers) {
    unordered_set<shared_ptr<DFSPathNode>> output;
    for (string lineNumber : lineNumbers) {
      output.insert(traverse(current, lineNumber));
    }
    return output;
  }

  
  
  /*
  Affects(s1, s2) holds if:
    - s1 and s2 are assignment statements,
    - s1 modifies a variable v which is used in s2 and
    - there is a control flow path from s1 to s2 on such that v is not modified
      in any assignment, read, or procedure call statement on that path
  
  To check for Affects:
  1. s1 modifies variable v (in LHS of assignment)
  2. s2 uses variable v (in RHS of assignment)
  3. s2 is within s1's Next*
  4. On backwards traversal to update Next*:

  endNodes = terminal nodes OR destination node
  for endNode in endNodes:
    ordered_collection<string> S = {} // statement numbers in upwards traversal.
    curr = endNode
    while curr is not start:
      if curr is assignment:
        encounteredModifies = False // init as false
        v = Modifies(curr, v)[0] // should only modify one var
        for s' in S:
          if |S| > 1 and v == Modifies(s', _)[0]:
            // meaning there exists statements in between that modifies
            encounteredModifies = True
          if v in Uses(s', _) and not encounteredModifies:
            Affects(curr, s') = True
      S.add_to_back(curr)
      curr = curr.parent // assume no if/while first

    a. ordered_collection<string> S = statement numbers in the upwards
       traversal. Initially empty. As nodes are traversed, curr is added to S
    b. Current statement = s.
    c. if s is assignment:
      i. for s' in S:
        
  4. after DFS down to terminal node, and traversing back up to update Next*
    a. keep track of statement numbers involved in this upwards traverse
       let this be set S
    b. for each statement s, if assignment statement a:
        i. Check if v in modify(a, v) is inside uses(s', v)
       ii. if no, ignore
      iii. if yes, check for all s' in S that there does not exist another
           modify(s', v).
       iv. if does not exist, add 
  
  */
  
};

#endif