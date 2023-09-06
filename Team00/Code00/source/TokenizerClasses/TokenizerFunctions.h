#ifndef TOKENIZERFUNCTIONS_H
#define TOKENIZERFUNCTIONS_H

#include <regex>
#include <string>
#include <string_view>
#include <vector>
#include "../TokenClasses/Token.h"

using namespace std::string_view_literals;

/**
* This class represents the common functions that both PQL Tokenizer and SP Tokenizer could use
*/
class TokenizerFunctions {
public:
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
      if (includeDelimiter) {
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
  }

  /// <summary>
  /// Splits a string with a default delimiter of whitespace characters. 
  /// The whitespaces will not be included in the output.
  /// </summary>
  /// 
  /// <param name="input">The string to split.</param>
  /// <returns>A vector of strings, containing the original input after being split by the delimiters.</returns>
  static std::vector<std::string> splitString(std::string input) {
    return TokenizerFunctions::splitString(input, "\\s+", false);
  }

  // Trims leading and trailing whitespaces.
  // Original code from https://www.positioniseverything.net/cpp-string-trim/
  // Modified to use " " as the arguments instead
  static std::string trimWhitespaces(std::string str)
  {
    str.erase(str.find_last_not_of(" ") + 1);
    str.erase(0, str.find_first_not_of(" " ));
    return str;
  }

};

#endif
