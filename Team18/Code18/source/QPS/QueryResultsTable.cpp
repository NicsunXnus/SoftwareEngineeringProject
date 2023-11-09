#include "QueryResultsTable.h"
#include<cassert>
shared_ptr<QueryResultsTable> QueryResultsTable::crossProductSet(shared_ptr<QueryResultsTable> other) {
    if (other->isEmpty() || this->isEmpty()) {
        if (other->getSignificant() && other->isEmpty() && this->getSignificant() &&
            !this->isEmpty()) {  // TRUE EMPTY X TABLE
            return make_shared<QueryResultsTable>(tableRows);
        }
        if (this->getSignificant() && this->isEmpty() && other->getSignificant() &&
            !other->isEmpty()) {  // TABLE X TRUE EMPTY
            return other;
        }
        return QueryResultsTable::createEmptyTable(other->getSignificant() && this->getSignificant());
    } 
    unordered_set<unordered_map<string,string>, HashFunc, EqualFunc> thisRows = getRowsSet();
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> otherRows = other->getRowsSet();

    unordered_set<unordered_map<string, string>,HashFunc, EqualFunc> newRows;
    for (unordered_map<string,string> thisRow : thisRows) {
        for (unordered_map<string, string> otherRow : otherRows) {
            unordered_map<string, string> newRow = thisRow;
            for (const auto& pair : otherRow) {
                newRow.insert({pair.first, pair.second});
            }
            newRows.insert(newRow);
        }
    }
    return make_shared<QueryResultsTable>(newRows);
}



shared_ptr<QueryResultsTable> QueryResultsTable::innerJoinSet(shared_ptr<QueryResultsTable> other) {
    unordered_set<string> thisHeaders = getHeaders();
    unordered_set<string> otherHeaders = other->getHeaders();
    unordered_set<string> commonHeaders;

    for (string thisHeader : thisHeaders) {
        for (string otherHeader : otherHeaders) {
            if (thisHeader == otherHeader) {
                commonHeaders.insert(thisHeader);
            }
        }
    }

    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> thisRows = getRowsSet();
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> otherRows = other->getRowsSet();

    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> newRows;

    for (unordered_map<string, string> thisRow : thisRows) {
        for (unordered_map<string, string> otherRow : otherRows) {
            bool isMatchingRow{ true };
            for (string commonHeader : commonHeaders) {
                if (thisRow[commonHeader] != otherRow[commonHeader]) {
                    isMatchingRow = false;
                    break;
                }
            }

            if (!isMatchingRow) {
                continue;
            }

            unordered_map<string, string> newRow = thisRow;
            for (const auto& pair : otherRow) {
                string header{ pair.first };
                string value{ pair.second };

                if (commonHeaders.find(header) == commonHeaders.end()) {
                    // not a common header, add to the newly joined row
                    newRow.insert({header, value});
                }
            }
            newRows.insert(newRow);
        }
    }
    return make_shared<QueryResultsTable>(newRows);
}

shared_ptr<QueryResultsTable> QueryResultsTable::difference(
	shared_ptr<QueryResultsTable> other) {
	// Check if headers are the same
    string header = *(getHeaders().begin());
	if (header != *(other->getHeaders().begin())) {
		// Headers don't match; return an empty QRT with the same header
        unordered_set<string> empty_values = {};
        return createTable(header,empty_values);
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
    shared_ptr<QueryResultsTable> crossed = other1->crossProductSet(other2);
    // early termination if crossed is empty
    if (crossed->isEmpty()) {
        return this->getShared();
    }
    // this and crossed should have same headers
    unordered_set<string> crossed_headers = crossed->getHeaders();
    assert(this->haveSimilarHeaders(crossed));

    // cache each row in smaller table (this)
    unordered_set<pair<string, string>, PairHash> cached_rows;
    vector<string> this_left_col = this->getColumnData(*(crossed_headers.begin()));
    vector<string> this_right_col = this->getColumnData(*(next(crossed_headers.begin(), 1)));
    int cache_size = this_left_col.size();
    for (int i = 0; i < cache_size; i++) {
        cached_rows.insert(make_pair(this_left_col[i], this_right_col[i]));
    }

    // loop through rows of crossed table, only include if not in cached_rows
    vector<string> new_left_col;
    vector<string> new_right_col;

    vector<string> crossed_left_col = crossed->getColumnData(*(crossed_headers.begin()));
    vector<string> crossed_right_col = crossed->getColumnData(*(next(crossed_headers.begin(), 1)));
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
    if (headers.find(primaryKey) != headers.end()) {
        for (unordered_map<string,string> map : tableRows) {
            string pKeyValue = map.at(primaryKey);
            map.clear();
            map.insert({primaryKey,pKeyValue});
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
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    for (string s : columnValues) {
        unordered_map<string, string> map;
        map.insert({ header, s});
        res.insert(map);
    }
    return make_shared<QueryResultsTable>(res);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, vector<string>> columnValues) {
    unordered_set<unordered_map<string, string>,HashFunc,EqualFunc> res;
    for (const auto& entry : columnValues) {
        string left = entry.first;
        for (string right : entry.second) {
            unordered_map<string, string> map;
            map.insert({ headers[0], left});
            map.insert({ headers[1], right });
            res.insert(map);
        }
    }
    return make_shared<QueryResultsTable>(res);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, unordered_set<string> columnValues) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    for (string s : columnValues) {
        unordered_map<string, string> map;
        map.insert({ header, s });
        res.insert(map);
    }
    return make_shared<QueryResultsTable>(res);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(vector<string> headers, map<string, unordered_set<string>> columnValues) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    for (const auto& entry : columnValues) {
        string left = entry.first;
        for (string right : entry.second) {
            unordered_map<string, string> map;
            map.insert({ headers[0], left });
            map.insert({ headers[1], right });
            res.insert(map);
        }
    }
    return make_shared<QueryResultsTable>(res);
}   

shared_ptr<QueryResultsTable> QueryResultsTable::create2DTable(unordered_set<string> headers, vector<vector<string>> columnValues) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    for (int i = 0; i < columnValues[0].size(); i++) {
        unordered_map<string, string> map;
        for (int j = 0; j < columnValues.size();j++) {
            map.insert({ *(next(headers.begin(),j)), columnValues[j][i]});
        }
        res.insert(map);
    }
    return make_shared<QueryResultsTable>(res);
}

shared_ptr<QueryResultsTable> QueryResultsTable::createEmptyTableWithHeaders(
    vector<string> _headers) {
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
    table->setHeaders(_headers);
  return table;
}

void QueryResultsTable::deleteColumn(string deleteHeader) {
    for (unordered_map<string,string> map : tableRows) {
        map.erase(deleteHeader);
    }
}

void QueryResultsTable::condenseTable(unordered_set<string> targetHeaders) {
    unordered_set<string> headers = getHeaders();
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

void QueryResultsTable::addAttrColumn(string attr, string oldName) {
    unordered_set<string> headers = getHeaders();
    if (headers.find(oldName) != headers.end()) {
        for (unordered_map<string, string> map : tableRows) {
            string val = map[oldName];
            map.erase(oldName);
            map.insert({attr, val});
        }
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::filter(string key, vector<string> targets) { //assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets
    if (isEmpty()) {
        shared_ptr<QueryResultsTable> table = QueryResultsTable::createEmptyTable();
        table->setSignificant(getSignificant());
        return table;
    }

    unordered_set<string> headers = getHeaders();
    unordered_map<string, string> map = *(tableRows.begin());
    if (map.find(key) != map.end()) {
        unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
        for (int i = 0; i < tableRows.size(); i++) {
            unordered_map<string, string> map = *(next(tableRows.begin(), i));
            string val = map.at(key);
            if (find(targets.begin(), targets.end(), val) != targets.end()) {
                res.insert(map);
            }
        }
        return make_shared<QueryResultsTable>(res);
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
    unordered_set<string> headers = getHeaders();

    unordered_set<unordered_map<string,string>, HashFunc, EqualFunc> res;
    for (int i = 0; i < tableRows.size(); i++) {
        unordered_map<string, string> map = *(next(tableRows.begin(), i));
        if (map.at(header1) == map.at(header2)) {
            res.insert(map);
        }
    }
  
    return make_shared<QueryResultsTable>(res);

}

bool QueryResultsTable::haveSimilarHeaders(
    shared_ptr<QueryResultsTable> other) {
  unordered_set<string> thisHeaders = headers;
  unordered_set<string> otherHeaders = other->getHeaders();
  for (auto it = headers.begin(); it != headers.end();it++) {
      if (otherHeaders.find(*it) != otherHeaders.end()) {
          return true;
      }
  }
  return false;
}

bool QueryResultsTable::hasHeader(string header) {
    unordered_set<string> headers = getHeaders();
    bool hasHeader = headers.find(header) != headers.end();
    return hasHeader;

}

bool QueryResultsTable::hasAttributeHeader(string header) {
    unordered_set<string> headers = getHeaders();
    size_t pos = header.find('.');
    if (pos == string::npos) {
        return false;
    }
    header = header.substr(0, pos);
    bool hasHeader = headers.find(header) != headers.end();

    return hasHeader;

}
