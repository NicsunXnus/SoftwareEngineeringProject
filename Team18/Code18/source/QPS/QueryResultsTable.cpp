#include "QueryResultsTable.h"

shared_ptr<QueryResultsTable> QueryResultsTable::crossProduct(shared_ptr<QueryResultsTable> other) {
    vector<unordered_map<string, vector<string>>> thisColumns = this->getColumns();
    vector<unordered_map<string, vector<string>>> otherColumns = other->getColumns();
    int thisColNums = thisColumns.size();
    int otherColNums = otherColumns.size();
    if (other->isEmpty() || this->isEmpty()) {
        if (other->getSignificant() && other->isEmpty() && this->getSignificant() && !this->isEmpty()) { // TRUE EMPTY X TABLE
            return make_shared<QueryResultsTable>(thisColumns);
        }
        if (this->getSignificant() && this->isEmpty() && other->getSignificant() && !other->isEmpty()) { // TABLE X TRUE EMPTY
            return other;
        }
        return make_shared<QueryResultsTable>();
    }

    // Get the number of columns and rows in both tables
       
    int thisRowNums = thisColumns[0].begin()->second.size();
        
    int otherRowNums = otherColumns[0].begin()->second.size();

    vector<string> thisHeaders;
    vector<string> otherHeaders;

    for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
        thisHeaders.emplace_back(thisColumns[thisCol].begin()->first);

    }

    for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
        otherHeaders.emplace_back(otherColumns[otherCol].begin()->first);
    }

    vector<unordered_map<string, vector<string>>> crossProducted(thisColumns);
    vector<unordered_map<string, vector<string>>> otherColumnsCopy(otherColumns);
    /*if you have 2 tables
        etc; table1: a, b table2:C, d
        Then for each entry in table1 columns a and b, duplicate each value by size(table2)
        etc;
        we have 
        table1:              table2:
        s1, s2,   s3         s4, s5, s6
        1  ,  2,   3           a, b,  c
        4,  5,   6            d,  e,  f

        first step will->duplicateEntries()
            table1:              table2:
        s1, s2,   s3         s4, s5, s6
        1  ,  2,   3           a, b,  c
        1,  2,   3            d,  e,  f
        4,  5,    6
        4,  5 ,   6
            
        Second step -> repeatEntries()
        table1:              table2:
        s1, s2,   s3         s4, s5, s6
        1  ,  2,   3           a, b,  c
        1,  2,   3            d,  e,  f
        4,  5,    6            a, b,  c
        4,  5 ,   6            d,  e,  f
            
        Third Step -> add table2 to table1
        final table:              
        s1, s2,   s3  ,s4, s5, s6
        1  ,  2,   3  ,a, b,  c
        1,  2,   3    ,d,  e,  f
        4,  5,    6    ,a, b,  c
        4,  5 ,   6    ,d,  e,  f
        */
    for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
        string key = crossProducted[thisCol].begin()->first;
        vector<string> values = crossProducted[thisCol].begin()->second;
        crossProducted[thisCol][key] = duplicateEntries(values, otherRowNums);
    }

    for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
        string key = otherColumns[otherCol].begin()->first;
        vector<string>  values = otherColumns[otherCol].begin()->second;
        vector<string> repValues = repeatEntries(values, thisRowNums);

        otherColumnsCopy[otherCol][key] = repValues;
        crossProducted.emplace_back(otherColumnsCopy[otherCol]);
    }

    return make_shared<QueryResultsTable>(crossProducted);
}

shared_ptr<QueryResultsTable> QueryResultsTable::innerJoin(shared_ptr<QueryResultsTable> other) {
    vector<unordered_map<string, vector<string>>> thisMap = QueryResultsTable::getColumns(), otherMap = other->getColumns(), innerJoined;
    vector<string> thisHeaders = QueryResultsTable::getHeaders();
    // Get the number of columns and rows in both tables
    int thisColNums = QueryResultsTable::getNumberOfCols(), thisRowNums = QueryResultsTable::getNumberOfRows(), otherColNums = other->getNumberOfCols(), otherRowNums = other->getNumberOfRows();
    // Find the columns with the headers in both tables
    vector<tuple<int, int>> sameCols;
    for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
        //add headers of thisMap
        unordered_map<string, vector<string>> map = { {thisMap[thisCol].begin()->first, {}} };
        innerJoined.emplace_back(map);
        //at the same time, check for similar headers and record down the pairs of column numbers from both tables
        for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
            if (thisMap[thisCol].begin()->first == otherMap[otherCol].begin()->first) {
                sameCols.emplace_back((tuple<int, int>({ thisCol, otherCol })));
            }
        }
    }
    for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
        string key = otherMap[otherCol].begin()->first;
        if (find(thisHeaders.begin(), thisHeaders.end(), key) != thisHeaders.end()) {
            continue;
        }
        //add headers of otherMap only if header is not in innerJoined
        unordered_map<string, vector<string>> map = { {key, {}} };
        innerJoined.emplace_back(map);
    }
    
    //perform inner join
    for (int thisRow = 0; thisRow < thisRowNums; thisRow++)
    {
        for (int otherRow = 0; otherRow < otherRowNums; otherRow++) {
            vector<string> vect1; vector<string> vect2;
            for (tuple<int, int> indexOfSameHeaders : sameCols) {
                vect1.emplace_back(thisMap[get<0>(indexOfSameHeaders)].begin()->second[thisRow]);
                vect2.emplace_back(otherMap[get<1>(indexOfSameHeaders)].begin()->second[otherRow]);
            }
            //only if we have a intersection of both rows
            if (vect1 == vect2) {
                //then do inner join
                int colInner = 0;
                for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
                    vector<string> valuesInner = innerJoined[colInner].begin()->second;
                    unordered_map<string, vector<string>> mapInner = innerJoined[colInner];
                    valuesInner.emplace_back(thisMap[thisCol].begin()->second[thisRow]);
                    innerJoined[colInner][innerJoined[colInner].begin()->first] = valuesInner;
                    colInner++;
                }
                for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
                    if (find(thisHeaders.begin(), thisHeaders.end(), otherMap[otherCol].begin()->first) != thisHeaders.end()) {
                        continue;
                    }
                    vector<string> valuesInner = innerJoined[colInner].begin()->second;
                    unordered_map<string, vector<string>> mapInner = innerJoined[colInner];
                    valuesInner.emplace_back(otherMap[otherCol].begin()->second[otherRow]);
                    innerJoined[colInner][innerJoined[colInner].begin()->first] = valuesInner;
                    colInner++;
                }
            }
        }
    }
    return make_shared<QueryResultsTable>(innerJoined);
}

void QueryResultsTable::getPrimaryKeyOnlyTable() {
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it = find(headers.begin(), headers.end(), primaryKey);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        unordered_map<string, vector<string>> pKeyColumn = columns[index];
        vector<unordered_map<string, vector<string>>> newColumns;
        newColumns.push_back(pKeyColumn);
        columns = newColumns;
    }
    else {
        cerr << "Error getting primary key only in table";
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::createEmptyTable() {
    return make_shared<QueryResultsTable>();
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, vector<string> columnValues) {
    unordered_map<string, vector<string>> column;
    column[header] = columnValues;
    vector<unordered_map<string, vector<string>>> columns;
    columns.emplace_back(column);
    shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
    return singleColTable;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, unordered_map<string, vector<string>> columnValues) {
    vector<unordered_map<string, vector<string>>> columns;
    unordered_map<string, vector<string>> col1;
    unordered_map<string, vector<string>> col2;
    vector<string> colContent1;
    vector<string> colContent2;
    for (const auto& entry : columnValues) {
        string leftTuple = entry.first;
        for (string rightTuple : entry.second) {
            colContent1.emplace_back(leftTuple);
            colContent2.emplace_back(rightTuple);
        }
    }
    //Start of check for error
    if (colContent1.size() != colContent2.size()) {
        cerr << "Table cannot contain columns of different row length!";
        return make_shared<QueryResultsTable>();
    }
    //End of Check for error
    col1[headers[0]] = colContent1;
    col2[headers[1]] = colContent2;

    columns.emplace_back(col1);
    columns.emplace_back(col2);

    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(columns);
    return table;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, unordered_set<string> columnValues) {
    unordered_map<string, vector<string>> column;
    vector<string> vectorFromSet(columnValues.begin(), columnValues.end());
    column[header] = vectorFromSet;
    vector<unordered_map<string, vector<string>>> columns;
    columns.emplace_back(column);
    shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
    return singleColTable;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, unordered_map<string, unordered_set<string>> columnValues) {
    vector<unordered_map<string, vector<string>>> columns;
    unordered_map<string, vector<string>> col1;
    unordered_map<string, vector<string>> col2;
    vector<string> colContent1;
    vector<string> colContent2;
    for (const auto& entry : columnValues) {
        string leftTuple = entry.first;
        for (string rightTuple : entry.second) {
            colContent1.emplace_back(leftTuple);
            colContent2.emplace_back(rightTuple);
        }
    }
    //Start of check for error
    if (colContent1.size() != colContent2.size()) {
        cerr << "Table cannot contain columns of different row length!";
        return make_shared<QueryResultsTable>();
    }
    //End of Check for error
    col1[headers[0]] = colContent1;
    col2[headers[1]] = colContent2;

    columns.emplace_back(col1);
    columns.emplace_back(col2);

    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(columns);
    return table;
}   

shared_ptr<QueryResultsTable> QueryResultsTable::create2DTable(vector<string> headers, vector<vector<string>> columnValues) {
    vector<unordered_map<string, vector<string>>> columns;

    //Start of check for error
    int currSize = columnValues[0].size();
    for (vector<string> column : columnValues) {
        if (column.size() != currSize) {
            cerr << "Table cannot contain columns of different row length!";
            return make_shared<QueryResultsTable>();
        }
        currSize = column.size();
    }
    if (headers.size() != columnValues.size()) {
        cerr << "Actual and Expected Number of columns do not match";
        return make_shared<QueryResultsTable>();
    }
    //End of Check for error

    for (int headerInd = 0; headerInd < headers.size(); headerInd++) {
        unordered_map<string, vector<string>> column;
        string header = headers[headerInd];
        column[header] = columnValues[headerInd];
        columns.emplace_back(column);
    }
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(columns);
    return table;
}

void QueryResultsTable::deleteColumn(string deleteHeader) {
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it = find(headers.begin(), headers.end(), deleteHeader);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        bool isSignificant = QueryResultsTable::getNumberOfRows() > 0; // table will be "empty" after drop
        if (QueryResultsTable::getNumberOfCols() < 1) {
            QueryResultsTable::setSignificant(isSignificant);
        }
        columns.erase(columns.begin() + index);
    }
    else {
        cerr << "Header not found, returning back the same table.";
    }
}

void QueryResultsTable::renameColumn(string newName, string oldName) {
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it = find(headers.begin(), headers.end(), oldName);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        vector<string> columnContent = columns[index].begin()->second;
        columns.erase(columns.begin() + index);
        unordered_map<string, vector<string>> renamedColumn;
        renamedColumn[newName] = columnContent;
        columns.emplace_back(renamedColumn);
    }
    else {
        cerr << "Header not found, returning back the same table.";
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::filter(string key, vector<string> targets) { //assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets
    if (isEmpty()) {
        shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
        table->setSignificant(getSignificant());
        return table;
    }
    shared_ptr<QueryResultsTable> filteredTable;
    vector<string> headers = getHeaders();
    vector<unordered_map<string, vector<string>>> filteredTableColumns = createColumnsWithHeaders(headers);

    auto it = find(headers.begin(), headers.end(), key);
    if (it != headers.end()) {
        set<int> matchingRows;
        int headerIndex = distance(headers.begin(), it);
        vector<string> targetColumn = getColumns()[headerIndex].begin()->second;
            
        for (int row = 0; row < targetColumn.size(); row++) {
            if (find(targets.begin(), targets.end(), targetColumn[row]) != targets.end()) matchingRows.insert(row);
        }

        for (int matchedRow : matchingRows) { // O(r x c)
            for (int col = 0; col < headers.size(); col++) {
                filteredTableColumns[col].begin()->second.emplace_back(getColumns()[col].begin()->second[matchedRow]);
            }
        }

        filteredTable = make_shared<QueryResultsTable>(filteredTableColumns);
        return filteredTable;
    }
    else {
        //return empty table if target not in headers
        return filteredTable;
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::innerJoinOnTwoColumns(string header1, string header2) {
    if (isEmpty()) {
        shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
        table->setSignificant(getSignificant());
        return table;
    }
    shared_ptr<QueryResultsTable> filteredTable;
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it1 = find(headers.begin(), headers.end(), header1);
    auto it2 = find(headers.begin(), headers.end(), header2);
    int dist1 = distance(headers.begin(), it1); // index of header1
    int dist2 = distance(headers.begin(), it2); // index of header2

    vector<unordered_map<string, vector<string>>> thisColumns = QueryResultsTable::getColumns();
    vector<unordered_map<string, vector<string>>> filteredTableColumns = createColumnsWithHeaders(headers);

    int totalRows = QueryResultsTable::getNumberOfRows();
    for (int row = 0; row < totalRows; row++) {
        if (thisColumns[dist1].begin()->second[row] == thisColumns[dist2].begin()->second[row]) {
            for (int col = 0; col < headers.size(); col++) {
                filteredTableColumns[col].begin()->second.emplace_back(QueryResultsTable::getColumns()[col].begin()->second[row]);
            }
        }
    }
    return make_shared<QueryResultsTable>(filteredTableColumns);

}

bool QueryResultsTable::haveSimilarHeaders(shared_ptr<QueryResultsTable> other) {
    set<string> thisHeaders = getHeadersAsSet();
    set<string> otherHeaders = other->getHeadersAsSet();
    set<string> intersect;
    set_intersection(thisHeaders.begin(), thisHeaders.end(), otherHeaders.begin(), otherHeaders.end(), inserter(intersect, intersect.begin()));
    return intersect.size() > 0;
}

bool QueryResultsTable::hasHeader(string header) {
    set<string> headers = QueryResultsTable::getHeadersAsSet();

    auto it = find(headers.begin(), headers.end(), header);
        
    return it != headers.end();
}

void QueryResultsTable::duplicateColumns(string columnName) {
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it = find(headers.begin(), headers.end(), columnName);
    if (it != headers.end()) {
        // found similar header
        int index = distance(headers.begin(), it);
        columns.push_back(columns[index]);
    }

}

int QueryResultsTable::differenceInHeaders(shared_ptr<QueryResultsTable> _table) {
    set<string> _headers = _table->getHeadersAsSet();
    set<string> headers = getHeadersAsSet();
    set<string> diff;
    set_symmetric_difference(headers.begin(), headers.end(), _headers.begin(), _headers.end(), inserter(diff, diff.begin()));
    return diff.size();
}

//Helper method where (a,b) -> (a,a,b,b)
vector<string> QueryResultsTable::duplicateEntries(const vector<string>& input, int x) {
    vector<string> result;
    for (const auto& str : input) {
        for (int i = 0; i < x; ++i) {
            result.push_back(str);
        }
    }
    return result;
}

//Helper method where (a,b) -> (a,b,a,b)
vector<string> QueryResultsTable::repeatEntries(vector<string> input, int repetition) {
    vector<string> result(input);
    vector<string> copied(input);
    //ai-gen start (GPT4 & Claude2,1)
    for (int i = 1; i < repetition; i++) {
        copied.insert(copied.end(), result.begin(), result.end());
    }
    //ai-gen end
    return copied;
}

//Create the vector representation of a table. To only be used internally
vector<unordered_map<string, vector<string>>> QueryResultsTable::createColumnsWithHeaders(vector<string> theseHeaders) {
    vector<unordered_map<string, vector<string>>> columns;
    for (string header : theseHeaders) {
        unordered_map<string, vector<string>> column;
        vector<string> emptyValues;
        column[header] = emptyValues;
        columns.emplace_back(column);
    }
    return columns;
}
