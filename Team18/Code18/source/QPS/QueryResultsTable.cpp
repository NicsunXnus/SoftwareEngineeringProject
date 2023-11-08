#include "QueryResultsTable.h"
#include<cassert>
shared_ptr<QueryResultsTable> QueryResultsTable::crossProduct(shared_ptr<QueryResultsTable> other) {
    if (other->isEmpty() || this->isEmpty()) {
        if (other->getSignificant() && other->isEmpty() && this->getSignificant() &&
            !this->isEmpty()) {  // TRUE EMPTY X TABLE
            return make_shared<QueryResultsTable>(rows);
        }
        if (this->getSignificant() && this->isEmpty() && other->getSignificant() &&
            !other->isEmpty()) {  // TABLE X TRUE EMPTY
            return other;
        }
        return QueryResultsTable::createEmptyTable(other->getSignificant() && this->getSignificant());
    }
    vector<string> thisRows = getRows();
    vector<string> otherRows = other->getRows();
    vector<string> xprod(thisRows);
    int sizeOfOtherRows = other->getNumberOfRows() - 1;
    int sizeOfThisRows = getNumberOfRows() - 1;
    xprod[0] = xprod[0] + "," + otherRows[0];
    for (int i = 1; i <= sizeOfThisRows; i++) {
        for (int j = 0; j < sizeOfOtherRows; j++) {
            xprod.insert(xprod.end(), xprod[1] + "," + otherRows[j + 1]);
        }
        xprod.erase(xprod.begin() + 1);
    }
    //removeDuplicateRows(xprod);
    return make_shared<QueryResultsTable>(xprod);
  }

template <typename T, typename Iter>
void removeIndicesFromVector(std::vector<T>& v, Iter begin, Iter end)
// requires std::is_convertible_v<std::iterator_traits<Iter>::value_type, std::size_t>
{
    std::size_t current_index = 0;

    if (std::is_sorted(begin, end)) {

        // sorted version - advance through begin..end
        auto rm_iter = begin;
        const auto pred = [&](const T&) {
            // any more to remove?
            if (rm_iter != end && *rm_iter == current_index++) {
                return ++rm_iter, true;
            }
            return false;
            };
        v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());

    }
    else {

        // unsorted version - search for each index in begin..end
        const auto pred = [&](const T&) {
            return std::find(begin, end, current_index++) != end;
            };
        v.erase(std::remove_if(v.begin(), v.end(), pred), v.end());
    }
}

template <typename T, typename S>
// requires std::is_convertible_v<S::value_type, std::size_t>
void removeIndicesFromVector(std::vector<T>& v, const S& rm)
{
    using std::begin;
    using std::end;
    return removeIndicesFromVector(v, begin(rm), end(rm));
}


shared_ptr<QueryResultsTable> QueryResultsTable::innerJoin(shared_ptr<QueryResultsTable> other) {
    vector<int> thisCommon;
    vector<int> thatCommon;
    vector<string> thisRows = getRows();
    vector<string> thatRows = other->getRows();
    vector<vector<string>> thisRowsVec = getVectorizedRows();
    vector<vector<string>> otherRowsVec = other->getVectorizedRows();
    int thisCols = getNumberOfCols();
    int otherCols = other->getNumberOfCols();
    int thisRowSize = getNumberOfRows();
    int thatRowSize = other->getNumberOfRows();
    vector<string> thisHeaders = thisRowsVec[0];
    vector<string> otherHeaders = otherRowsVec[0];
    vector<string> innerJoined;
    for (int s1 = 0; s1 < thisCols; s1++) {
        for (int s2 = 0; s2 < otherCols; s2++) {
            if (thisHeaders[s1] == otherHeaders[s2]) {
                thisCommon.emplace_back(s1);
                thatCommon.emplace_back(s2);
            }
        }
    }

    vector<string> newHeaders(thisHeaders);
    removeIndicesFromVector(newHeaders, thisCommon);
    newHeaders.insert(newHeaders.end(), otherHeaders.begin(), otherHeaders.end());
    string joinedHeader = "";
    for (string s : newHeaders) {
        joinedHeader += s + ",";
    }
    joinedHeader.erase(joinedHeader.end() - 1);
    innerJoined.emplace_back(joinedHeader);

    vector<tuple<string, int>> store1;
    vector<tuple<string, int>> store2;
    for (int i = 1; i < thisRowSize; i++) {
        string temp1 = "";
        for (int j : thisCommon) { temp1 += thisRowsVec[i][j] + ","; }
        temp1.erase(temp1.end() - 1);
        store1.emplace_back(tuple<string, int>(temp1, i));
    }

    for (int i = 1; i < thatRowSize; i++) {
        string temp2 = "";
        for (int j : thatCommon) { temp2 += otherRowsVec[i][j] + ","; }
        temp2.erase(temp2.end() - 1);
        store2.emplace_back(tuple<string, int>(temp2, i));
    }

    for (tuple<string, int> i : store1) {
        for (tuple<string, int> j : store2) {
            if (get<string>(i) == get<string>(j)) {
                vector<string> temp3 = thisRowsVec[get<int>(i)];
                removeIndicesFromVector(temp3, thisCommon);
                vector<string> temp4 = otherRowsVec[get<int>(j)];
                temp3.insert(temp3.end(), temp4.begin(), temp4.end());
                string str = "";
                for (string s : temp3) {
                    str += s + ",";
                }
                str.erase(str.end() - 1);
                innerJoined.emplace_back(str);
            }
        }
    }
    return make_shared<QueryResultsTable>(innerJoined);
}

  /*struct CustomVectorHash {
      size_t operator()(const std::vector<std::string>& vec) const {
          size_t hash = 0;
          for (size_t i = 0; i < vec.size(); ++i) {
              hash ^= std::hash<std::string>{}(vec[i]) + (i << 1);
          }
          return hash;
      }
  };*/

  shared_ptr<QueryResultsTable> QueryResultsTable::removeDuplicates() {
      vector<string> result;
      // add headers of thisMap
      int numCols = getNumberOfCols();
      int numRows = getNumberOfRows();
      cout << "numRows" << numRows << endl;

      if (numRows == 0) {
          return QueryResultsTable::createEmptyTableWithHeaders(vectorizedRows[0]);
      }

      //for (int thisCol = 0; thisCol < numCols; thisCol++) {
      //    map<string, vector<string>> map = { {thisMap[thisCol].begin()->first, {}} };
      //    result.emplace_back(map);
      //}
      result.emplace_back(rows[0]);
      // store all existing rows in a set
      unordered_set<string> existingRows;
      for (int i = 1; i < numRows; i++) {
          existingRows.insert(rows[i]);
      }
      for (string vs : existingRows) {
          result.emplace_back(vs);
      }
      return make_shared<QueryResultsTable>(result);

      //cout << "existing rows size" << existingRows.size() << endl;
     // // print each row in existingRows
     // for (const auto& row : existingRows) {
     //   for (int i = 0; i < numCols; i++) {
        //  result[i].begin()->second.emplace_back(row[i]);
        //}
     // }

      // TODO: add rows from existingRows to result

      //return make_shared<QueryResultsTable>(result);
  }


shared_ptr<QueryResultsTable> QueryResultsTable::difference(
    shared_ptr<QueryResultsTable> other) {
  // Check if headers are the same
  string header = vectorizedRows[0][0];
  if (header != other->getVectorizedRows()[0][0]) {
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

// Define a hash function for pair of strings
struct PairHash {
  template <class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    auto h1 = hash<T1>{}(p.first);
    auto h2 = hash<T2>{}(p.second);
    return h1 ^ (h2 << 1);
  }
};

shared_ptr<QueryResultsTable> QueryResultsTable::difference(
    shared_ptr<QueryResultsTable> other1,
    shared_ptr<QueryResultsTable> other2) {
    // cross product
    shared_ptr<QueryResultsTable> crossed = other1->crossProduct(other2);
    // early termination if crossed is empty
    if (crossed->isEmpty()) {
        return this->getShared();
    }
    // this and crossed should have same headers
    vector<string> crossed_headers = crossed->getVectorizedRows()[0];
    assert(this->haveSimilarHeaders(crossed));

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

    vector<string> crossed_left_col = crossed->getColumnData(crossed_headers[0]);
    vector<string> crossed_right_col = crossed->getColumnData(crossed_headers[1]);
    int crossed_size = crossed_left_col.size();
    for (int i = 0; i < crossed_size; i++) {
        string left_elem = crossed_left_col[i];
        string right_elem = crossed_right_col[i];
        if (!cached_rows.count(make_pair(left_elem, right_elem))) {
            new_left_col.push_back(left_elem);
            new_right_col.push_back(right_elem);
        }
    }
    vector<vector<string>> columnValues = { new_left_col, new_right_col };
    return create2DTable(crossed_headers, columnValues);
}


void QueryResultsTable::getPrimaryKeyOnlyTable() {
    unordered_set<string> headers = getHeadersAsUnorderedSet();
    auto it = find(headers.begin(), headers.end(), primaryKey);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        for (int i = 0; i < numRows; i++) {
            rows[i] = vectorizedRows[i][index];
            vectorizedRows[i] = {rows[i]};
        }
    }
    else {
        cerr << "Error getting primary key only in table";
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::createEmptyTable(
    bool isSignificant) {
  shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
  table->setSignificant(isSignificant);
  return table;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, vector<string> columnValues) {
    vector<string> values;
    values.emplace_back(header);
    values.insert(values.end(), columnValues.begin(), columnValues.end());
    return make_shared<QueryResultsTable>(values);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, vector<string>> columnValues) {
    vector<string> values;
    string header = "";
    for (string s : headers) {
        header += s + ",";
    }
    header.erase(header.end() - 1);
    values.emplace_back(header);
    for (const auto& entry : columnValues) {
        string leftTuple = entry.first;
        for (string rightTuple : entry.second) {
            values.emplace_back(leftTuple + "," + rightTuple);
        }
    }
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(values);
    return table;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, unordered_set<string> columnValues) {
    vector<string> values;
    values.emplace_back(header);
    values.insert(values.end(), columnValues.begin(), columnValues.end());
    return make_shared<QueryResultsTable>(values);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, unordered_set<string>> columnValues) {
    vector<string> values;
    string header = "";
    for (string s : headers) {
        header += s + ",";
    }
    header.erase(header.end() - 1);
    values.emplace_back(header);
    for (const auto& entry : columnValues) {
        string leftTuple = entry.first;
        for (string rightTuple : entry.second) {
            values.emplace_back(leftTuple + "," + rightTuple);
        }
    }
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(values);
    return table;
}   

shared_ptr<QueryResultsTable> QueryResultsTable::create2DTable(vector<string> headers, vector<vector<string>> columnValues) {
    vector<string> values;
    string header = "";
    for (string s : headers) {
        header += s + ",";
    }
    header.erase(header.end() - 1);
    values.emplace_back(header);

    for (vector<string> vs : columnValues) {
        string temp = "";
        for (string s : vs) {
            temp += s + ",";
        }
        temp.erase(temp.end() - 1);
        values.emplace_back(temp);
    }

    return make_shared<QueryResultsTable>(values);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createEmptyTableWithHeaders(
    vector<string> headers) {
  vector<string> values;
  string temp = "";
  for (string header : headers) {
      temp += header + ",";
  }
  temp.erase(temp.end() - 1);
  values.emplace_back(temp);
  shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(values);
  return table;
}

void QueryResultsTable::deleteColumn(string deleteHeader) {
    unordered_set<string> headers = getHeadersAsUnorderedSet();
    auto it = find(headers.begin(), headers.end(), deleteHeader);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        bool isSignificant = getNumberOfRows() > 0; // table will be "empty" after drop
        if (getNumberOfCols() < 1) {
            setSignificant(isSignificant);
        }
        for (vector<string> vs : vectorizedRows) {
            vs.erase(vs.begin() + index);
        }
        for (int i = 0; i < numRows; i++) {
            string temp = "";
            for (string s : vectorizedRows[i]) {
                temp += s + ",";
            }
            temp.erase(temp.end() - 1);
            rows[i] = temp;
        }
        updateNumberOfCols(); updateNumberOfRows();
    }
}


void QueryResultsTable::condenseTable(unordered_set<string> targetHeaders) {
    vector<string> headers = vectorizedRows[0];
    vector<string> colsToDelete;
    for (string header : headers) {
        if (targetHeaders.find(header) == targetHeaders.end()) { //exact match
            colsToDelete.push_back(header);
        }
    }

    for (string col : colsToDelete) {
        deleteColumn(col);
    }
}

void QueryResultsTable::renameColumn(string newName, string oldName) {
    unordered_set<string> headers = getHeadersAsUnorderedSet();
    auto it = find(headers.begin(), headers.end(), oldName);
    if (it != headers.end()) {
        int index = distance(headers.begin(), it);
        vectorizedRows[0][index] = newName;
        string newHeaders = "";
        for (string s : vectorizedRows[0]) {
            newHeaders += s + ",";
        }
        newHeaders.erase(newHeaders.end() - 1);
        rows[0] = newHeaders;
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::filter(string key, vector<string> targets) { //assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets
    if (isEmpty()) {
        shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
        table->setSignificant(getSignificant());
        return table;
    }

    unordered_set<string> headers = getHeadersAsUnorderedSet();

    vector<string> values = { rows[0] };
    vector<vector<string>> vectorVals = vectorizedRows;

    auto it = find(headers.begin(), headers.end(), key);
    if (it != headers.end()) {
        int headerIndex = distance(headers.begin(), it);
        for (int i = 1; i < numRows; i++) {
            if (find(targets.begin(), targets.end(), vectorVals[i][headerIndex]) != targets.end()) {
                values.emplace_back(rows[i]);
            }
        }
        return make_shared<QueryResultsTable>(values);
    }
    else {
        //return empty table if target not in headers
        return createEmptyTable();
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::innerJoinOnTwoColumns(string header1, string header2) {
    if (isEmpty()) {
        shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
        table->setSignificant(getSignificant());
        return table;
    }
    shared_ptr<QueryResultsTable> filteredTable;
    unordered_set<string> headers = getHeadersAsUnorderedSet();
    auto it1 = find(headers.begin(), headers.end(), header1);
    auto it2 = find(headers.begin(), headers.end(), header2);
    int dist1 = distance(headers.begin(), it1); // index of header1
    int dist2 = distance(headers.begin(), it2); // index of header2

    vector<string> values;
    values.emplace_back(rows[0]);
    for (int i = 1; i < numRows; i++) {
        if (vectorizedRows[i][dist1] == vectorizedRows[i][dist2]) {
            values.emplace_back(rows[i]);
        }
    }
  
    return make_shared<QueryResultsTable>(values);

}

bool QueryResultsTable::haveSimilarHeaders(
    shared_ptr<QueryResultsTable> other) {
  set<string> thisHeaders = getHeadersAsSet();
  set<string> otherHeaders = other->getHeadersAsSet();
  set<string> intersect;
  set_intersection(thisHeaders.begin(), thisHeaders.end(), otherHeaders.begin(),
                   otherHeaders.end(), inserter(intersect, intersect.begin()));
  return intersect.size() > 0;
}

bool QueryResultsTable::hasHeader(string header) {
  set<string> headers = QueryResultsTable::getHeadersAsSet();

  auto it = find(headers.begin(), headers.end(), header);
  bool hasHeader = it != headers.end();
   
  return hasHeader;

}

bool QueryResultsTable::hasAttributeHeader(string header) {
    set<string> headers = QueryResultsTable::getHeadersAsSet();
    size_t pos = header.find('.');
    if (pos == string::npos) {
        return false;
    }
    header = header.substr(0, pos);

    auto it = find(headers.begin(), headers.end(), header);
    bool hasHeader = it != headers.end();

    return hasHeader;

}

void QueryResultsTable::duplicateColumns(string columnName) {
    unordered_set<string> headers = getHeadersAsUnorderedSet();
    auto it = find(headers.begin(), headers.end(), columnName);
    if (it != headers.end()) {
        // found similar header
        int index = distance(headers.begin(), it);
        for (int i = 0; i < numRows; i++) {
            rows[i] = rows[i] + "," + vectorizedRows[i][index];
            vectorizedRows[i].emplace_back(vectorizedRows[i][index]);
        }
    }
}

int QueryResultsTable::differenceInHeaders(
    shared_ptr<QueryResultsTable> _table) {
  set<string> _headers = _table->getHeadersAsSet();
  set<string> headers = getHeadersAsSet();
  set<string> diff;
  set_symmetric_difference(headers.begin(), headers.end(), _headers.begin(),
                           _headers.end(), inserter(diff, diff.begin()));
  return diff.size();
}
