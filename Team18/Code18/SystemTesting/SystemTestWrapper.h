#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
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
  static vector<tuple<string, list<string>, long long>>
    unpackQueryFile(string queryFilePath) {
    ifstream infile(queryFilePath);
    string tempQuery = "";
    string line;
    vector<tuple<string, list<string>, long long>> out;
    while (getline(infile, line)) {  // first line is useless, only for comments
      tempQuery = "";
      for (int _ = 0; _ < 2; _++) {
        // next 2 lines are for declaration and query
        getline(infile, line);
        tempQuery += line;
      }
      getline(infile, line);  // this line is for results
      list<string> results;
      vector<string> split = splitString(line, ",");
      for (string s : split) {
        results.push_back(trimWhitespaces(s));
      }
      getline(infile, line);  // this line is for time limit
      out.push_back(
        tuple<string, list<string>, long long>(tempQuery, results, stoi(line)));
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

  // check what is in actual but not in expected
  static unordered_set<string> checkExtra(list<string> expected, list<string> actual) {
    unordered_set<string> expectedSet;
    copy(expected.begin(), expected.end(),
      inserter(expectedSet, expectedSet.begin()));
    unordered_set<string> extras;
    for (string s : actual) {
      if (!containerHasKey(expectedSet, s)) {
        extras.insert(s);
      }
    }
    return extras;
  }
  template<typename T>
  static string numToFixedLenStr(T number, int strLen) {
    std::stringstream ss;
    ss << std::setw(strLen) << std::setfill(' ') << number;
    return ss.str();
  }

public:
  static bool run(string srcFilePath, string queryFilePath,
    bool debugMode = true) {
    ApplicationWrapper applicationWrapper;
    applicationWrapper.parse(srcFilePath);
    bool isAllOk = true;
    vector<bool> queryResults;
    vector<tuple<string, list<string>, long long>> unpacked =
      unpackQueryFile(queryFilePath);
    for (int i = 1; i < unpacked.size() + 1; i++) {
      tuple<string, list<string>, long long> curr = unpacked[i - 1];
      string query = get<0>(curr);
      list<string> expected = get<1>(curr);
      long long timeLimit = get<2>(curr);

      list<string> results = {};
      auto start = chrono::high_resolution_clock::now();
      applicationWrapper.evaluate(query, results);
      auto stop = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
      long long timeTaken = duration.count();

      bool TLE = timeTaken > timeLimit;
      bool passed = checkListEquality(expected, results);
      if (debugMode) {
        string passedStr = passed
          ? TLE
          ? " TLE   "
          : " passed"
          : " failed";
        Logger::WriteMessage(
          ("Query  " + numToFixedLenStr(i, 3) + passedStr + " | ").c_str());
        string time = "Time Taken: " + numToFixedLenStr(timeTaken, 5) +
          " / " + numToFixedLenStr(timeLimit, 4) + " | ";
        Logger::WriteMessage(time.c_str());
        if (!passed) {
          string extras = "Extra results: ";
          string missing = "Missing results: ";
          for (auto s : checkExtra(expected, results)) {
            extras += s + " ";
          }
          for (auto s : checkExtra(results, expected)) {
            missing += s + " ";
          }
          Logger::WriteMessage(string("\n").c_str());
          Logger::WriteMessage(extras.c_str());
          Logger::WriteMessage("| ");
          Logger::WriteMessage(missing.c_str());
          Logger::WriteMessage(string("\n").c_str());
        }
        string tempActl = "Actual: ";
        string tempExp = "Expected: ";
        for (auto r : results) {
          tempActl += r + " ";
        }
        for (auto r : expected) {
          tempExp += r + " ";
        }
        Logger::WriteMessage(tempExp.c_str());
        Logger::WriteMessage("| ");
        Logger::WriteMessage(tempActl.c_str());
        Logger::WriteMessage(string("\n").c_str());
        if (!passed) Logger::WriteMessage(string("\n").c_str());
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
