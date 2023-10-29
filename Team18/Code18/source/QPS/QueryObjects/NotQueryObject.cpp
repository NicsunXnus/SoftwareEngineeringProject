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
    auto all_syn_table = syn->callAndProcess(dataAccessLayer);
    table->difference(all_syn_table);
    return table;
  }
  // 2 synonyms
  // Get full table of both syn args, then set diff with the
  // original clause e.g: not p1.procName = p2.procName -> (p1.procName x
  // p2.procName) - (p1.procName = p2.procName)

  // auto table1 = synonyms[0]->callAndProcess(dataAccessLayer);
  // auto table2 = synonyms[1]->callAndProcess(dataAccessLayer);
  // table1->setCrossProduct(table2);
  // table1->setDifference(originalQueryObject->callAndProcess(dataAccessLayer));
  // return table1;
  return QueryResultsTable::createEmptyTable();
}