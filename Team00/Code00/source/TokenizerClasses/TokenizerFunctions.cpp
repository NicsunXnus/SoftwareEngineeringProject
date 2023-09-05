#include <regex>

#include "TokenizerFunctions.h"

/// <summary>
/// Splits a string with a given delimiter.
/// </summary>
/// <param name="input">The string to split.</param>
/// <param name="delimiter">The delimiter to split the string with.</param>
/// <returns>A vector of strings, containing the original input after being split by the delimiters.</returns>
 
//  THIS CODE WAS GENERATED WITH THE HELP OF CHATGPT.
// 
//  PROMPT GIVEN:
//  write a function in C++ with the following function signature:
//  static std::vector<std::string> splitString(std::string input, std::string regex);
//
//  This function will take in a string, named input, and another string, named regex.
//  It will split the input string into as many pieces as necessary, according to the regex.
//
//  Output will be a vector of strings, whereby each string was split by the regex

static std::vector<std::string> splitString(std::string input, std::string delimiter) {
  std::vector<std::string> result;
  std::regex regex(delimiter);
  std::sregex_token_iterator iterator(input.begin(), input.end(), regex, -1);
  std::sregex_token_iterator end;

  while (iterator != end) {
    result.push_back(*iterator);
    ++iterator;
  }

  return result;
}

/// <summary>
/// Splits a string with a default delimiter of whitespace characters.
/// </summary>
/// <param name="input">The string to split.</param>
/// <returns>A vector of strings, containing the original input after being split by the delimiters.</returns>
static std::vector<std::string> splitString(std::string input) {
  return TokenizerFunctions::splitString(input, "\\s+");
}