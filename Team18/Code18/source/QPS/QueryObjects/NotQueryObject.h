#ifndef NOTQUERYOBJECT_H
#define NOTQUERYOBJECT_H

#include "QueryObject.h"
#include <string_view>

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
      shared_ptr<DataAccessLayer> dataAccessLayer) override;

  int getSynonymCount() { return static_cast<int>(synonyms.size()); }

  bool isPattern() { return getQueryObjectName().substr(0, 7) == "pattern"; }

  shared_ptr<QueryObject> getFirstSynonym() { return synonyms[0]; }

  shared_ptr<QueryObject> getSecondSynonym() { return synonyms[1]; }
};

#endif