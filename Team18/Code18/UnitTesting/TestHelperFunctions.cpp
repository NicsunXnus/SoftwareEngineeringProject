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
		bool checkWrapper(vector<shared_ptr<pair<int, int>>> result, vector<shared_ptr<pair<int, int>>> expected) {
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
			string input = "";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			vector<shared_ptr<pair<int, int>>> expected = {
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(oneBracket_success) {
			string input = "(abc)";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			vector<shared_ptr<pair<int, int>>> expected = {
				make_shared<pair<int,int>>(0, 4)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(oneCurly_success) {
			string input = "{abc}";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			vector<shared_ptr<pair<int, int>>> expected = {
				make_shared<pair<int,int>>(0, 4)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(innerBrackets_success) {
			string input = "((((abc))))";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			vector<shared_ptr<pair<int, int>>> expected = {
				make_shared<pair<int,int>>(0, 10)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(manyOuter_success) {
			string input = " (a)  (b) (c)";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::BRACKET);
			vector<shared_ptr<pair<int, int>>> expected = {
				make_shared<pair<int,int>>(1, 3),
				make_shared<pair<int,int>>(6, 8),
				make_shared<pair<int,int>>(10, 12)
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(wrongType_success) {
			string input = " (a)  (b) (c)";
			vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			vector<shared_ptr<pair<int, int>>> expected = {
			};
			bool result = checkWrapper(output, expected);
			assert(result);
		}

		TEST_METHOD(unclosed_failure) {
			string input = " {aaa";
			try {
				vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			}
			catch (invalid_argument e) {
				assert(true);
				return;
			}
			assert(false);
		}

		TEST_METHOD(mixedTypeUnclosed_failure) {
			string input = " (a)  (b} (c}";
			try {
				vector<shared_ptr<pair<int, int>>> output = outermostSepDetector(input, Separator::CURLY);
			}
			catch (invalid_argument e) {
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
			vector<string> tests = {
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
			bool result = any_of(tests.begin(), tests.end(), isValidNumber);
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
			vector<string> tests = {
				"0123",
				"9",
				"10",
				"0x1",
				"0b1"
			};
			bool result = any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

		TEST_METHOD(leadingDigit_failure) {
			vector<string> tests = {
				"0read",
				"10read",
				"0x1read",
				"0b1read"
			};
			bool result = any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

		TEST_METHOD(invalidCharacter_failure) {
			vector<string> tests = {
				"b_read", // invalid character: _
				"em@il", // invalid character: @
				"one+two", // invalid character: +
				"wrong*format", // invalid character: *
				"true||false", // invalid character: |
				"one day", // invalid character: " "
				"éclaire" // invalid character: é
			};
			bool result = any_of(tests.begin(), tests.end(), isValidName);
			assert(result == false);
		}

	};

	TEST_CLASS(splitString_defaultValues_Test) {
	public:
		TEST_METHOD(empty_success) {
			string input = "";
			vector<string> expected = {};
			vector<string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(allWhitespaces_success) {
			string input = " \n\t\r\v\f";
			vector<string> expected = {};
			vector<string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(sentence_success) {
			string input = "The brown   \n fox\t  is \r quick  \v for \f   its size.";
			vector<string> expected = { "The", "brown", "fox", "is", "quick", "for", "its", "size." };
			vector<string> output = splitString(input);
			assert(output == expected);
		}

		TEST_METHOD(excessiveWhitespaces_success) {
			string input = " \t\f\v\n\r\b \t\f\v\n\r\bThe \t\f\v\n\r\b \t\f\v\n\r\bbrown \t\f\v\n\r\b \t\f\v\n\r\bfox"
				" \t\f\v\n\r\bis \t\f\v\n\r\bquick \t\f\v\n\r\bfor \t\f\v\n\r\bits \t\f\v\n\r\bsize.";
			vector<string> expected = { "The", "brown", "fox", "is", "quick", "for", "its", "size." };
			vector<string> output = splitString(input);
			assert(output == expected);
		}
	};

	TEST_CLASS(splitString_customValues_Test) {
	public:
		TEST_METHOD(empty_success) {
			string input = "";
			vector<string> expected = {};
			vector<string> output = splitString(input, "a", true);
			assert(output == expected);
			vector<string> output2 = splitString(input, "a", false);
			assert(output2 == expected);
		}

		TEST_METHOD(allDelimiters_success) {
			string input = "aaaa";
			vector<string> expected = {};
			vector<string> output = splitString(input, "a", false);
			assert(output == expected);
			vector<string> expected2 = {"a", "a", "a", "a"};
			vector<string> output2 = splitString(input, "a", true);
			assert(output2 == expected2);
		}

		TEST_METHOD(sentence_success) {
			string input = "int x = 1;\nbool y = true;";
			vector<string> expected = { "int x = 1", "\nbool y = true" };
			vector<string> output = splitString(input, ";", false);
			assert(output == expected);
			vector<string> expected2 = { "int x = 1", ";", "\nbool y = true", ";" };
			vector<string> output2 = splitString(input, ";", true);
			assert(output2 == expected2);
		}

		TEST_METHOD(regex_success) {
			string input = "(( 1+2)-3)*4/5%6";
			vector<string> expected = {"1", "2", "3", "4", "5", "6"};
			vector<string> output = splitString(input, "([()+\\-/*%\\s])", false);
			vector<string> expected2 = {"(", "(", " ", "1", "+", "2", ")", "-", "3", ")", "*", "4", "/", "5", "%", "6"};
			vector<string> output2 = splitString(input, "([()+\\-/*%\\s])", true);
			assert(output == expected);
			assert(output2 == expected2);
		}
	};

	TEST_CLASS(trimWhiteSpaces_Test) {
	public:
		TEST_METHOD(nothing_success) {
			string input = "";
			string expected = "";
			string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(onlyWhitespaces_success) {
			string input = " \n\t\r\v\f\b";
			string expected = "";
			string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(sandwichedLetters_success) {
			string input = " \n\t\rabcd\v\f\b";
			string expected = "abcd";
			string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(sandwichedWhitespaces_success) {
			string input = "xy \n\t\r\v\f\bzw";
			string expected = "xy \n\t\r\v\f\bzw";
			string output = trimWhitespaces(input);
			assert(output == expected);
		}
		TEST_METHOD(stressTest_success) {
			string input = " \n This:\f a\vtest* of\b(the capabilitiés\f)\t \n";
			string expected = "This:\f a\vtest* of\b(the capabilitiés\f)";
			string output = trimWhitespaces(input);
			assert(output == expected);
		}
	};

	TEST_CLASS(substring_Test) {
		TEST_METHOD(validIndexes_success) {
			string input = "0123456";
			string output = substring(input, 3, 5);
			string expected = "345";
			assert(output == expected);
		}
		TEST_METHOD(endIndexMoreThanLastIndex_success) {
			string input = "0123456";
			string output = substring(input, 3, 100);
			string expected = "3456";
			assert(output == expected);
		}
		TEST_METHOD(negativeStartIndex_failure) {
			string input = "0123456";
			string output = substring(input, -1, 5);
			string expected = "012345";
			assert(output == expected);
		}
		TEST_METHOD(endIndexLarger_failure) {
			string input = "0123456";
			string output = substring(input, 3, 2);
			assert(output == "");
		}
	};
}