#include "NotQueryObject.h"

shared_ptr<QueryResultsTable> NotQueryObject::callAndProcess(
    shared_ptr<DataAccessLayer> dataAccessLayer) {
  // Get the original table
  auto table = originalQueryObject->callAndProcess(dataAccessLayer);
  int syn_count = getSynonymCount();

  if (syn_count == 0) {  // No synonyms: inverse the bool
    table->flipSignificant();
    return table;

  } else if (syn_count == 1) {  // 1 synonym
    // Get full table of synonym arg, then set diff with original clause
    // e.g: not p1.procName = "test" -> p1.procName - (p1.procName = "test")
    shared_ptr<QueryObject> syn = getFirstSynonym();
    auto syn_table = syn->callAndProcess(dataAccessLayer);
    return table->difference(syn_table);

  } else {  // 2 synonyms
    // Get full table of both syn args, then set diff with the
    // original clause e.g: not p1.procName = p2.procName -> (p1.procName x
    // p2.procName) - (p1.procName = p2.procName)
    shared_ptr<QueryObject> syn1 = getFirstSynonym();
    shared_ptr<QueryObject> syn2 = getSecondSynonym();
    auto syn1_table = syn1->callAndProcess(dataAccessLayer);
    auto syn2_table = syn2->callAndProcess(dataAccessLayer);
    return table->difference(syn1_table, syn2_table);
  }
}