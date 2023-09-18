#pragma once

#include <string>
#include <tuple>
#include <vector>
#include "../source/ApplicationWrapper.h"

class SystemTestWrapper {
private:

  static std::vector<std::string> unpackQueryFile(std::string queryFilePath) {

  }
  // Query - Expected
  static std::tuple<std::string, std::list<std::string>> unpackQuery(std::string indivQuery) {

  }
  
public:
  static bool run(std::string srcFilePath, std::string queryFilePath) {
    ApplicationWrapper applicationWrapper;
    applicationWrapper.parse(srcFilePath);
    std::vector<std::string> queries = unpackQueryFile(queryFilePath);
    for (std::string query : queries) {
      std::list<std::string> result = {};
      std::tuple<std::string, std::list<std::string>> unpacked = unpackQuery(query);
      applicationWrapper.evaluate(get<0>(unpacked), result);
      if (get<1>(unpacked) != result) {
        return false;
      }
    }
    return true;
  }
};

