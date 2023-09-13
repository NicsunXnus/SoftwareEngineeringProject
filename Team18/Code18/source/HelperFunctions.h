#ifndef TOKENIZERFUNCTIONS_H
#define TOKENIZERFUNCTIONS_H

#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include "TokenClasses/Token.h"

using namespace std::string_view_literals;

const std::string whitespaces = " \t\f\v\n\r\b";

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
/// <param name="delimiter">The delimiter to split the string with.</param>
/// <param name="includeDelimiter">Whether the delimiters should appear in the output as well or not.</param>
/// <returns>A vector of strings, containing the original input after being split by the delimiters.</returns>
static std::vector<std::string> splitString(std::string input, std::string delimiter, bool includeDelimiter = false) {
  //  THIS CODE WAS GENERATED WITH THE HELP OF CHATGPT.
  // 
  //  PROMPT GIVEN:
  //  I want to have a code in C++ that splits a given string with a regex. The return type of the function is 
  //  a vector<string>. The exact character(s) used to split the string should also appear in the output vector, 
  //  in the appropriate order.
  //
  //  EDITS MADE:
  //  Included a "includeDelimiters" boolean flag to check if the delimiters should be in the vector output or not.
  // ai-gen start (gpt3, 1)
  std::vector<std::string> result;
  std::regex regexPattern(delimiter);

  auto words_begin = std::sregex_iterator(input.begin(), input.end(), regexPattern);
  auto words_end = std::sregex_iterator();

  size_t last_pos = 0;

  for (auto it = words_begin; it != words_end; ++it) {
    const std::smatch match = *it;

    // Add the text before the match
    if (last_pos < match.position()) {
      result.push_back(input.substr(last_pos, match.position() - last_pos));
    }

    // Add the matched text
    if (includeDelimiter) { // this conditional was added manually
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
/// <returns>A vector of strings, containing the original input after being split by the delimiters.</returns>
static std::vector<std::string> splitString(std::string input) {
  return splitString(input, "\\s+", false);
}

// Trims leading and trailing whitespaces.
// Original code from https://www.positioniseverything.net/cpp-string-trim/
// Modified to use " \t\f\v\n\r\b\0" as the arguments instead
static std::string trimWhitespaces(std::string str) {
  str.erase(str.find_last_not_of(whitespaces) + 1);
  str.erase(0, str.find_first_not_of(whitespaces));
  return str;
}

// prints to console, toggle here to turn on / off for development / production
static void debug(std::string debugMessage) {
    bool DEBUG_MODE = true; // toggle this
    if (DEBUG_MODE) {
        std::cout << debugMessage  + "\n" << std::endl;
    }
}

#endif
