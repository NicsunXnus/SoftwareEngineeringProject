#include "QueryResultsTable.h"

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

//  Only for two single column tables
shared_ptr<QueryResultsTable> QueryResultsTable::difference(
    shared_ptr<QueryResultsTable> other1,
    shared_ptr<QueryResultsTable> other2) {
    // cross product
    shared_ptr<QueryResultsTable> crossed = other1->crossProductSet(other2);
    // early termination if crossed is empty
    if (crossed->isEmpty()) {
        return getShared();
    }
    // this and crossed should have same headers
    unordered_set<string> crossed_headers = crossed->getHeaders();
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> otherRows = crossed->getRowsSet();
    for (const auto& map : otherRows) {
        if (tableRows.find(map) == tableRows.end()) { // include rows from this table not found inside the crossProduct of the 2 tables
            res.insert(map);
        }
    }
    return make_shared<QueryResultsTable>(res);
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
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(res);
    vector<string> headers = { header };
    table->setHeaders(headers);
    return table;

}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(
    vector<string> headers,
    unordered_map<string, vector<string>> columnValues) {
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
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(res);
    table->setHeaders(headers);
    return table;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(string header, unordered_set<string> columnValues) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    for (string s : columnValues) {
        unordered_map<string, string> map;
        map.insert({ header, s });
        res.insert(map);
    }
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(res);
    vector<string> headers = { header };
    table->setHeaders(headers);
    return table;
}

shared_ptr<QueryResultsTable> QueryResultsTable::createTable(
    vector<string> headers,
    unordered_map<string, unordered_set<string>> columnValues) {
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
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(res);
    table->setHeaders(headers);
    return table;
}   

shared_ptr<QueryResultsTable> QueryResultsTable::create2DTable(unordered_set<string> headers, vector<vector<string>> columnValues) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> res;
    if (columnValues.empty()) {
        return createEmptyTableWithHeadersSet(headers);
    }
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

shared_ptr<QueryResultsTable> QueryResultsTable::createEmptyTableWithHeadersSet(unordered_set<string> headers) {
    shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>();
    table->setHeaders(headers);
    return table;
}

void QueryResultsTable::deleteColumn(string deleteHeader) {
    unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> newTableRows;
    for (unordered_map<string, string> map : tableRows) {
        string val = map.at(deleteHeader);
        map.erase(deleteHeader);
        headers.erase(deleteHeader);
        newTableRows.insert(map);
    }
    tableRows = newTableRows;
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

void QueryResultsTable::renameColumn(string newName, string oldName) {
    if (headers.find(oldName) != headers.end()) {
        unordered_set<unordered_map<string, string>, HashFunc, EqualFunc> newTableRows;
        for (unordered_map<string, string> map : tableRows) {
            string val = map.at(oldName);
            map.erase(oldName);
            headers.erase(oldName);
            map.insert({ newName, val });
            headers.insert(newName);
            newTableRows.insert(map);
        }
        tableRows = newTableRows;

    }
}


void QueryResultsTable::duplicateAndAddColumn(string newName, string oldName) {
    unordered_set<string> headers = getHeaders();
    if (headers.find(oldName) != headers.end()) {
        for (unordered_map<string, string> map : tableRows) {
            string val = map[oldName];
            map.insert({ newName, val });
        }
    }
}

shared_ptr<QueryResultsTable> QueryResultsTable::filter(string key, unordered_set<string> targets) { //assuming there is no spacing/wrong capitalisation in key & there is no duplicates in targets
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
            if (targets.find(val) != targets.end()) {
                res.insert(map);
            }
        }
        shared_ptr<QueryResultsTable> table = make_shared<QueryResultsTable>(res);
        table->setHeaders(headers);
        return table;
    }
    else {
        //return empty table if target not in headers
        return createEmptyTableWithHeadersSet(headers);
    }
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
