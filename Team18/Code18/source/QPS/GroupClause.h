#ifndef GROUPCLAUSE_H
#define GROUPCLAUSE_H

#include "QueryResultsTable.h"

// This class represents a group of clauses/QueryResultstTables.
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

    int size() {
        return members.size();
    }

    vector<shared_ptr< QueryResultsTable>> getMembers() {
        return members;
    }
    set<string> getHeaders() {
        return headers;
    }
    bool hasSimilarHeaders(shared_ptr < GroupClause > _group) {
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
