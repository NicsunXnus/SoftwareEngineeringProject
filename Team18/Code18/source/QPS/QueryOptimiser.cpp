#include "QueryOptimiser.h"

// group 0 first (no syns), then the distinct groups which share syns
// within groups, sort by abstraction_weights_map

void QueryOptimiser::groupQueryObjects() {

  UnionFind unionFind;
  // Make a set for each shared_ptr<QueryObject>
  for (const auto& obj : unsortedQueryObjects) {
    unionFind.makeSet(obj);
  }

  // Union objects based on common synonyms
  for (const auto& obj1 : unsortedQueryObjects) {
    for (const auto& obj2 : unsortedQueryObjects) {
      if (obj1 == obj2) continue;
      if (obj1->getSynCount() == 0 || obj2->getSynCount() == 0) continue;
      shared_ptr<unordered_set<string>> obj1Syns = obj1->getSynonyms();
      shared_ptr<unordered_set<string>> obj2Syns = obj2->getSynonyms();

      for (const auto& syn : *obj2Syns) {
        if (obj1Syns->count(syn)) {
          unionFind.unite(obj1, obj2);
        }
      }
    }
  }

  vector<vector<shared_ptr<QueryObject>>> new_groups = unionFind.getGroups();

  // update queryGroups and numGroups
  createQueryGroups(new_groups);
}

void QueryOptimiser::sortGroups() {
  // sort within groups to update groupScores
  sortWithinGroups();

  // for each group, sort by heuristic
  auto groups_comparator = [](const shared_ptr<QueryGroup>& group1,
                              const shared_ptr<QueryGroup>& group2) {
    // this check ensures that group 0 (with no syns) is always first
    if (group1->getClauses()[0]->getSynCount() == 0) {
      return true;
    } else if (group1->getClauses()[0]->getSynCount() == 0) {
      return false;
    } else {
      return group1->getGroupScore() < group2->getGroupScore();
    }
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

  int clauseScore = 0;

  string query_obj_string = svToString(clause->getQueryObjectName());
  if (query_obj_string.substr(0, 3) == "not") {
    clauseScore += NOT_CLAUSE_SCORE;
    query_obj_string = query_obj_string.substr(3);
  }

  if (query_obj_string == "Compare") {
    clauseScore += WITH_CLAUSE_SCORE;
  } else if (query_obj_string.substr(0, 7) == "pattern") {
    clauseScore += PATTERN_CLAUSE_SCORE;
  } else {
    ABSTRACTION abstraction = QueryObjStringToEnum(query_obj_string);
    clauseScore += abstraction_weights_map.at(abstraction);
  }
  int synCountScore = clause->getSynCount() * SYNONYM_COUNT_WEIGHT;
  return clauseScore + synCountScore;
}
