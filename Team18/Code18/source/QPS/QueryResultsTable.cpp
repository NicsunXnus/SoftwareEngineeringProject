#include "QueryResultsTable.h"
#include <cassert>

shared_ptr<QueryResultsTable> QueryResultsTable::crossProduct(shared_ptr<QueryResultsTable> other) {
    vector<map<string, vector<string>>> thisColumns = this->getColumns();
    vector<map<string, vector<string>>> otherColumns = other->getColumns();
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

    vector<map<string, vector<string>>> crossProducted(thisColumns);
    vector<map<string, vector<string>>> otherColumnsCopy(otherColumns);
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
    vector<map<string, vector<string>>> thisMap = QueryResultsTable::getColumns(), otherMap = other->getColumns(), innerJoined;
    vector<string> thisHeaders = QueryResultsTable::getHeaders();
    // Get the number of columns and rows in both tables
    int thisColNums = QueryResultsTable::getNumberOfCols(), thisRowNums = QueryResultsTable::getNumberOfRows(), otherColNums = other->getNumberOfCols(), otherRowNums = other->getNumberOfRows();
    // Find the columns with the headers in both tables
    vector<tuple<int, int>> sameCols;
    for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
        //add headers of thisMap
        map<string, vector<string>> map = { {thisMap[thisCol].begin()->first, {}} };
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
        map<string, vector<string>> map = { {key, {}} };
        innerJoined.emplace_back(map);
    }
    //perform inner join
    for (int thisRow = 0; thisRow < thisRowNums; thisRow++) {
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
                    map<string, vector<string>> mapInner = innerJoined[colInner];
                    valuesInner.emplace_back(thisMap[thisCol].begin()->second[thisRow]);
                    innerJoined[colInner][innerJoined[colInner].begin()->first] = valuesInner;
                    colInner++;
                }
                for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
                    if (find(thisHeaders.begin(), thisHeaders.end(), otherMap[otherCol].begin()->first) != thisHeaders.end()) {
                        continue;
                    }
                    vector<string> valuesInner = innerJoined[colInner].begin()->second;
                    map<string, vector<string>> mapInner = innerJoined[colInner];
                    valuesInner.emplace_back(otherMap[otherCol].begin()->second[otherRow]);
                    innerJoined[colInner][innerJoined[colInner].begin()->first] = valuesInner;
                    colInner++;
                }
            }
        }
    }
    return make_shared<QueryResultsTable>(innerJoined);
}

shared_ptr<QueryResultsTable> QueryResultsTable::difference(
    shared_ptr<QueryResultsTable> other) {
    // Check if headers are the same
    string header = this->getHeaders()[0];
    if (header != other->getHeaders()[0]) {
        // Headers don't match; return an empty QRT with the same header
        unordered_set<string> empty_values = {};
        return createTable(header, empty_values);
    }

    // Create a new unordered_set to store the difference
    unordered_set<string> diff_column_values;

    // Convert this_col to set for efficient checks
    unordered_set<string> this_col;
    for (const string& elem : this->getColumnData(header)) {
        this_col.insert(elem);
    }

    // Iterate through the values in 'other' and add those not in 'this' to
    // diff_column_values
    vector<string> other_col = other->getColumnData(header);
    for (const string& value : other_col) {
        if (!this_col.count(value)) {
            diff_column_values.insert(value);
        }
    }

    // Create new QRT with the same header and the difference set
    return createTable(header, diff_column_values);
}

// Define a hash function for std::pair of strings
struct PairHash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ (h2 << 1);
    }
};

shared_ptr<QueryResultsTable> QueryResultsTable::difference(
    shared_ptr<QueryResultsTable> other1,
                                         shared_ptr<QueryResultsTable> other2) {
    // cross product
    shared_ptr<QueryResultsTable> crossed = other1->crossProduct(other2);
    // this and crossed should have same headers
    vector<string> crossed_headers = crossed->getHeaders();
    assert(this->haveSameHeaders(crossed));

    // cache each row in smaller table (this)
    unordered_set<pair<string, string>, PairHash> cached_rows;
    vector<string> this_left_col = this->getColumnData(crossed_headers[0]);
    vector<string> this_right_col = this->getColumnData(crossed_headers[1]);
    int cache_size = this_left_col.size();
    for (int i = 0; i < cache_size; i++) {
        cached_rows.insert(make_pair(this_left_col[i], this_right_col[i]));
    }

    // loop through rows of crossed table, only include if not in cached_rows
    vector<string> new_left_col;
    vector<string> new_right_col;

    vector<string> crossed_left_col =
        crossed->getColumnData(crossed_headers[0]);
    vector<string> crossed_right_col =
        crossed->getColumnData(crossed_headers[1]);
    int crossed_size = crossed_left_col.size();
    for (int i = 0; i < crossed_size; i++) {
        string left_elem = crossed_left_col[i];
        string right_elem = crossed_right_col[i];
        if (!cached_rows.count(make_pair(left_elem, right_elem))) {
            new_left_col.push_back(left_elem);
            new_right_col.push_back(right_elem);
        }
    }
    vector<vector<string>> columnValues = {new_left_col, new_right_col};
    return create2DTable(crossed_headers, columnValues);
}

void QueryResultsTable::getPrimaryKeyOnlyTable() {
    vector<string> headers = QueryResultsTable::getHeaders();
    auto it = find(headers.begin(), headers.end(), primaryKey);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        map<string, vector<string>> pKeyColumn = columns[index];
        vector<map<string, vector<string>>> newColumns;
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
    map<string, vector<string>> column;
    column[header] = columnValues;
    vector<map<string, vector<string>>> columns;
    columns.emplace_back(column);
    shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
    return singleColTable;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, vector<string>> columnValues) {
    vector<map<string, vector<string>>> columns;
    map<string, vector<string>> col1;
    map<string, vector<string>> col2;
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
    map<string, vector<string>> column;
    vector<string> vectorFromSet(columnValues.begin(), columnValues.end());
    column[header] = vectorFromSet;
    vector<map<string, vector<string>>> columns;
    columns.emplace_back(column);
    shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
    return singleColTable;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, unordered_set<string>> columnValues) {
    vector<map<string, vector<string>>> columns;
    map<string, vector<string>> col1;
    map<string, vector<string>> col2;
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
    vector<map<string, vector<string>>> columns;

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
        map<string, vector<string>> column;
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
        map<string, vector<string>> renamedColumn;
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
    vector<map<string, vector<string>>> filteredTableColumns = createColumnsWithHeaders(headers);

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

    vector<map<string, vector<string>>> thisColumns = QueryResultsTable::getColumns();
    vector<map<string, vector<string>>> filteredTableColumns = createColumnsWithHeaders(headers);

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

bool QueryResultsTable::haveSameHeaders(shared_ptr<QueryResultsTable> other) {
    vector<map<string, vector<string>>> thisColumns = QueryResultsTable::getColumns();
    vector<map<string, vector<string>>> otherColumns = other->getColumns();
    vector<string> thisHeaders;
    vector<string> otherHeaders;

    for (map<string, vector<string>> thisColumn : thisColumns) {
        thisHeaders.emplace_back(thisColumn.begin()->first);
    }

    for (map<string, vector<string>> otherColumn : otherColumns) {
        string header = otherColumn.begin()->first;
        auto it = find(thisHeaders.begin(), thisHeaders.end(), header);
        if (it != thisHeaders.end()) {
            return true;
        }
    }
    return false;
}

bool QueryResultsTable::hasHeader(string header) {
    vector<string> headers = QueryResultsTable::getHeaders();

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
vector<map<string, vector<string>>> QueryResultsTable::createColumnsWithHeaders(vector<string> theseHeaders) {
    vector<map<string, vector<string>>> columns;
    for (string header : theseHeaders) {
        map<string, vector<string>> column;
        vector<string> emptyValues;
        column[header] = emptyValues;
        columns.emplace_back(column);
    }
    return columns;
}
