#ifndef UNIONFIND_H
#define UNIONFIND_H

#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "QueryObjects/QueryObject.h"

using namespace std;

class UnionFind {
 private:
  unordered_map<shared_ptr<QueryObject>, shared_ptr<QueryObject>>
      leader;
  unordered_map<shared_ptr<QueryObject>, unordered_set<shared_ptr<QueryObject>>>
      sets;

  const shared_ptr<QueryObject> findLeader(const shared_ptr<QueryObject> obj) {
    shared_ptr<QueryObject> current = obj;
    while (leader[current] != current) {
      current = leader[current];
    }
    return current;
  }

 public:
  void makeSet(const shared_ptr<QueryObject>& obj) {
    sets[obj] = {obj};
    leader[obj] = obj;
  }

  const shared_ptr<QueryObject> find(
      const shared_ptr<QueryObject>& obj) {
    return leader[findLeader(obj)];
  }

  void unite(const shared_ptr<QueryObject>& obj1,
             const shared_ptr<QueryObject>& obj2) {
    const shared_ptr<QueryObject> leader1 = findLeader(obj1);
    const shared_ptr<QueryObject> leader2 = findLeader(obj2);

    if (leader1 != leader2) {
      sets[leader1].insert(
          sets[leader2].begin(),
          sets[leader2].end());
      for (const auto& obj : sets[leader2]) {
        leader[obj] = leader1;
      }
      sets.erase(leader2);
    }
  }

  vector<vector<shared_ptr<QueryObject>>> getGroups() const {
    vector<vector<shared_ptr<QueryObject>>> groups;
    for (const auto& entry : sets) {
      groups.push_back({entry.second.begin(), entry.second.end()});
    }
    return groups;
  }
};

#endif