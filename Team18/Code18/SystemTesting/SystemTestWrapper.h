#pragma once

#include <string>
#include <utility>
#include <tuple>
#include <vector>
#include <fstream>
#include <sstream>
#include "../source/HelperFunctions.h"
#include "../source/ApplicationWrapper.h"

class SystemTestWrapper {
private:
  // Splits into individual queries
  //1 - assign
  //assign a;
  //Select a
  //3, 4
  //5000
  // Returns: vector(query, expected_results_list)
  static std::vector<std::pair<std::string, std::list<std::string>>> unpackQueryFile(std::string queryFilePath) {
    std::ifstream infile(queryFilePath);
    std::string tempQuery = "";
    std::string line;
    std::vector<std::pair<std::string, std::list<std::string>>> out;
    while (getline(infile, line)) { // first line is useless, only for comments
      for (int _ = 0; _ < 2; _++) { // next 2 lines are for declaration and query
        getline(infile, line);
        tempQuery += line;
      }
      getline(infile, line); // this line is for results
      std::list<std::string> results;
      if (line == "semantic error") {
        results.push_back("semantic error");
        out.push_back(std::pair<std::string, std::list<std::string>>(tempQuery, results));
      }
      else {
        std::vector<std::string> split = splitString(line, ",");
        for (std::string s : split) {
          results.push_back(trimWhitespaces(s));
        }
        out.push_back(std::pair<std::string, std::list<std::string>>(tempQuery, results));
      }
      tempQuery = "";
      getline(infile, line); // this line is for time limit
    }
    return out;
  }

  // checks if two lists are equal by converting them to a set and then performing equality checks
  static bool checkListEquality(std::list<std::string> left, std::list<std::string> right) {
    std::unordered_set<std::string> leftSet;
    std::unordered_set<std::string> rightSet;
    std::copy(left.begin(), left.end(), std::inserter(leftSet, leftSet.begin()));
    std::copy(right.begin(), right.end(), std::inserter(rightSet, rightSet.begin()));
    return leftSet == rightSet;
  }
  
public:
  static bool run(std::string srcFilePath, std::string queryFilePath) {
    ApplicationWrapper applicationWrapper;
    applicationWrapper.parse(srcFilePath);
    bool isAllOk = true;
    std::vector<bool> queryResults;
    std::vector<std::pair<std::string, std::list<std::string>>> unpacked = unpackQueryFile(queryFilePath);
    for (int i = 0; i < unpacked.size(); i++) {
      std::pair<std::string, std::list<std::string>> curr = unpacked[i];
      std::string query = curr.first;
      std::list < std::string> expected = curr.second;

      std::list<std::string> results = {};
      applicationWrapper.evaluate(query, results);
      bool passed = checkListEquality(expected, results);
      queryResults.push_back(passed);
      if (!passed) {
        isAllOk = false;
      }
    }
    if (!isAllOk) {
      // convert results into string and return as error
      std::stringstream ss;
      for (size_t i = 0; i < queryResults.size(); i++) {
        if (i != 0) {
          ss << "\n";
        }
        ss << "Query #" + to_string(i + 1) + ": " + to_string(queryResults[i]);
      }
      throw std::invalid_argument(ss.str());
    }
    return isAllOk;
  }
};

