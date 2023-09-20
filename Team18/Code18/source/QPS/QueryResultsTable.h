#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
using namespace std;

class QueryResultsTable {
public:
    //A table can be represented by an array of ordered columns.
    //Each column is reprsented by a map, key being header and value being its contents
    //It is assumed that there are no duplicate headers in the table.
    //A table is always sorted by headers.
    QueryResultsTable(vector<map<string, vector<string>>> _columns) : columns(_columns) {
        sort(columns.begin(), columns.end());
    }

    //Constructor for empty table creation
    QueryResultsTable() {}

    /**
     * Creates a new QueryResultsTable objet that is the result of a cartesian product between the row of elements of this table
     * and the other table.
     *
     * @param other A shared pointer to a QueryResultsTable object that represents the other tables.
     * @return A shared pointer to a newly created QueryResultsTable object.
     */
    shared_ptr<QueryResultsTable> crossProduct(shared_ptr<QueryResultsTable> other) {
        vector<map<string, vector<string>>> thisColumns = this->columns;
        vector<map<string, vector<string>>> otherColumns = other->getColumns();

        // Get the number of columns and rows in both tables
        int thisColNums = thisColumns.size();
        int thisRowNums = thisColumns[0].begin()->second.size();
        int otherColNums = otherColumns.size();
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
    shared_ptr<QueryResultsTable> innerJoin(shared_ptr<QueryResultsTable> other) const {
        vector<map<string, vector<string>>> thisMap = this->columns;
        vector<map<string, vector<string>>> otherMap = other->columns;

        // Get the number of columns and rows in both tables
        int thisColNums = thisMap.size();
        int thisRowNums = thisMap[0].begin()->second.size();
        int otherColNums = otherMap.size();
        int otherRowNums = otherMap[0].begin()->second.size();

        // Find the columns with the headers in both tables
        vector<string> thisHeaders;
        vector<string> otherHeaders;
        vector<tuple<int, int>> sameCols;

        for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
            thisHeaders.emplace_back(thisMap[thisCol].begin()->first);
            for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
                auto it = find(thisHeaders.begin(), thisHeaders.end(), otherMap[otherCol].begin()->first);
                if (it != thisHeaders.end()) {
                    int index = distance(thisHeaders.begin(), it);
                    sameCols.emplace_back((tuple<int, int>({ thisCol, otherCol })));
                    thisHeaders.erase(thisHeaders.begin() + index);
                }
            }
        }

        for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
            thisHeaders.emplace_back(thisMap[thisCol].begin()->first);
        }

        for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
            otherHeaders.emplace_back(otherMap[otherCol].begin()->first);
        }
        // Perform the inner join
        vector<map<string, vector<string>>> innerJoined;

        for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
            string key = thisMap[thisCol].begin()->first;
            vector<string> values;
            map<string, vector<string>> map;
            map.insert({ key, values });
            innerJoined.emplace_back(map);
        }

        for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
            string key = otherMap[otherCol].begin()->first;
            auto it = find(thisHeaders.begin(), thisHeaders.end(), key);
            if (it != thisHeaders.end()) {
                continue;
            }
            vector<string> values;
            map<string, vector<string>> map;
            map.insert({ key, values });
            innerJoined.emplace_back(map);
        }


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
                        vector<string> values = thisMap[thisCol].begin()->second;
                        vector<string> valuesInner = innerJoined[colInner].begin()->second;
                        string keyInner = innerJoined[colInner].begin()->first;
                        map<string, vector<string>> mapInner = innerJoined[colInner];

                        valuesInner.emplace_back(values[thisRow]);
                        innerJoined[colInner][keyInner] = valuesInner;
                        colInner++;
                    }

                    for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
                        vector<string> values = otherMap[otherCol].begin()->second;
                        string key = otherMap[otherCol].begin()->first;
                        auto it = find(thisHeaders.begin(), thisHeaders.end(), key);
                        if (it != thisHeaders.end()) {
                            continue;
                        }

                        vector<string> valuesInner = innerJoined[colInner].begin()->second;
                        string keyInner = innerJoined[colInner].begin()->first;
                        map<string, vector<string>> mapInner = innerJoined[colInner];

                        valuesInner.emplace_back(values[otherRow]);
                        innerJoined[colInner][keyInner] = valuesInner;
                        colInner++;
                    }
                }
                else {
                    //else dont do
                }
            }
        }
        return make_shared<QueryResultsTable>(innerJoined);
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
     * A static method that creates a new QueryResultsTable object with the provided headers and column values.
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A vector of a vector of strings representing the column values.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, vector<vector<string>> columnValues) {
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
        shared_ptr<QueryResultsTable> filteredTable;
        vector<string> headers = this->getHeaders();
        vector<map<string, vector<string>>> filteredTableColumns = this->createColumnsWithHeaders(headers);
        
        auto it = find(headers.begin(), headers.end(), key);
        if (it != headers.end()) {
            int headerIndex = distance(headers.begin(), it);
            vector<string> targetColumn = this->columns[headerIndex].begin()->second;
            for (string target : targets) { // Time taken = O(t x r x c)
                for (int row = 0; row < targetColumn.size(); row++) {
                    if (targetColumn[row] == target) {
                        for (int col = 0; col < headers.size(); col++) {
                            filteredTableColumns[col].begin()->second.emplace_back(this->columns[col].begin()->second[row]);
                        }
                    }
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
            if (it != thisHeaders.end()) {
                return true;
            }
        }
        return false;
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

private:
    vector<map<string, vector<string>>> columns;

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
        for (int i = 1; i < repetition; i++) {
            copied.insert(copied.end(), result.begin(), result.end());
        }
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