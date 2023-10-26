#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include "../HelperFunctions.h"
#include <unordered_set>
using namespace std;

class QueryResultsTable {
public:
    //A table can be represented by an array of ordered columns.
    //Each column is reprsented by a map, key being header and value being its contents
    //It is assumed that there are no duplicate headers in the table.
    //A table is always sorted by headers.

    QueryResultsTable(vector<map<string, vector<string>>> _columns) : columns(_columns), isSignificant(getNumberOfCols() > 0 && getNumberOfRows() > 0) {
        sort(columns.begin(), columns.end());
    }

    //Constructor for empty table creation
    QueryResultsTable() : isSignificant(false) {}

    // get number of rows in table
    int getNumberOfRows() {
        if (getNumberOfCols() <= 0) {
            return 0;
        }
        return columns[0].begin()->second.size();
    }

    // get number of cols in table
    int getNumberOfCols() {
        return columns.size();
    }

    /**
     * Creates a new QueryResultsTable object that is the result of a cartesian product between the row of elements of this table
     * and the other table.
     *
     * @param other A shared pointer to a QueryResultsTable object that represents the other tables.
     * @return A shared pointer to a newly created QueryResultsTable object.
     */
    shared_ptr<QueryResultsTable> crossProduct(shared_ptr<QueryResultsTable> other) {
        vector<map<string, vector<string>>> thisColumns = this->columns;
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

    /**
  * Creates a shared pointer to a QueryResultsTable object representing the intersection between this table and the other table.
  *
  * @param other A shared pointer to a QueryResultsTable object representing the other table
  * @return A sharer pointer to a newly created QueryResultsTable object
  */
    shared_ptr<QueryResultsTable> innerJoin(shared_ptr<QueryResultsTable> other) {
        vector<map<string, vector<string>>> thisMap = this->columns, otherMap = other->columns, innerJoined;
        vector<string> thisHeaders = getHeaders();
        // Get the number of columns and rows in both tables
        int thisColNums = getNumberOfCols(), thisRowNums = getNumberOfRows(), otherColNums = other->getNumberOfCols(), otherRowNums = other->getNumberOfRows();
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

    

    void getPrimaryKeyOnlyTable() {
        vector<string> headers = this->getHeaders();
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

    /**
     * A static method that creates an empty QueryResultsTable.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues A vector of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createEmptyTable() {
        return make_shared<QueryResultsTable>();
    }
    
    /**
     * A static method that creates a new QueryResultsTable object with a single column.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues A vector of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createTable(string header, vector<string> columnValues) {
        map<string, vector<string>> column;
        column[header] = columnValues;
        vector<map<string, vector<string>>> columns;
        columns.emplace_back(column);
        shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
        return singleColTable;
    }

    /**
     * A static method that flattens a map into a table of two columns, left column representing the key and right column representing the values
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A map of a string to a vector of strings representing the unflattened map.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, map<string, vector<string>> columnValues) {
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

    /**
     * A static method that creates a new QueryResultsTable object with a single column.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues An unordered set of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createTable(string header, unordered_set<string> columnValues) {
        map<string, vector<string>> column;
        vector<string> vectorFromSet(columnValues.begin(), columnValues.end());
        column[header] = vectorFromSet;
        vector<map<string, vector<string>>> columns;
        columns.emplace_back(column);
        shared_ptr<QueryResultsTable> singleColTable = make_shared<QueryResultsTable>(columns);
        return singleColTable;
    }

    /**
     * A static method that flattens a map into a table of two columns, left column representing the key and right column representing the values
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A map of a string to a vector of strings representing the unflattened map.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, map<string, unordered_set<string>> columnValues) {
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

    

    /**
     * A static method that creates a new QueryResultsTable object with the provided headers and column values.
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A vector of a vector of strings representing the column values.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> create2DTable(vector<string> headers, vector<vector<string>> columnValues) {
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

    /**
     * Deletes a column with the provided header.
     *
     * @param deleteHeader The header of the column to be deleted, represented as a string.
     */
    void deleteColumn(string deleteHeader) {
        vector<string> headers = this->getHeaders();
        auto it = find(headers.begin(), headers.end(), deleteHeader);
        if (it != headers.end()) {
            int index = distance(headers.begin(), it);
            bool isSignificant = getNumberOfRows() > 0; // table will be "empty" after drop
            if (getNumberOfCols() < 1) {
                setSignificant(isSignificant);
            }
            columns.erase(columns.begin() + index);
        }
        else {
            cerr << "Header not found, returning back the same table.";
        }
    }

    /**
     * Renames a column indicated by the header given with the new header provided.
     *
     * @param newName The new header to be changed to, represented as a string.
     * @param oldName The old header to be renamed, repersented as a string.
    */
    void renameColumn(string newName, string oldName) {
        vector<string> headers = this->getHeaders();
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

    /**
     * Creates a new QueryResultsTable object that represents the filtered table.
     *
     * @param key The header of the target column represented by a string
     * @param targets A vector of strings representing the values the column should only have.
     * @return A shared pointer to the newly created QueryResultsTable object
     * representing the old table but only containing rows having only values of the "targets" of the column with header "key"
    */
    shared_ptr<QueryResultsTable> filter(string key, vector<string> targets) { //assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets
        if (isEmpty()) {
            shared_ptr<QueryResultsTable> table = createEmptyTable();
            table->setSignificant(getSignificant());
            return table;
        }
        shared_ptr<QueryResultsTable> filteredTable;
        vector<string> headers = this->getHeaders();
        vector<map<string, vector<string>>> filteredTableColumns = this->createColumnsWithHeaders(headers);
        
        auto it = find(headers.begin(), headers.end(), key);
        if (it != headers.end()) {
            vector<int> matchingRows;
            int headerIndex = distance(headers.begin(), it);
            vector<string> targetColumn = this->columns[headerIndex].begin()->second;
            for (string target : targets) { // Time taken = O(t x r x c)
                for (int row = 0; row < targetColumn.size(); row++) {
                    /*if (targetColumn[row] == target) {
                        matchingRows.emplace_back(row);
                        for (int col = 0; col < headers.size(); col++) {
                            filteredTableColumns[col].begin()->second.emplace_back(this->columns[col].begin()->second[row]);
                        }
                    }*/
                }
            }
            for (int matchedRow : matchingRows) {
                for (int col = 0; col < headers.size(); col++) {
                    filteredTableColumns[col].begin()->second.emplace_back(this->columns[col].begin()->second[matchedRow]);
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

    /**
     * Creates a new QueryResultsTable object that represents the filtered table.
     *
     * @param header1 The header of a column represented by a string
     * @param header2 The header of another column represented by a string
     * @return A shared pointer to the newly created QueryResultsTable object
     * representing the old table but only containing rows having only values where col1 = col2
    */
    shared_ptr<QueryResultsTable> innerJoinOnTwoColumns(string header1, string header2) { 
        if (isEmpty()) {
            shared_ptr<QueryResultsTable> table = createEmptyTable();
            table->setSignificant(getSignificant());
            return table;
        }
        shared_ptr<QueryResultsTable> filteredTable;
        vector<string> headers = this->getHeaders();
        auto it1 = find(headers.begin(), headers.end(), header1);
        auto it2 = find(headers.begin(), headers.end(), header2);
        int dist1 = distance(headers.begin(), it1); // index of header1
        int dist2 = distance(headers.begin(), it2); // index of header2

        vector<map<string, vector<string>>> thisColumns = this->columns;
        vector<map<string, vector<string>>> filteredTableColumns = this->createColumnsWithHeaders(headers);

        int totalRows = this->getNumberOfRows();
        for (int row = 0; row < totalRows; row++) {
            if (thisColumns[dist1].begin()->second[row] == thisColumns[dist2].begin()->second[row]) {
                for (int col = 0; col < headers.size(); col++) {
                    filteredTableColumns[col].begin()->second.emplace_back(this->columns[col].begin()->second[row]);
                }
            }
        }
        return make_shared<QueryResultsTable>(filteredTableColumns);

    }

    /**
     * Checks whether this table and other table contains the same headers.
     *
     * @param other A shared pointer to a QueryResultsTable object representing the other table
     * @return A boolean value, with a true value representing same headers for both, and a false value for otherwise.
    */
    bool haveSameHeaders(shared_ptr<QueryResultsTable> other) {
        vector<map<string, vector<string>>> thisColumns = this->columns;
        vector<map<string, vector<string>>> otherColumns = other->getColumns();
        vector<string> thisHeaders;
        vector<string> otherHeaders;

        for (map<string, vector<string>> thisColumn : thisColumns) {
            thisHeaders.emplace_back(thisColumn.begin()->first);
        }

        for (map<string, vector<string>> otherColumn : otherColumns) {
            string header = otherColumn.begin()->first;
            auto it = find(thisHeaders.begin(), thisHeaders.end(), header);
            if (it == thisHeaders.end()) {
                return false;
            }
        }
        return true;
    }

    /**
    * Checks this header exists in the table
    *
    * @param target header
    * @return A boolean value, with a true value representing same headers for both, and a false value for otherwise.
   */
    bool hasHeader(string header) {
        vector<string> headers = getHeaders();

        auto it = find(headers.begin(), headers.end(), header);
        
        return it != headers.end();
    }

    void duplicateColumns(string columnName) {
        vector<string> headers = getHeaders();
        auto it = find(headers.begin(), headers.end(), columnName);
        if (it != headers.end()) {
            // found similar header
            int index = distance(headers.begin(), it);
            columns.push_back(columns[index]);
        }

    }
    
    //Getter method for columns
    vector<map<string, vector<string>>> getColumns() {
        return this->columns;
    }

    //Getter method for headers of columns
    vector<string> getHeaders() {
        vector<string> headers;

        for (map<string, vector<string>> column : this->columns) {
            headers.emplace_back(column.begin()->first);
        }

        return headers;
    }

    //Setter method for columns
    void setColumns(vector<map<string, vector<string>>> newColumns) {
        this->columns = newColumns;
    }

    //Getter method for data in a column
    vector<string> getColumnData(string header) {
        vector<string> data;

        for (map<string, vector<string>> column : this->columns) {
            if (column.begin()->first == header) {
                return column.begin()->second;
            }
           
        }

        return data;
    }

    //Prints out the table
    void printTable() {
        // Print the header row
        for (const auto& m : columns) {
            for (const auto& p : m) {
                cout << p.first << " | ";
            }
        }
        cout << endl;

        // Print the data rows
        int numRows = columns[0].begin()->second.size();
        for (int i = 0; i < numRows; i++) {
            for (const auto& m : columns) {
                for (const auto& p : m) {
                    cout << p.second[i] << " | ";
                }
            }
            cout << endl;
        }
    }

    void setSignificant(bool empty) {
        isSignificant = empty;
    }

    bool getSignificant() {
        return isSignificant;
    }

    bool isEmpty() {
        return getNumberOfCols() == 0 || getNumberOfRows() == 0;
    }

    void setPrimaryKey(string key) {
        primaryKey = key;
    }

    string getPrimaryKey() {
        return primaryKey;
    }

private:
    vector<map<string, vector<string>>> columns; // column name: values
    bool isSignificant; 
    // denotes whether a table is significant or not. 
    // A significant table represents the boolean "true", regardless if the table is empty or not
    // This is especially important for queries that involve booleans, such as with clause: "this" = "this"
    string primaryKey = ""; // primary key of the table, especially important for selecting attributes 
    // e.g. Select constant.value, primary key will be constant.value, other key will be constant

    //Helper method where (a,b) -> (a,a,b,b)
    vector<string> duplicateEntries(const vector<string>& input, int x) {
        vector<string> result;
        for (const auto& str : input) {
            for (int i = 0; i < x; ++i) {
                result.push_back(str);
            }
        }
        return result;
    }

    //Helper method where (a,b) -> (a,b,a,b)
    vector<string> repeatEntries(vector<string> input, int repetition) {
        vector<string> result(input);
        vector<string> copied(input);
        // ai-gen start (GPT4 & Claude2,1)
        for (int i = 1; i < repetition; i++) {
            copied.insert(copied.end(), result.begin(), result.end());
        }
        // ai-gen end
        return copied;
    }

    //Create the vector representation of a table. To only be used internally
    vector<map<string, vector<string>>> createColumnsWithHeaders(vector<string> theseHeaders) {
        vector<map<string, vector<string>>> columns;
        for (string header : theseHeaders) {
            map<string, vector<string>> column;
            vector<string> emptyValues;
            column[header] = emptyValues;
            columns.emplace_back(column);
        }
        return columns;
    }
};
