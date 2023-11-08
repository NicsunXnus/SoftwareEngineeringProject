#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "../source/ApplicationWrapper.h"
#include "../source/HelperFunctions.h"

using Microsoft::VisualStudio::CppUnitTestFramework::Logger;

class SystemTestWrapper {
 private:
  // Splits into individual queries
  // 1 - assign
  // assign a;
  // Select a
  // 3, 4
  // 5000
  // Returns: vector(query, expected_results_list)
  static vector<pair<string, list<string>>>
  unpackQueryFile(string queryFilePath) {
    ifstream infile(queryFilePath);
    string tempQuery = "";
    string line;
    vector<pair<string, list<string>>> out;
    while (getline(infile, line)) {  // first line is useless, only for comments
      for (int _ = 0; _ < 2;
           _++) {  // next 2 lines are for declaration and query
        getline(infile, line);
        tempQuery += line;
      }
      getline(infile, line);  // this line is for results
      list<string> results;
      if (line == "semantic error") {
        results.push_back("semantic error");
        out.push_back(
            pair<string, list<string>>(tempQuery, results));
      } else {
        vector<string> split = splitString(line, ",");
        for (string s : split) {
          results.push_back(trimWhitespaces(s));
        }
        out.push_back(
            pair<string, list<string>>(tempQuery, results));
      }
      tempQuery = "";
      getline(infile, line);  // this line is for time limit
    }
    return out;
  }

  // checks if two lists are equal by converting them to a set and then
  // performing equality checks
  static bool checkListEquality(list<string> left,
                                list<string> right) {
    unordered_set<string> leftSet;
    unordered_set<string> rightSet;
    copy(left.begin(), left.end(),
              inserter(leftSet, leftSet.begin()));
    copy(right.begin(), right.end(),
              inserter(rightSet, rightSet.begin()));
    return leftSet == rightSet;
  }

 public:
  static bool run(string srcFilePath, string queryFilePath,
                  bool debugMode = true) {
    ApplicationWrapper applicationWrapper;
    applicationWrapper.parse(srcFilePath);
    bool isAllOk = true;
    vector<bool> queryResults;
    vector<pair<string, list<string>>> unpacked =
        unpackQueryFile(queryFilePath);
    for (int i = 1; i < unpacked.size() + 1; i++) {
      pair<string, list<string>> curr = unpacked[i - 1];
      string query = curr.first;
      list<string> expected = curr.second;

      list<string> results = {};
      if (i == 2) {
          cout << endl;
      }
      applicationWrapper.evaluate(query, results);
      
      bool passed = checkListEquality(expected, results);

      if (debugMode) {
        string passedStr = passed ? " passed" : " failed";
        Logger::WriteMessage(
            ("! Query Number " + to_string(i) + passedStr + " !").c_str());
        // cout << "Query Number " << to_string(i) << endl;
        string tempActl = "Actual: ";
        string tempExp = "Expected: ";
        for (auto r : results) {
          tempActl += r + " ";
          // cout << r << " " << endl;
        }
        for (auto r : expected) {
          tempExp += r + " ";
          // cout << r << " " << endl;
        }
        Logger::WriteMessage(tempExp.c_str());
        Logger::WriteMessage(tempActl.c_str());
        Logger::WriteMessage(string("\n").c_str());
      }
      queryResults.push_back(passed);
      if (!passed) {
        isAllOk = false;
      }
    }
    if (!isAllOk) {
      // convert results into string and return as error
      stringstream ss;
      for (size_t i = 0; i < queryResults.size(); i++) {
        if (i != 0) {
          ss << "\n";
        }
        ss << "Query #" + to_string(i) + ": " + to_string(queryResults[i]);
      }
      throw invalid_argument(ss.str());
    }
    return isAllOk;
  }
};
