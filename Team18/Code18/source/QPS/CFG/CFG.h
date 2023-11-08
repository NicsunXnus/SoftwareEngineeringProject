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

class DataAccessLayer;

/// <summary>
/// This class represents an extended CFG to include not just Next relations, but also Next* relations and Affects Relations.
/// 
/// The way the DFS is implemented allows for partial evaluation of procedures. For example:
/// in the query with clauses:
/// ... Affects(4, s) Affects(2, s) ...
/// The (2,s) will use the results previously calculated from (4,s).
/// </summary>
class ExtendedCFG {
private:
  map<string, pair<string, string>> stmtNumBoundaries; // boundaries of each procedure in terms of statement numbers
  unordered_set<string> procNames;
  StringMap next; // children nodes
  StringMap modifies; // variables that this statement number modifies
  StringMap uses; // variables that this statement number uses
  map<ENTITY, unordered_set<string>> statementEntities;

  StringMap nextStar; // <Line Number, Next*>, internally map<string, unordered_set<string>>
  StringMap nextStarInverse;
  StringMap affects; // Affects(s, collection<other_s>) is True
  StringMap affectsInverse; // AffectedBy(s, other_s) is True; 

  unordered_set<string> nextStarCompleted; // Stores line numbers of statements that have been evaluated completely for next*
  unordered_set<string> affectsCompleted; // Stores line numbers of statements that have been evaluated completely for affects
  unordered_map<string, unordered_set<shared_ptr<DFSPathNode>>> frozenPaths; // line number : set<frozen paths>

  /// GROUP: MISC HELPER FUNCTIONS FOR QUERY METHODS

  // Checks if a given line number is valid or not (ie within the bounds of the program)
  bool isValidLineNumber(string lineNumber);

  // Finds the procedure that contains this line number
  // returns "-1" if does not exist
  string procedureNameContainingThis(string lineNumber);

  // Checks if two lines are in the same procedure
  bool isInSameProcedure(string lineNumber1, string lineNumber2);

  // Gets the first line number of a procedure. Guaranteed to exist from SPA grammar rules
  string getFirstLineNumberOfProc(string procName);

  // Checks if a certain line number has been evaluated for next*
  bool isNextStarCompleted(string stmtNumber);

  // Checks if a certain line number has been evaluated for affects
  bool isAffectsCompleted(string stmtNumber);

  // Checks if a certain statement number has been evaluated
  bool isCompleted(string stmtNumber);

  // Ensures that a certain statement number has been evaluated, by performing DFS
  // from that line number if it has not been evaluated.
  // Returns whether the line number is valid or not.
  bool ensureCompleted(string lineNumber, bool calculateAffects);

  // Ensures that the given procedure has been evaluated, by performing DFS
  // on the first line number of the procedure.
  void ensureProcCompleted(string procName, bool calculateAffects);

  // Checks if a given statement has affects
  bool hasAffects(string from);

  // Checks if a given statement is destination of any Affects
  bool hasAffectsInv(string to);

  // Checks if a given statement is destination of any Next*
  bool hasNextStarInv(string to);

  // Next*(int, int) and Affects(int, int) helper
  // Returns false if either from/to are invalid line numbers
  bool intIntHelper(string from, string to, bool calculateAffects);

  // Next*(syn, int) and Affects(syn, int) helper
  // Returns whether the line number is a valid line number or not
  bool synIntHelper(string lineNumber, bool calculateAffects);

  // Checks if a given statement number is of a given statement type
  bool isStmtType(string stmtNumber, ENTITY entity);

  // Checks if a given statement number has a next node
  bool hasNext(string stmtNumber);

  // Gets the variables that are used by this statement number
  unordered_set<string> getUsedVariables(string stmtNumber);

  /// GROUP: Add to abstraction methods

  // Adds Next* relations for a given line number and one other line number
  // Next*(lineNumber, toAdd)
  void addNextStar(string lineNumber, string toAdd);

  // Adds Next* inverse relations for a given line number and one other line number
  // Next*(lineNumber, toAdd)
  void addNextStarInv(string lineNumber, string toAdd);

  // Adds Affects relations for a given line number and one other line number
  // Also adds the Inverse Affects relations
  void addAffects(string lineNumber, string toAdd);

  // GROUP: DFS Functions
  // Helper function to add to incomplete paths, while adding to next* inverse too
  void DFSIncompletePathsHelper(stack<shared_ptr<DFSPathNode>>&incompletePaths, shared_ptr<DFSPathNode> toAdd, string currLineNum);

  // Helper function to consider the case when encountering an IF statement
  void DFSIfHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes);

  // Helper function to consider the case when encountering an WHILE statement.
  // Cuts short if the while node is terminal, and visit count == 3, to allow the DFS to stop.
  void DFSWhileHelper(stack<shared_ptr<DFSPathNode>>& incompletePaths,
    unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr, unordered_set<string>& nextNodes, string& currLineNumber);

  // Helper function to consider the case when encountering a statement that has been evaluated before.
  void DFSEncounteredCacheHelper(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, shared_ptr<DFSPathNode>& curr);

  /// <summary>
  /// Main DFS Traversal Method.
  /// 
  /// Traverses from a given start node until all terminal nodes. All paths are individually tracked with
  /// the usage of DFSPathNodes, and are passed to unrollPaths when the DFS has no more paths to explore.
  /// </summary>
  /// <param name="start">The start statement number</param>
  void DFS(string start, bool calculateAffects);

  // GROUP: Unroll functions

  // Caches a given Path at a given line number
  void storePath(string lineNumber, shared_ptr<DFSPathNode> curr);

  // Restores the descendants deque when given the current node, using cached paths.
  void restoreDescendants(deque<string>& descendants, shared_ptr<DFSPathNode> curr);

  // Used to move the curr pointer to the parent. This also indicates that the current iteration is over
  // and hence, mark the current node as done, and cache information, as well as to add to the descendants deque.
  void unrollPathsIteratorHelper(string currLineNumber, deque<string>& descendants, shared_ptr<DFSPathNode>& curr);

  // Helper method to consider the functionality for a Next* relationship
  void unrollNextStarHelper(string currLineNumber, deque<string>& descendants);

  // Helper method to consider the functionality for an Affects relationship.
  // Should be called within each iteration of a path unrolling.
  // Cuts short if the given statement is not an assignment statement.
  void unrollAffectsHelper(deque<string>& descendants, shared_ptr<DFSPathNode>& curr, string currLineNumber);

  /// <summary>
  /// Unrolls the paths given by DFS. 
  /// Not unlike how to find the path from a source to target using regular DFS, an unrolling is performed.
  /// 
  /// During the unrolling, Next* and Affects relations are calculated and cached.
  /// </summary>
  /// <param name="finishedPaths">The Paths taken by the DFS to reach terminal nodes or cached nodes</param>
  void unrollPaths(unordered_set<shared_ptr<DFSPathNode>>& finishedPaths, bool calculateAffects);

public:
  /// <summary>
  /// Constructor to take in the data access layer to extract out information required for
  /// the processing of Next* and Affects.
  /// </summary>
  /// <param name="dataAccessLayer">The standard QPS data access layer</param>
  ExtendedCFG(shared_ptr<DataAccessLayer> dataAccessLayer);
  
  // NEXT STAR  
  /// <summary>
  /// Returns whether Next*(from, to) is true or not.
  /// </summary>
  /// <param name="from">statement number of the 1st arg</param>
  /// <param name="to">statement number of the 2nd arg</param>
  /// <returns>Whether the relation is true or not</returns>
  bool nextStarIntInt(string from, string to);

  /// <summary>
  /// Returns all statements s that satisfies Next*(s, to).
  /// </summary>
  /// <param name="to">statement number of the 2nd arg</param>
  /// <returns>All statement s that satisfies the above relation.</returns>
  unordered_set<string> nextStarSynInt(string to);

  /// <summary>
  /// Returns all statements s that satisfies Next*(from, s).
  /// </summary>
  /// <param name="from">statement number of the 1st arg</param>
  /// <returns>All statement s that satisfies the above relation.</returns>
  unordered_set<string> nextStarIntSyn(string from);

  /// <summary>
  /// Returns all statements s and s' such that Next*(s, s').
  /// </summary>
  /// <returns>all statements that satisfy the above relation, in StringMap format.</returns>
  StringMap nextStarSynSyn();

  // AFFECTS
  /// <summary>
  /// Returns whether Affects(from, to) is true or not.
  /// </summary>
  /// <param name="from">statement number of the 1st arg</param>
  /// <param name="to">statement number of the 2nd arg</param>
  /// <returns>Whether the relation is true or not</returns>
  bool affectsIntInt(string from, string to);

  /// <summary>
  /// Returns all assignment stmts a that satisfies Next*(a, to).
  /// </summary>
  /// <param name="to">statement number of the 2nd arg</param>
  /// <returns>All statements a that satisfies the above relation.</returns>
  unordered_set<string> affectsSynInt(string to);

  /// <summary>
  /// Returns all assignment stmts a that satisfies Affects(from, a).
  /// </summary>
  /// <param name="from">statement number of the 1st arg</param>
  /// <returns>All statements a that satisfies the above relation.</returns>
  unordered_set<string> affectsIntSyn(string from);

  /// <summary>
  /// Returns all assignment stmts a and a' such that Affects(a, a').
  /// </summary>
  /// <returns>all statements that satisfy the above relation, in StringMap format.</returns>
  StringMap affectsSynSyn();
};

#endif