//#include "GroupClause.h"
//
//void GroupClause::addOne(shared_ptr< QueryResultsTable> table) {
//    members.emplace_back(table);
//    updateHeaders(table->getVectorizedRows()[0]);
//}
//
//void GroupClause::addMany(vector<shared_ptr< QueryResultsTable>> tables) {
//    for (shared_ptr< QueryResultsTable> table : tables)
//        addOne(table);
//}
//
//bool GroupClause::hasCommonHeaders(shared_ptr < GroupClause > _group) {
//    set<string> otherGroupHeaders = _group->getHeaders();
//    set<string> intersect;
//    set_intersection(otherGroupHeaders.begin(), otherGroupHeaders.end(), headers.begin(), headers.end(), inserter(intersect, intersect.begin()));
//    return intersect.size() > 0;
//}
//
//vector<shared_ptr< QueryResultsTable>> GroupClause::getMembers() {
//    return members;
//}
//
//set<string> GroupClause::getHeaders() {
//    return headers;
//}
//
//bool GroupClause::hasEmptyTablesFirst() {
//    return !members.empty() && members[0]->isEmpty();
//}
//
//void GroupClause::merge(shared_ptr < GroupClause > _group) {
//    addMany(_group->getMembers());
//}
//
//void GroupClause::reduceToOne() {
//    if (size() == 0) return;
//    shared_ptr<QueryResultsTable> intermediateTable = members[0];
//    members.erase(members.begin());
//    shared_ptr<QueryResultsTable> currTable;
//    for (shared_ptr<QueryResultsTable> table : members) {
//        currTable = table;
//        // for empty but significant tables
//        if (currTable->isEmpty()) {
//            if (currTable->getSignificant()) {
//                continue; // just keep current table
//            }
//            else { // no need to evaluate the rest of the query  
//                members = { QueryResultsTable::createEmptyTable() };
//                return;
//            }
//        }
//        if (intermediateTable->haveSimilarHeaders(currTable)) {
//            //do inner join
//            intermediateTable = intermediateTable->innerJoin(currTable);
//        }
//        else {
//            //do cross product
//            intermediateTable = intermediateTable->crossProduct(currTable);
//        }
//    }
//    members = { intermediateTable };
//}
//
//void GroupClause::setMembers(vector<shared_ptr< QueryResultsTable>> tables) {
//    members.clear();
//    addMany(tables);
//}
//
//int GroupClause::size() {
//    return members.size();
//}
//
//void GroupClause::updateHeaders(vector<string> _headers) {
//    sort(_headers.begin(), _headers.end());
//    set_union(headers.cbegin(), headers.cend(), _headers.cbegin(), _headers.cend(), inserter(headers, headers.begin()));
//
//}
