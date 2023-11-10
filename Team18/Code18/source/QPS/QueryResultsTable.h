#ifndef QUERYRESULTSTABLE_H
#define QUERYRESULTSTABLE_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <memory>
#include "../HelperFunctions.h"
#include <unordered_set>
#include <unordered_map>
using namespace std;
// Represents the results of an evaluated clause. The data structure used is an unordered set of map<string,string>.
// Each map represents a row, with the key being a synonym and the value being the column value of the row.
// An unordered set will naturally remove duplicate rows while a map will remove duplicate columns.
// A map will also allow faster retrieval of column value.
class QueryResultsTable : public enable_shared_from_this<QueryResultsTable> {
 public:
    
     struct HashFunc {
         size_t operator()(const std::unordered_map<std::string, std::string>& map) const {
             size_t hash = 0;
             for (const auto& pair : map) {
                 hash ^= std::hash<std::string>()(pair.first) + std::hash<std::string>()(pair.second);
             }
             return hash;
         }
     };

     struct EqualFunc {
         bool operator()(const std::unordered_map<std::string, std::string>& map1, const std::unordered_map<std::string, std::string>& map2) const {
             return map1 == map2;
         }
     };

    // overloaded constructor to create set qrt
    QueryResultsTable(unordered_set<unordered_map<string, string>,HashFunc,EqualFunc> rows) {
        if (rows.size() == 0) {
            numRows = 0; numCols = 0; isSignificant = false;
        }
        else {
            // code for new table
            tableRows.insert(rows.begin(), rows.end());
            numRows = rows.size();
            auto map = rows.begin();
            numCols = (*map).size();
            isSignificant = numRows > 0;
            setHeaders();
        }
    }

    //Constructor for empty table creation
    QueryResultsTable() : isSignificant(false), numRows(0), numCols(0) {}

    void condenseTable(unordered_set<string> headers);

    // get number of rows in table
    int getNumberOfRows() {
        return tableRows.size();
    }

    // get number of cols in table
    int getNumberOfCols() {
        return headers.size();
    }

    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> getRowsSet() {
        return tableRows;
    }

    /**
     * Creates a new QueryResultsTable object that is the result of a cartesian product between the row of elements of this table
     * and the other table.
     *
     * @param other A shared pointer to a QueryResultsTable object that represents the other tables.
     * @return A shared pointer to a newly created QueryResultsTable object.
     */
    shared_ptr<QueryResultsTable> crossProductSet(shared_ptr<QueryResultsTable> other);

    /**
      * Creates a shared pointer to a QueryResultsTable object representing the intersection between this table and the other table.
      *
      * @param other A shared pointer to a QueryResultsTable object representing the other table
      * @return A sharer pointer to a newly created QueryResultsTable object
    */
    shared_ptr<QueryResultsTable> innerJoinSet(shared_ptr<QueryResultsTable> other);

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

    // Sets the current table to only the column values of the primary key
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
    static shared_ptr<QueryResultsTable> createTable(vector<string> headers, unordered_map<string, vector<string>> columnValues);

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
    static shared_ptr<QueryResultsTable> createTable(
        vector<string> headers,
        unordered_map<string, unordered_set<string>> columnValues);

    /**
     * A static method that creates a new QueryResultsTable object with the provided headers and column values.
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @param columnValues A vector of a vector of strings representing the column values.
     * @return A shared pointer to the newly created QueryResultsTable object.
    */
    static shared_ptr<QueryResultsTable> create2DTable(unordered_set<string> headers, vector<vector<string>> columnValues);

    /**
     * A static method that creates a new QueryResultsTable object with the provided headers
     *
     * @param headers A vector of strings representing the headers of the table to be created.
     * @return A shared pointer to the newly created QueryResultsTable object, which is empty
    */
    static shared_ptr<QueryResultsTable> createEmptyTableWithHeaders(vector<string> headers);
    static shared_ptr<QueryResultsTable> createEmptyTableWithHeadersSet(unordered_set<string> headers);

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

    void duplicateAndAddColumn(string newName, string oldName);

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
    
    bool QueryResultsTable::hasAttributeHeader(string header);

    void setHeaders() {
        if (numRows == 0) return;
        auto map = tableRows.begin();
        for (const auto& pair : *map) {
            headers.insert(pair.first);
        }
    }

    unordered_set<string> getHeaders() {
        return headers;
    }

    void setHeaders(vector<string> _headers) {
        unordered_set<string> finalheaders;
        finalheaders.insert(_headers.begin(), _headers.end());
        headers = finalheaders;
    }

    void setHeaders(unordered_set<string> _headers) {
         unordered_set<string> finalheaders;
        finalheaders.insert(_headers.begin(), _headers.end());
        headers = finalheaders;
    }

    //Getter method for data in a column
    vector<string> getColumnData(string synonym) {
        vector<string> res;
        for (const auto& map : tableRows) {
            res.emplace_back(map.at(synonym));
        }
        return res;
    }

    //DEBUGGING: Prints out the table for visualisation
    void printTable() {
        for (string s : headers) {
            cout << s << " ";
        }
        cout << endl;
        for (const auto& map : tableRows) {
            for (string s : headers) {
                cout << map.at(s) << " ";
            }
            cout << endl;
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
        return getNumberOfRows() == 0;
    }

    void setPrimaryKey(string key) {
        primaryKey = key;
    }

    string getPrimaryKey() {
        return primaryKey;
    }

    void setAttr(StringMap attribute) {
        attr = attribute;
    }

    StringMap getAttr() {
        return attr;
    }

    shared_ptr<QueryResultsTable> getShared() { return shared_from_this(); }

private:
    int numRows;
    int numCols;
    StringMap attr;
    bool isSignificant; 
    // denotes whether a table is significant or not. 
    // A significant table represents the boolean "true", regardless if the table is empty or not
    // This is especially important for queries that involve booleans, such as with clause: "this" = "this"
    string primaryKey = ""; // primary key of the table, especially important for selecting attributes 
    // e.g. Select constant.value, primary key will be constant.value, other key will be constant

    unordered_set<unordered_map<string,string>, HashFunc, EqualFunc> tableRows;
    unordered_set<string> headers;

};
#endif
