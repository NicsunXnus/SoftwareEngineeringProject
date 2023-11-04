#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/HelperFunctions.h"
#include <map>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace HelperFunctions_Test
{
	TEST_CLASS(outermostSepDetector_Test) {
	private:
		bool checkWrapper(std::vector<std::shared_ptr<pair<int, int>>> result, std::vector<std::shared_ptr<pair<int, int>>> expected) {
			int resultSize = result.size();
			int expectedSize = expected.size();
			if (resultSize != expectedSize) {
				return false;
			}
			for (int i = 0; i < resultSize; i++) {
				pair<int, int> currRes = *(result[0]);
				pair<int, int> currExp = *(expected[0]);
				if (currRes != currExp) {
					return false;
				}
			}
			return true;
		}
	public:
		TEST_METHOD(blank_success) {
			std::string input = "";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(oneBracket_success) {
			std::string input = "(abc)";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
				std::make_shared<pair<int,int>>(0, 4)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(oneCurly_success) {
			std::string input = "{abc}";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
				std::make_shared<pair<int,int>>(0, 4)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(innerBrackets_success) {
			std::string input = "((((abc))))";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
				std::make_shared<pair<int,int>>(0, 10)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(manyOuter_success) {
			std::string input = " (a)  (b) (c)";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
				std::make_shared<pair<int,int>>(1, 3),
				std::make_shared<pair<int,int>>(6, 8),
				std::make_shared<pair<int,int>>(10, 12)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(wrongType_success) {
			std::string input = " (a)  (b) (c)";
			std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			std::vector<std::shared_ptr<pair<int, int>>> expected = {
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(unclosed_failure) {
			std::string input = " {aaa";
			try {
				std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			}
			catch (std::invalid_argument e) {
				assert(true);
				return;
			}
			assert(false);
		}

		TEST_METHOD(mixedTypeUnclosed_failure) {
			std::string input = " (a)  (b} (c}";
			try {
				std::vector<std::shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			}
			catch (std::invalid_argument e) {
				assert(true);
				return;
			}
			assert(false);
		}
	};
	TEST_CLASS(isValidNumber_Test) {
	public:
		TEST_METHOD(numbers_success) {
			bool result = isValidNumber("1234");
			assert(result == true);
		}
		TEST_METHOD(leading_zero_failure) {
			bool result = isValidNumber("01234");
			assert(result == false);
		}
		TEST_METHOD(only_zero_success) {
			bool result = isValidNumber("0");
			assert(result == true);
		}

		TEST_METHOD(nonNumbers_failure) {
			std::vector<std::string> tests = {
				"0x1",
				"0b1",
				"0abc",
				"abc0",
				"-1",
				"--1",
				" ",
				"%",
				"|",
				"é"
			};
			bool result = std::any_of(tests.begin(), tests.end(), isValidNumber);
			assert(result == false);
		}
	};

	TEST_CLASS(isValidName_Test) {
	public:
		TEST_METHOD(lettersOnly_success) {
			bool result = isValidName("AbCd");
			assert(result == true); // success
		}

		TEST_METHOD(alphanumeric_success) {
			bool result = isValidName("a1Bc23D4");
			result = result && isValidName("a41qg650l");
			assert(result == true); // success
		}

		TEST_METHOD(keyword_success) {
			bool result = isValidName("read");
			result = result && isValidName("procedure");
			assert(result == true); // success
		}

		TEST_METHOD(allDigits_failure) {
			std::vector<std::string> tests = {
				"0123",
				"9",
				"10",
				"0x1",
				"0b1"
			};
			bool result = std::any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

		TEST_METHOD(leadingDigit_failure) {
			std::vector<std::string> tests = {
				"0read",
				"10read",
				"0x1read",
				"0b1read"
			};
			bool result = std::any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

		TEST_METHOD(invalidCharacter_failure) {
			std::vector<std::string> tests = {
				"b_read", // invalid character: _
				"em@il", // invalid character: @
				"one+two", // invalid character: +
				"wrong*format", // invalid character: *
				"true||false", // invalid character: |
				"one day", // invalid character: " "
				"éclaire" // invalid character: é
			};
			bool result = std::any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

	};

	TEST_CLASS(splitString_defaultValues_Test) {
	public:
		TEST_METHOD(empty_success) {
			std::string input = "";
			std::vector<std::string> expected = {};
			std::vector<std::string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(allWhitespaces_success) {
			std::string input = " \n\t\r\v\f";
			std::vector<std::string> expected = {};
			std::vector<std::string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(sentence_success) {
			std::string input = "The brown   \n fox\t  is \r quick  \v for \f   its size.";
			std::vector<std::string> expected = { "The", "brown", "fox", "is", "quick", "for", "its", "size." };
			std::vector<std::string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(excessiveWhitespaces_success) {
			std::string input = " \t\f\v\n\r\b \t\f\v\n\r\bThe \t\f\v\n\r\b \t\f\v\n\r\bbrown \t\f\v\n\r\b \t\f\v\n\r\bfox"
				" \t\f\v\n\r\bis \t\f\v\n\r\bquick \t\f\v\n\r\bfor \t\f\v\n\r\bits \t\f\v\n\r\bsize.";
			std::vector<std::string> expected = { "The", "brown", "fox", "is", "quick", "for", "its", "size." };
			std::vector<std::string> output = splitString(input);
			assert(output == expected);
		}
	};

	TEST_CLASS(splitString_customValues_Test) {
	public:
		TEST_METHOD(empty_success) {
			std::string input = "";
			std::vector<std::string> expected = {};
			std::vector<std::string> output = splitString(input, "a", true);
			assert(output == expected);
			std::vector<std::string> output2 = splitString(input, "a", false);
			assert(output2 == expected);
		}

		TEST_METHOD(allDelimiters_success) {
			std::string input = "aaaa";
			std::vector<std::string> expected = {};
			std::vector<std::string> output = splitString(input, "a", false);
			assert(output == expected);
			std::vector<std::string> expected2 = {"a", "a", "a", "a"};
			std::vector<std::string> output2 = splitString(input, "a", true);
			assert(output2 == expected2);
		}

		TEST_METHOD(sentence_success) {
			std::string input = "int x = 1;\nbool y = true;";
			std::vector<std::string> expected = { "int x = 1", "\nbool y = true" };
			std::vector<std::string> output = splitString(input, ";", false);
			assert(output == expected);
			std::vector<std::string> expected2 = { "int x = 1", ";", "\nbool y = true", ";" };
			std::vector<std::string> output2 = splitString(input, ";", true);
			assert(output2 == expected2);
		}

		TEST_METHOD(regex_success) {
			std::string input = "(( 1+2)-3)*4/5%6";
			std::vector<std::string> expected = {"1", "2", "3", "4", "5", "6"};
			std::vector<std::string> output = splitString(input, "([()+\\-/*%\\s])", false);
			std::vector<std::string> expected2 = {"(", "(", " ", "1", "+", "2", ")", "-", "3", ")", "*", "4", "/", "5", "%", "6"};
			std::vector<std::string> output2 = splitString(input, "([()+\\-/*%\\s])", true);
			assert(output == expected);
			assert(output2 == expected2);
		}
	};

	TEST_CLASS(trimWhiteSpaces_Test) {
	public:
		TEST_METHOD(nothing_success) {
			std::string input = "";
			std::string expected = "";
			std::string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(onlyWhitespaces_success) {
			std::string input = " \n\t\r\v\f\b";
			std::string expected = "";
			std::string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(sandwichedLetters_success) {
			std::string input = " \n\t\rabcd\v\f\b";
			std::string expected = "abcd";
			std::string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(sandwichedWhitespaces_success) {
			std::string input = "xy \n\t\r\v\f\bzw";
			std::string expected = "xy \n\t\r\v\f\bzw";
			std::string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(stressTest_success) {
			std::string input = " \n This:\f a\vtest* of\b(the capabilitiés\f)\t \n";
			std::string expected = "This:\f a\vtest* of\b(the capabilitiés\f)";
			std::string output = trimWhitespaces(input);
			assert(output == expected);
		}
	};

	TEST_CLASS(substring_Test) {
		TEST_METHOD(validIndexes_success) {
			std::string input = "0123456";
			std::string output = substring(input, 3, 5);
			std::string expected = "345";
			assert(output == expected);
		}
		TEST_METHOD(endIndexMoreThanLastIndex_success) {
			std::string input = "0123456";
			std::string output = substring(input, 3, 100);
			std::string expected = "3456";
			assert(output == expected);
		}
		TEST_METHOD(negativeStartIndex_failure) {
			std::string input = "0123456";
			std::string output = substring(input, -1, 5);
			std::string expected = "012345";
			assert(output == expected);
		}
		TEST_METHOD(endIndexLarger_failure) {
			std::string input = "0123456";
			std::string output = substring(input, 3, 2);
			assert(output == "");
		}
	};
}