#ifndef GROUPCLAUSE_H
#define GROUPCLAUSE_H

#include "QueryResultsTable.h"
// This class represents a group of clauses/QueryResultsTables.
class GroupClause {
public:
    GroupClause() {}

    void addOne(shared_ptr< QueryResultsTable> table) {
        members.emplace_back(table);
        updateHeaders(table->getHeaders());
    }

    void addMany(vector<shared_ptr< QueryResultsTable>> tables) {
        for (shared_ptr< QueryResultsTable> table : tables)
            addOne(table);
    }

    void merge(shared_ptr < GroupClause > _group) {
        addMany(_group->getMembers());
    }

    void setMembers(vector<shared_ptr< QueryResultsTable>> tables) {
        members.clear();
        addMany(tables);
    }

    void reduceToOne() {
        if (size() == 0) return;
        shared_ptr<QueryResultsTable> intermediateTable = members[0];
        members.erase(members.begin());
        shared_ptr<QueryResultsTable> currTable;
        for (shared_ptr<QueryResultsTable> table : members) {
            currTable = table;
            // for empty but significant tables
            if (currTable->isEmpty()) {
                if (currTable->getSignificant()) {
                    continue; // just keep current table
                }
                else { // no need to evaluate the rest of the query  
                    members = { QueryResultsTable::createEmptyTable() };
                    return;
                }
            }
            if (intermediateTable->haveSimilarHeaders(currTable)) {
                //do inner join
                intermediateTable = intermediateTable->innerJoin(currTable);
            }
            else {
                //do cross product
                intermediateTable = intermediateTable->crossProduct(currTable);
            }
        }
        members = { intermediateTable };
    }

    int size() {
        return members.size();
    }

    vector<shared_ptr< QueryResultsTable>> getMembers() {
        return members;
    }
    set<string> getHeaders() {
        return headers;
    }
    bool hasCommonHeaders(shared_ptr < GroupClause > _group) {
        set<string> otherGroupHeaders = _group->getHeaders();
        set<string> intersect;
        set_intersection(otherGroupHeaders.begin(), otherGroupHeaders.end(), headers.begin(), headers.end(), inserter(intersect, intersect.begin()));
        return intersect.size() > 0;
    }

    bool hasEmptyTablesFirst() {
        return !members.empty() && members[0]->isEmpty();
    }
private: 
    void updateHeaders(vector<string> _headers) {
        set_union(headers.cbegin(), headers.cend(), _headers.cbegin(), _headers.cend(), inserter(headers, headers.begin()));
    }
    vector<shared_ptr< QueryResultsTable>> members;
    set<string> headers;
};
#endif
