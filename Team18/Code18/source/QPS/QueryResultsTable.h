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

    QueryResultsTable(vector<map<string, vector<string>>> _columns) : columns(_columns), isSignificant(getNumberOfCols() > 0 && getNumberOfRows() > 0) {
        sort(columns.begin(), columns.end());
    }

    //Constructor for empty table creation
    QueryResultsTable() : isSignificant(false) {}

    void condenseTable(unordered_set<string> headers);


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
    shared_ptr<QueryResultsTable> crossProduct(
        shared_ptr<QueryResultsTable> other);

    /**
    * Creates a new QueryResultsTable object that has duplicate rows removed.
    *
    * @return A shared pointer to a newly created QueryResultsTable object.
    */
    shared_ptr<QueryResultsTable> removeDuplicates();

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
    shared_ptr<QueryResultsTable> filter(string key, unordered_set<string> targets);//assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets

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

    bool QueryResultsTable::hasAttributeHeader(string header);

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

    set<string> getHeadersAsSet() {
        set<string> headers;

        for (map<string, vector<string>> column : this->columns) {
            headers.insert(column.begin()->first);
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

    //DEBUGGING: Prints out the table for visualisation
    void printTable() {
        // Print the header row
        for (const auto& m : columns) {
            for (const auto& p : m) {
                cout << p.first << " | ";
            }
        }
        cout << endl;

        // Print the data rows
        if (getNumberOfCols() > 0 && getNumberOfRows() > 0) {
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
        return getNumberOfCols() == 0 || getNumberOfRows() == 0;
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

    void setAttr(StringMap attribute) {
        attr = attribute;
    }

    StringMap getAttr() {
        return attr;
    }

    shared_ptr<QueryResultsTable> getShared() { return shared_from_this(); }

private:
    vector<map<string, vector<string>>> columns; // column name: values
    StringMap attr;
    bool isSignificant; 
    // denotes whether a table is significant or not. 
    // A significant table represents the boolean "true", regardless if the table is empty or not
    // This is especially important for queries that involve booleans, such as with clause: "this" = "this"
    string primaryKey = ""; // primary key of the table, especially important for selecting attributes 
    // e.g. Select constant.value, primary key will be constant.value, other key will be constant

    //FOR DEBUGGING
    int id;

    //Helper method where (a,b) -> (a,a,b,b)
    vector<string> duplicateEntries(const vector<string>& input, int x);

    //Helper method where (a,b) -> (a,b,a,b)
    vector<string> repeatEntries(vector<string> input, int repetition);

    //Create the vector representation of a table. To only be used internally
    vector<map<string, vector<string>>> createColumnsWithHeaders(vector<string> theseHeaders);
};
#endif
