#ifndef NOTQUERYOBJECT_H
#define NOTQUERYOBJECT_H

#include "QueryObject.h"

/*
 * This class represents a negated query object
 */
class NotQueryObject : public QueryObject {
 private:
  shared_ptr<QueryObject> originalQueryObject;

  /*
   * Contains the synonym query objects found inside the original query object.
   * Excludes duplicated synonyms. E.g., not Follows(s, s) will only have 1 "s"
   * query object in this vector.
   */
  vector<shared_ptr<QueryObject>> synonyms;

 public:
  NotQueryObject(string_view data, shared_ptr<QueryObject> originalQueryObject,
                 vector<shared_ptr<QueryObject>> synonyms)
      : QueryObject(data),
        originalQueryObject{originalQueryObject},
        synonyms{synonyms} {};

  shared_ptr<QueryResultsTable> callAndProcess(
      shared_ptr<DataAccessLayer> dataAccessLayer) {
    // No synonyms: inverse the bool
    if (getSynonymCount() == 0) {
      auto table = originalQueryObject->callAndProcess(dataAccessLayer);
      table->flipSignificant();
      return table;
    } else if (getSynonymCount() == 1) {
      // 1 synonym: Get full table of the synonym arg, then set diff with the
      // original clause e.g: not p1.procName = "test" -> p1.procName -
      // (p1.procName = "test")

      // auto table = synonyms[0]->callAndProcess(dataAccessLayer);
      // table->setDifference(originalQueryObject->callAndProcess(dataAccessLayer));
      // return table;
      return QueryResultsTable::createEmptyTable();
    }
    // 2 synonyms
    // 2 synonyms: Get full table of both syn args, then set diff with the
    // original clause e.g: not p1.procName = p2.procName -> (p1.procName x
    // p2.procName) - (p1.procName = p2.procName)

    // auto table1 = synonyms[0]->callAndProcess(dataAccessLayer);
    // auto table2 = synonyms[1]->callAndProcess(dataAccessLayer);
    // table1->setCrossProduct(table2);
    // table1->setDifference(originalQueryObject->callAndProcess(dataAccessLayer));
    // return table1;
    return QueryResultsTable::createEmptyTable();
  }

  int getSynonymCount() { return static_cast<int>(synonyms.size()); }
};

#endif