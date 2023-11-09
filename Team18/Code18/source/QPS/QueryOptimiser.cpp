#include "QueryOptimiser.h"

// nonSelectClauseQueryObjects {0a, 0b, 1a, 1b, 1c, 1d, 2a, 2b, 2c }

// group nonSelectClauseQueryObjects into groups. group 0 first (no syns), then
// the distinct groups which share syns
// within group 0, put most expensive clauses last (based on abstraction_weights
// map) within each group (not group 0), sort by a heuristic that factors in:
// 1. number of syns in the group 2. type of abstraction (abstraction_weights
// map)

void QueryOptimiser::groupQueryObjects() {
  cout << "QueryObjects" << endl;
  for (shared_ptr<QueryObject> obj : unsortedQueryObjects) {
    cout << "obj: " << obj->getQueryObjectName() << endl;
  }
  // TODO: union-find algorithm
}

void QueryOptimiser::sortGroups() {
  // sort within groups to update groupScores
  sortWithinGroups();

  // for each group, sort by heuristic
  auto groups_comparator = [](const shared_ptr<QueryGroup>& group1,
                              const shared_ptr<QueryGroup>& group2) {
    return group1->getGroupScore() < group2->getGroupScore();
  };
  sort(queryGroups.begin(), queryGroups.end(), groups_comparator);
}

void QueryOptimiser::sortWithinGroups() {
  auto clauses_comparator = [](const shared_ptr<QueryObject>& clause1,
                               const shared_ptr<QueryObject>& clause2) {
    return calculateHeuristic(clause1) < calculateHeuristic(clause2);
  };

  for (shared_ptr<QueryGroup> group : queryGroups) {
    // within each group, sort by heuristic
    vector<shared_ptr<QueryObject>> clauses = group->getClauses();
    sort(clauses.begin(), clauses.end(), clauses_comparator);

    // update groupScores as sum of heuristics of the group
    int groupScore = 0;
    for (shared_ptr<QueryObject> clause : clauses) {
      groupScore += calculateHeuristic(clause);
      group->setGroupScore(groupScore);
    }
  }
}

int QueryOptimiser::calculateHeuristic(shared_ptr<QueryObject> clause) {
    // TODO: waiting for Jan to augment QueryObject info
  /*return abstraction_weights_map[AbstractionStringToEnum(
             clause->getAbstraction())] *
             abstraction_criterion_weight +
         clause->getSynCount() * synonym_count_weight;*/
  return true;
}
