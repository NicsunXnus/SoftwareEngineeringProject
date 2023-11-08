#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <string>
#include <string_view>
#include <vector>
#include <list>
#include <iostream>
#include <regex>
#include <set>
#include <stack>
#include <unordered_set>
#include <map>

#include "ExceptionMessages.h"
#include "Constants/SPConstants.h"
#include "Constants/DesignEnums.h"
#include "QPS/Errors/SemanticError.h"

using namespace std;

const string whitespaces = " \t\f\v\n\r\b";
const string arithmeticOpsWithWhitespaceRegex = "([()+-/*%]|\\s+)";
const string relationalOpsRegex = "([><]=?|==|!=)";

enum Separator {
  BRACKET,
  CURLY
};

// Detects outermost separators like "{}" and "()".
// Throws an exception if there are any mismatching separators, including within inner scopes.
// Returns a vector of pair<int, int> pointers. Each pair represents the indexes of a matching "{" and "}" respectively.
static vector<shared_ptr<pair<int, int>>> outermostSepDetector(string input, Separator sepType) {
  // stack keeps track of the indexes of the open curly
  stack<int> scopeTracker;
  vector<shared_ptr<pair<int, int>>> output;
  char open = sepType == Separator::BRACKET ? OPEN_BRACKET_CHAR : OPEN_CURLY_CHAR;
  char close = sepType == Separator::BRACKET ? CLOSE_BRACKET_CHAR : CLOSE_CURLY_CHAR;
  for (size_t i = 0; i < input.size(); i++) {
    char thisChar = input[i];
    if (thisChar == open) {
      scopeTracker.push(i);
      continue;
    }
    if (thisChar != close) {
      continue;
    }
    // guaranteed to be close character here
    if (scopeTracker.empty()) {
      throw invalid_argument(ExceptionMessages::extraCloseSep); 
    }
    int lastSeen = scopeTracker.top();
    scopeTracker.pop();
    if (scopeTracker.empty()) {
      output.push_back(make_shared<pair<int, int>>(lastSeen, i));
    }
  }
  if (!scopeTracker.empty()) {
    throw invalid_argument(ExceptionMessages::extraOpenSep);
  }
  return output;
}

// Checks if a given input string is a valid number as defined in SPA
static bool isValidNumber(string input) {
  if (input.empty()) {
    return false;
  } 
  if (input.length() == 1 && isdigit(input[0])) {
    return true;
  }
  if (input[0] == '0') {
    return false;
  }
  return all_of(input.begin(), input.end(), isdigit);
}

// Checks if a given input string is a valid name: LETTER (LETTER | DIGIT)*
static bool isValidName(string input) {
  if (input.empty()) {
    return false;
  }
  if (isdigit(input[0])) {
    return false;
  }
  return all_of(input.begin(), input.end(), isalnum);
}

/// <summary>
/// Splits a string with a given delimiter.
/// </summary>
///
/// <param name="input">The string to split.</param>
/// <param name="delimiter">The delimiter to split the string with. Input can be
/// a single-character string or a regex as a string</param> <param
/// name="includeDelimiter">Whether the delimiters should appear in the output
/// as well or not.</param> <returns>A vector of strings, containing the
/// original input after being split by the delimiters.</returns>
static vector<string> splitString(string input,
                                            string delimiter,
                                            bool includeDelimiter = false) {
  //  THIS CODE WAS GENERATED WITH THE HELP OF CHATGPT.
  //
  //  PROMPT GIVEN:
  //  I want to have a code in C++ that splits a given string with a regex. The
  //  return type of the function is a vector<string>. The exact character(s)
  //  used to split the string should also appear in the output vector, in the
  //  appropriate order.
  //
  //  EDITS MADE:
  //  Included a "includeDelimiters" boolean flag to check if the delimiters
  //  should be in the vector output or not.
  // ai-gen start (gpt3, 1)
  vector<string> result;
  regex regexPattern(delimiter);

  auto words_begin =
      sregex_iterator(input.begin(), input.end(), regexPattern);
  auto words_end = sregex_iterator();

  size_t last_pos = 0;

  for (auto it = words_begin; it != words_end; ++it) {
    const smatch match = *it;

    // Add the text before the match
    if (last_pos < (size_t) match.position()) {
      result.push_back(input.substr(last_pos, match.position() - last_pos));
    }

    // Add the matched text
    if (includeDelimiter) {  // this conditional was added manually
      result.push_back(match.str());
    }

    // Update the last position
    last_pos = match.position() + match.length();
  }

  // Add the text after the last match
  if (last_pos < input.length()) {
    result.push_back(input.substr(last_pos));
  }

  return result;
  // ai-gen end
}

/// <summary>
/// Splits a string with a default delimiter of whitespace characters.
/// The whitespaces will not be included in the output.
/// </summary>
///
/// <param name="input">The string to split.</param>
/// <returns>A vector of strings, containing the original input after being
/// split by the delimiters.</returns>
static vector<string> splitString(string input) {
  return splitString(input, "[\\s\b]+", false);
}

// Trims leading and trailing whitespaces.
// Original code from https://www.positioniseverything.net/cpp-string-trim/
// Modified to use " \t\f\v\n\r\b\0" as the arguments instead
static string trimWhitespaces(string str) {
  str.erase(str.find_last_not_of(whitespaces) + 1);
  str.erase(0, str.find_first_not_of(whitespaces));
  return str;
}

/// <summary>
/// Returns a continuous substring based on indices provided.
///
/// </summary>
/// <param name="str">the original string</param> 
/// <param name="startIndex">the start index. If negative indexes provided, defaults to 0</param> 
/// <param name="endIndex">the end index. if index larger than string size provided, defaults to last index == string size - 1</param> 
/// <returns>The substring</returns>
static string substring(string str, int startIndex, int endIndex) {
  if (endIndex < startIndex) {
    cerr << ExceptionMessages::endIndexLarger << endl;
    return "";
  }
  startIndex = startIndex < 0 ? 0 : startIndex;  // basically max(startIndex, 0)
  int max_index = int(str.size()) - 1;
  endIndex = endIndex > max_index
                 ? max_index
                 : endIndex;  // basically min(endIndex, stringSize - 1);
  int len = endIndex - startIndex + 1;
  return str.substr(startIndex, len);
}

// prints to console, toggle here to turn on / off for development / production
static void debug(string debugMessage) {
  bool DEBUG_MODE = true; // toggle this
  if (DEBUG_MODE) {
    cout << debugMessage + "\n" << endl;
  }
}

// Converts string views to strings
static string svToString(string_view sv) {
  string str(sv);
  return str;
}

// Converts vectors of strings into lists of strings
static list<string> vectorToList(vector<string> vectorOfString) {
  list<string> listOfString(vectorOfString.begin(), vectorOfString.end());
  return listOfString;
}


template <typename Container, typename KeyType>
static bool containerHasKey(const Container& myContainer, const KeyType& keyToCheck) {
    return myContainer.find(keyToCheck) != myContainer.end();
}


// convert map to set, by taking all the keys in the map
static unordered_set<string> getMapKeys(map<string, unordered_set<string>> stringMap) {
    unordered_set<string> keySet;
    for (const auto& pair : stringMap) {
        keySet.insert(pair.first);
    }
    return keySet;
}

static list<string> vectorToUniqueList(
    vector<string> vectorOfString) {
  list<string> listOfString(vectorOfString.begin(),
                                      vectorOfString.end());
  set<string> uniqueStrings;
  for (string str : listOfString) {
    uniqueStrings.insert(str);
  }

  list<string> uniqueStringList;
  for (string str : uniqueStrings) {
    uniqueStringList.push_back(str);
  }
  return uniqueStringList;
}

// Converts a vector of strings to a vector of string_views
static vector<string_view> sToSvVector(
    vector<string>& svVector) {
  vector<string_view> v(svVector.size());
  transform(svVector.begin(), svVector.end(), v.begin(),
                 [](const string& str) { return string_view(str); });
  return v;
}

// Gets the intersect of two sets
static unordered_set<string> get_intersection(
    unordered_set<string>& strings1,
    unordered_set<string>& strings2) {
  unordered_set<string> m(strings1.begin(), strings1.end());
  unordered_set<string> res;
  for (auto a : strings2)
    if (m.count(a)) {
      res.insert(a);
      m.erase(a);
    }
  return res;
}

// debugging
static void printVectorString(vector<string> v) {
  for (const string& element : v) {
    cout << element << endl;
  }
}

// debugging
static void printMap(map<string, vector<string>> m) {
  for (const auto& pair : m) {
    string result;
    for (const string& num : pair.second) {
      result += num;
    }
    cout << "Key: " << pair.first << ", Value: " << result << endl;
  }
}
#endif
