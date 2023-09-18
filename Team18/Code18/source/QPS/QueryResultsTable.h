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
    //Tested using: https://www.onlinegdb.com/online_c++_compiler#
    QueryResultsTable(vector<map<string, vector<string>>> _columns) : columns(_columns) {}

    //Returns cross product for both tables with different headers
    //Performing on tables with same headers may introduce unexpected results
    shared_ptr<QueryResultsTable> crossProduct(shared_ptr<QueryResultsTable> other) {
        // Sort the columns of both tables by headers
        vector<map<string, vector<string>>> thisColumns = this->columns;
        vector<map<string, vector<string>>> otherColumns = other->getColumns();
        sort(thisColumns.begin(), thisColumns.end());
        sort(otherColumns.begin(), otherColumns.end());

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

        bool repeat = false; //check true if there are common headers, essentially reverse which table does the following process 
        std::vector<std::map<std::string, std::vector<std::string>>> crossProducted(thisColumns);
        std::vector<std::map<std::string, std::vector<std::string>>> otherColumnsCopy(otherColumns);
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

            Optional Fourth Step: if both tables have common headers then just do step 1 to 3 but flip the tables around

            Optional Fifth Step: Stack both the tables on top of each other. This will most likely create duplicate rows.

            Optional Sixth step: Remove duplicates.

            */
        for (int thisCol = 0; thisCol < thisColNums; thisCol++) {
            string key = crossProducted[thisCol].begin()->first;
            vector<string> values = crossProducted[thisCol].begin()->second;
            crossProducted[thisCol][key] = duplicateEntries(values, otherRowNums);
        }

        for (int otherCol = 0; otherCol < otherColNums; otherCol++) {
            string key = otherColumns[otherCol].begin()->first;
            auto it = find(thisHeaders.begin(), thisHeaders.end(), key);
            if (it != thisHeaders.end()) {
                repeat = true;
                continue;
            }
            vector<string>  values = otherColumns[otherCol].begin()->second;
            vector<string> repValues = repeatEntries(values, thisRowNums);

            otherColumnsCopy[otherCol][key] = repValues;
            crossProducted.emplace_back(otherColumnsCopy[otherCol]);
        }
        sort(crossProducted.begin(), crossProducted.end());

        return make_shared<QueryResultsTable>(crossProducted);
    }
    //Helper method where (a,b) -> (a,a,b,b)
    std::vector<std::string> duplicateEntries(const std::vector<std::string>& input, int x) {
        std::vector<std::string> result;
        for (const auto& str : input) {
            for (int i = 0; i < x; ++i) {
                result.push_back(str);
            }
        }
        return result;
    }

    //Helper method where (a,b) -> (a,b,a,b)
    std::vector<std::string> repeatEntries(std::vector<std::string> input, int repetition) {
        std::vector<std::string> result(input);
        std::vector<std::string> copied(input);
        for (int i = 1; i < repetition; i++) {
            copied.insert(copied.end(), result.begin(), result.end());
        }
        return copied;
    }
    
    //Getter method for columns
    vector<map<string, vector<string>>> getColumns() {
        return this->columns;
    }

    vector<string> getHeaders() {
        vector<string> headers;

        for (map<string, vector<string>> column : this->columns) {
            headers.emplace_back(column.begin()->first);
        }

        return headers;
    }

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

    //Returns inner joining of two tables
    shared_ptr<QueryResultsTable> innerJoin(shared_ptr<QueryResultsTable> other) const {
        // Sort the columns of both tables by headers
        vector<map<string, vector<string>>> thisMap = this->columns;
        vector<map<string, vector<string>>> otherMap = other->columns;
        std::sort(thisMap.begin(), thisMap.end());
        std::sort(otherMap.begin(), otherMap.end());
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
                    int index = std::distance(thisHeaders.begin(), it);
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
        sort(innerJoined.begin(), innerJoined.end());
        return make_shared<QueryResultsTable>(innerJoined);
    }

    void printByColumn() const {
        for (const auto& m : columns) {
            for (const auto& p : m) {
                cout << p.first << ": ";
                for (const auto& s : p.second) {
                    cout << s << " ";
                }
                cout << endl;
            }
        }
    }

    void printTable() {
        // Print the header row
        for (const auto& m : columns) {
            for (const auto& p : m) {
                std::cout << p.first << " | ";
            }
        }
        std::cout << std::endl;

        // Print the data rows
        int numRows = columns[0].begin()->second.size();
        for (int i = 0; i < numRows; i++) {
            for (const auto& m : columns) {
                for (const auto& p : m) {
                    std::cout << p.second[i] << " | ";
                }
            }
            std::cout << std::endl;
        }
    }

private:
    vector<map<string, vector<string>>> columns;

};