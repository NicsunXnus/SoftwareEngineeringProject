#ifndef QUERYRESULTSTABLE_H
#define QUERYRESULTSTABLE_H
#pragma warning (disable :26495)
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include "../HelperFunctions.h"
#include <unordered_set>
using namespace std;

class QueryResultsTable : public enable_shared_from_this<QueryResultsTable> {
 public:
    //A table can be represented by an array of ordered columns.
    //Each column is represented by a map, key being header and value being its contents
    //It is assumed that there are no duplicate headers in the table.
    //A table is always sorted by headers.

    /*QueryResultsTable(vector<map<string, vector<string>>> _columns) : columns(_columns), isSignificant(getNumberOfCols() > 0 && getNumberOfRows() > 0) {
        //sort(columns.begin(), columns.end());
    }*/

    QueryResultsTable(vector<string> _rows) : rows(_rows), isSignificant(getNumberOfCols() > 0 && getNumberOfRows() > 0) {
        //sort(columns.begin(), columns.end());
        if (_rows.size() == 0) {
            numRows = 0; numCols = 0;
        }
        else {
            vectorizedRows = vectorizeRows(rows);
            numRows = rows.size();
            numCols = numRows > 0 ? vectorizedRows[0].size() : 0;
            headersUnorderedSet = getHeadersAsUnorderedSet();
            headersSet = getHeadersAsSet();
        }
    }

    //Constructor for empty table creation
    QueryResultsTable() : isSignificant(false), numRows(0), numCols(0) {}

    // get number of rows in table
    int getNumberOfRows() {
        return numRows;
    }

    void updateNumberOfRows() {
        numRows = rows.size();
    }

    // get number of cols in table
    int getNumberOfCols() {
        return numCols;
    }

    void updateNumberOfCols() {
        numCols = numRows > 0 ? vectorizedRows[0].size() : 0;
    }

    /**
     * Creates a new QueryResultsTable object that is the result of a cartesian product between the row of elements of this table
     * and the other table.
     *
     * @param other A shared pointer to a QueryResultsTable object that represents the other tables.
     * @return A shared pointer to a newly created QueryResultsTable object.
     */
    shared_ptr<QueryResultsTable> crossProduct(shared_ptr<QueryResultsTable> other);

    /**
  * Creates a shared pointer to a QueryResultsTable object representing the intersection between this table and the other table.
  *
  * @param other A shared pointer to a QueryResultsTable object representing the other table
  * @return A sharer pointer to a newly created QueryResultsTable object
  */
    shared_ptr<QueryResultsTable> innerJoin(
        shared_ptr<QueryResultsTable> other);

    /**
     * Creates a shared pointer to a QueryResultsTable object representing the
     * difference between this table and one other table.
     *
     * @param other A shared pointer to a QueryResultsTable object representing
     * the other table.
     * this table's column values is a subset of the other table's column
     * values.
     * @return A shared pointer to a newly created QueryResultsTable object
     * (single column)
     */
    shared_ptr<QueryResultsTable> difference(
        shared_ptr<QueryResultsTable> other);

    /**
     * Creates a shared pointer to a QueryResultsTable object representing the
     * difference between this two-column table and two other single column
     * tables.
     * Each one of this table's headers is identical to other1/other2's header.
     * Runs in O(m+n) time, where m is size of this QRT, n is size of other QRT
     *
     * @param other1 A shared pointer to a QueryResultsTable object representing
     * a single column other table.
     * this table has a set of column values is a subset of the other table's
     * column values.
     * @param other2 A shared pointer to a QueryResultsTable object representing
     * a single column other table.
     * this table has a set of column values is a subset of the other table's
     * column values.
     * @return A shared pointer to a newly created QueryResultsTable object
     * (two columns)
     */
    shared_ptr<QueryResultsTable> difference(
        shared_ptr<QueryResultsTable> other1,
        shared_ptr<QueryResultsTable> other2);

    void getPrimaryKeyOnlyTable();

    /**
     * A static method that creates an empty QueryResultsTable.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues A vector of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createEmptyTable(bool isSignificant = false);
    
    /**
     * A static method that creates a new QueryResultsTable object with a single column.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues A vector of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createTable(string header, vector<string> columnValues);

    /**
     * A static method that flattens a map into a table of two columns, left column representing the key and right column representing the values
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A map of a string to a vector of strings representing the unflattened map.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, map<string, vector<string>> columnValues);

    /**
     * A static method that creates a new QueryResultsTable object with a single column.
     *
     * @param header The header of the column, represented as a string.
     * @param columnValues An unordered set of strings representing the values in the column.
     *                     The vector should have the same length for all columns in the table.
     * @return A shared_ptr to the newly created QueryResultsTable object.
     */
    static shared_ptr<QueryResultsTable> createTable(string header, unordered_set<string> columnValues);

    /**
     * A static method that flattens a map into a table of two columns, left column representing the key and right column representing the values
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A map of a string to a vector of strings representing the unflattened map.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, map<string, unordered_set<string>> columnValues);

    /**
     * A static method that creates a new QueryResultsTable object with the provided headers and column values.
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A vector of a vector of strings representing the column values.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> create2DTable(vector<string> headers, vector<vector<string>> columnValues);

    /**
     * A static method that creates a new QueryResultsTable object with the provided headers
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @return A shared pointer to the newly created QueryResultsTable object, which is empty
    */
    static shared_ptr<QueryResultsTable> createEmptyTableWithHeaders(vector<string> headers);

    /**
     * Deletes a column with the provided header.
     *
     * @param deleteHeader The header of the column to be deleted, represented as a string.
     */
    void deleteColumn(string deleteHeader);

    /**
     * Renames a column indicated by the header given with the new header provided.
     *
     * @param newName The new header to be changed to, represented as a string.
     * @param oldName The old header to be renamed, represented as a string.
    */
    void renameColumn(string newName, string oldName);

    /**
     * Creates a new QueryResultsTable object that represents the filtered table.
     *
     * @param key The header of the target column represented by a string
     * @param targets A vector of strings representing the values the column should only have.
     * @return A shared pointer to the newly created QueryResultsTable object
     * representing the old table but only containing rows having only values of the "targets" of the column with header "key"
    */
    shared_ptr<QueryResultsTable> filter(string key, vector<string> targets);//assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets

    /**
     * Creates a new QueryResultsTable object that represents the filtered table.
     *
     * @param header1 The header of a column represented by a string
     * @param header2 The header of another column represented by a string
     * @return A shared pointer to the newly created QueryResultsTable object
     * representing the old table but only containing rows having only values where col1 = col2
    */
    shared_ptr<QueryResultsTable> innerJoinOnTwoColumns(string header1, string header2);

    /**
     * Checks whether this table and other table contains at least one same header.
     *
     * @param other A shared pointer to a QueryResultsTable object representing the other table
     * @return A boolean value, with a true value representing same headers for both, and a false value for otherwise.
    */
    bool haveSimilarHeaders(shared_ptr<QueryResultsTable> other);

    /**
    * Checks this header exists in the table
    *
    * @param target header
    * @return A boolean value, with a true value representing same headers for both, and a false value for otherwise.
   */
    bool hasHeader(string header);

    void duplicateColumns(string columnName);
    
    /**
    * Returns the number of headers in the argument not found within this table.
    *
    * @param shared_ptr<QueryResultsTable> _table
    * @return See description
   */
    int differenceInHeaders(shared_ptr<QueryResultsTable> _table);

    vector<string> splitString(string s, string del = " ")
    {
        vector<string> res;
        int start, end = -1 * del.size();
        do {
            start = end + del.size();
            end = s.find(del, start);
            res.emplace_back(s.substr(start, end - start));
        } while (end != -1);
        return res;
    }

    vector<vector<string>> vectorizeRows(vector<string> rows) {
        vector<vector<string>> res;
        for (string s : rows) {
            res.emplace_back(splitString(s, ","));
        }
        return res;
    }

    //Getter method for headers of columns
    string getHeaders() {
        return numRows > 0  ? rows[0] : "";
    }


    vector<string> getRows() {
        return rows;
    }

    vector<vector<string>> getVectorizedRows() {
        return vectorizedRows;
    }

    unordered_set<string> getHeadersAsUnorderedSet() {
        vector<string> temp = splitString(getHeaders(), ",");
        unordered_set<string> res;
        res.insert(temp.begin(), temp.end());
        return res;
    }

    set<string> getHeadersAsSet() {
        set<string> res;
        res.insert(headersUnorderedSet.begin(), headersUnorderedSet.end());
        return res;
    }

    //Setter method for columns
    void setRows(vector<string> newRows) {
        rows = newRows;
    }

    //Getter method for data in a column
    vector<string> getColumnData(string header) {
        vector<string> res;
        if (numRows == 0) return res;
        auto it = find(vectorizedRows[0].begin(), vectorizedRows[0].end(), header);
        int index = it - vectorizedRows[0].begin();
        for (vector<string> row : vectorizedRows) {
            res.emplace_back(row[index]);
        }
        res.erase(res.begin());
        return res;
    }

    //DEBUGGING: Prints out the table for visualisation
    void printTable() {
        for (string s : rows) {
            cout << s << endl;
        }
    }

    void setSignificant(bool empty) {
        isSignificant = empty;
    }

    void flipSignificant() {
		isSignificant = !isSignificant;
	}

    bool getSignificant() {
        return isSignificant;
    }

    bool isEmpty() {
        return getNumberOfCols() == 0 || getNumberOfRows() <= 1;
    }

    void setPrimaryKey(string key) {
        primaryKey = key;
    }

    string getPrimaryKey() {
        return primaryKey;
    }

    //BELOW TWO FUNCTIONS USED FOR DEBUGGING
    void setId(int _id) {
        id = _id;
    }

    int getId() {
        return id;
    }

    shared_ptr<QueryResultsTable> getShared() { return shared_from_this(); }

private:
    int numRows;
    int numCols;
    bool isSignificant; 
    // denotes whether a table is significant or not. 
    // A significant table represents the boolean "true", regardless if the table is empty or not
    // This is especially important for queries that involve booleans, such as with clause: "this" = "this"
    string primaryKey = ""; // primary key of the table, especially important for selecting attributes 
    // e.g. Select constant.value, primary key will be constant.value, other key will be constant
    set<string> headersSet;
    unordered_set<string> headersUnorderedSet;
    vector<string> rows;
    vector<vector<string>> vectorizedRows;
    //FOR DEBUGGING
    int id;
};
#endif
