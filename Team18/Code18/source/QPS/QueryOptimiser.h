#ifndef QUERYOPTIMISER_H
#define QUERYOPTIMISER_H

#include <vector>

#include "../Constants/DesignEnums.h"
#include "../Constants/QPSConstants.h"
#include "QueryObjects/QueryObject.h"

/**
 * This class groups and sorts queries based on different criteria.
 */
using namespace std;

class QueryGroup {
 private:
  int groupScore;
  vector<shared_ptr<QueryObject>> clauses;

 public:
  /**
   * Constructor for the QueryGroup object
   */
  QueryGroup(int groupScore, vector<shared_ptr<QueryObject>> clauses)
      : groupScore(groupScore), clauses(clauses) {}

  /**
   * Destructor for the QueryGroup object
   */
  ~QueryGroup(){};

  vector<shared_ptr<QueryObject>> getClauses() { return clauses; }

  int getGroupScore() { return groupScore; }

  void setGroupScore(int groupScore) { this->groupScore = groupScore; }
};

class QueryOptimiser {
 private:
  vector<shared_ptr<QueryObject>> unsortedQueryObjects;
  int numGroups = 0;
  vector<shared_ptr<QueryGroup>> queryGroups;

 public:
  /**
   * Constructor for the QueryOptimiser object
   */
  QueryOptimiser(vector<shared_ptr<QueryObject>> unsortedQueryObjects)
      : unsortedQueryObjects(unsortedQueryObjects) {}

  /**
   * Destructor for the QueryOptimiser object
   */
  ~QueryOptimiser(){};

  /**
   * Group QueryObjects based on criteria considering their synonyms, and sets
   * the groups field. Group 0 having no synonyms, and each subsequent group
   * having shared synonyms.
   * This is done by an adaptation of union-find algorithm.
   *
   * Sorting of groups will be done outside of this method.
   * Updates numGroups field too.
   */
  void groupQueryObjects();

  /**
   * Sorts groups based on sum of heuristic.
   * Calls sortWithinGroups() to sort within each group first.
   * Will update the groups field.
   */
  void sortGroups();

  /**
   * Helper function called by sortGroups() to sorts the clauses in a group,
   * based on heuristic.
   * Returns a vector of sorted clauses for the group,
   * and updates the groupScores field.
   */
  void sortWithinGroups();

  /**
   * Returns the groups, to be called after grouping and sorting.
   */
  vector<shared_ptr<QueryGroup>> getQueryGroups() { return queryGroups; };

  /**
   * Calculates the heuristic score for a clause.
   */
  static int calculateHeuristic(shared_ptr<QueryObject> clause);
};

#endif