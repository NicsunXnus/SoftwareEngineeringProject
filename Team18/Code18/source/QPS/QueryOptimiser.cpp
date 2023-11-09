#include "QueryOptimiser.h"

// group 0 first (no syns), then the distinct groups which share syns
// within groups, sort by abstraction_weights_map

void QueryOptimiser::groupQueryObjects() {
  cout << "QueryObjects" << endl;
  for (shared_ptr<QueryObject> obj : unsortedQueryObjects) {
    cout << "obj: " << obj->getQueryObjectName() << endl;
  }

  UnionFind unionFind;
  // Make a set for each shared_ptr<QueryObject>
  for (const auto& obj : unsortedQueryObjects) {
    unionFind.makeSet(obj);
  }

  // Union objects based on common synonyms
  for (const auto& obj1 : unsortedQueryObjects) {
    for (const auto& obj2 : unsortedQueryObjects) {
      // TODO: uncomment this!
      // if (obj1->getSynonyms().count(*obj2->getSynonyms().begin()) > 0) {
      // unionFind.unite(obj1, obj2);
      // }
    }
  }

  vector<vector<shared_ptr<QueryObject>>> new_groups = unionFind.getGroups();

  // update queryGroups and numGroups
  createQueryGroups(new_groups);

  for (const auto& group : new_groups) {
    cout << "Group:";
    for (const auto& obj : group) {
      cout << " " << obj->getQueryObjectName();
    }
    cout << endl;
  }
}

void QueryOptimiser::sortGroups() {
  // sort within groups to update groupScores
  sortWithinGroups();

  // for each group, sort by heuristic
  auto groups_comparator = [](const shared_ptr<QueryGroup>& group1,
                              const shared_ptr<QueryGroup>& group2) {
    // this check ensures that group 0 (with no syns) is always first
    // TODO: uncomment this!
    // if (group1->getClauses()[0]->getSynonyms().size() == 0) {
    // return true;
    // } else if (group2->getClauses()[0]->getSynonyms().size() == 0) {
    // return false;
    // } else {
    return group1->getGroupScore() < group2->getGroupScore();
    // }
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
  // within each group (not group 0), sort by a heuristic that factors in:
  // 1. number of syns in group 2. type of abstraction (abstraction_weights_map)

  // TODO: waiting for Jan to augment QueryObject info
  /*return abstraction_weights_map[AbstractionStringToEnum(
             clause->getAbstraction())] *
             abstraction_criterion_weight +
         clause->getSynCount() * synonym_count_weight;*/
  return true;
}
