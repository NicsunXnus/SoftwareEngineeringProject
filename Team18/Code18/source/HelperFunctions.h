#ifndef TOKENIZERFUNCTIONS_H
#define TOKENIZERFUNCTIONS_H

#include <list>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "ExceptionMessages.h"
#include "SimpleTokens/Token.h"

using namespace std::string_view_literals;

const std::string whitespaces = " \t\f\v\n\r\b";
const std::string arithmeticOpsWithWhitespace = "([()+\\-/*%\\s])";
const std::string relationalOps = "([><]=?|==|!=)";

// Checks if a given input string are all numbers
// Does NOT check if it is a valid integer literal (ie leading 0)
static bool isNumber(std::string input) {
  for (char const& ch : input) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}
// Checks if a given input string is alphanumeric.
static bool isAlphanumeric(std::string input) {
  for (char const& ch : input) {
    if (!std::isalnum(ch)) {
      return false;
    }
  }
  return true;
}

// Checks if a given input string is a valid name: LETTER (LETTER | DIGIT)*
static bool isValidName(std::string input) {
  bool seenFirst = false;
  for (char const& ch : input) {
    if (seenFirst) {
      if (!std::isalnum(ch)) {
        return false;
      }
      continue;
    }
    // looking at the first character
    if (!std::isalpha(ch)) {
      return false;
    }
    seenFirst = true;
  }
  return true;
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
static std::vector<std::string> splitString(std::string input,
                                            std::string delimiter,
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
  std::vector<std::string> result;
  std::regex regexPattern(delimiter);

  auto words_begin =
      std::sregex_iterator(input.begin(), input.end(), regexPattern);
  auto words_end = std::sregex_iterator();

  size_t last_pos = 0;

  for (auto it = words_begin; it != words_end; ++it) {
    const std::smatch match = *it;

    // Add the text before the match
    if (last_pos < match.position()) {
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
static std::vector<std::string> splitString(std::string input) {
  return splitString(input, "[\\s\b]+", false);
}

// Trims leading and trailing whitespaces.
// Original code from https://www.positioniseverything.net/cpp-string-trim/
// Modified to use " \t\f\v\n\r\b\0" as the arguments instead
static std::string trimWhitespaces(std::string str) {
  str.erase(str.find_last_not_of(whitespaces) + 1);
  str.erase(0, str.find_first_not_of(whitespaces));
  return str;
}

/// <summary>
/// Returns a continuous substring based on indices provided.
///
/// </summary>
/// <param name="str">the original string</param>
/// <param name="startIndex">the start index. If negative indexes provided,
/// defaults to 0</param> <param name="endIndex">the end index. if index larger
/// than string size provided, defaults to string size</param> <returns>The
/// substring</returns>
static std::string substring(std::string str, int startIndex, int endIndex) {
  if (endIndex < startIndex) {
    std::cerr << ExceptionMessages::endIndexLarger << std::endl;
    return "";
  }
  startIndex = startIndex < 0 ? 0 : startIndex;  // basically max(startIndex, 0)
  endIndex = endIndex > str.size() - 1
                 ? str.size() - 1
                 : endIndex;  // basically min(endIndex, str.size() - 1);
  int len = endIndex - startIndex + 1;
  return str.substr(startIndex, len);
}

// prints to console, toggle here to turn on / off for development / production
static void debug(std::string debugMessage) {
  bool DEBUG_MODE = true;  // toggle this
  if (DEBUG_MODE) {
    std::cout << debugMessage + "\n" << std::endl;
  }
}

static std::string svToString(std::string_view sv) {
  std::string str(sv);
  return str;
}

static std::list<std::string> vectorToList(
    std::vector<std::string> vectorOfString) {
  std::list<std::string> listOfString(vectorOfString.begin(),
                                      vectorOfString.end());
  return listOfString;
}

static std::list<std::string> vectorToUniqueList(
    std::vector<std::string> vectorOfString) {
  std::list<std::string> listOfString(vectorOfString.begin(),
                                      vectorOfString.end());
  std::set<std::string> uniqueStrings;
  for (std::string str : listOfString) {
    uniqueStrings.insert(str);
  }

  std::list<std::string> uniqueStringList;
  for (std::string str : uniqueStrings) {
    uniqueStringList.push_back(str);
  }
  return uniqueStringList;
}

// Converts a vector of strings to a vector of string_views
static std::vector<std::string_view> sToSvVector(
    std::vector<std::string>& svVector) {
  std::vector<std::string_view> v(svVector.size());
  std::transform(svVector.begin(), svVector.end(), v.begin(),
                 [](const std::string& str) { return std::string_view(str); });
  return v;
}
#endif
