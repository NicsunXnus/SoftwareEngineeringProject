#ifndef GROUPCLAUSE_H
#define GROUPCLAUSE_H

#include "QueryResultsTable.h"
// This class represents a group of clauses/QueryResultsTables.
class GroupClause {
public:
    GroupClause() {}

    void addOne(shared_ptr< QueryResultsTable> table);

    void addMany(vector<shared_ptr< QueryResultsTable>> tables);

    vector<shared_ptr< QueryResultsTable>> getMembers();

    set<string> getHeaders();

    bool hasCommonHeaders(shared_ptr < GroupClause > _group);

    bool hasEmptyTablesFirst();

    void merge(shared_ptr < GroupClause > _group);

    void reduceToOne();

    void setMembers(vector<shared_ptr< QueryResultsTable>> tables);

    int size();

private: 
    vector<shared_ptr< QueryResultsTable>> members;
    set<string> headers;

    void updateHeaders(vector<string> _headers);
};
#endif
