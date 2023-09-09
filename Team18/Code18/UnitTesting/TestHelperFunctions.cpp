#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/HelperFunctions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace HelperFunctions_Test
{
	TEST_CLASS(isNumber_Test) {
	public:
		TEST_METHOD(numbers_success) {
			bool result = isNumber("01234");
			assert(result == true);
		}

		TEST_METHOD(nonNumbers_failure) {
			bool result = isNumber("0x1");
			result = result || isNumber("0b1");
			result = result || isNumber("0b1");
			result = result || isNumber("0abc");
			result = result || isNumber("abc0");
			result = result || isNumber(" ");
			result = result || isNumber("%");
			result = result || isNumber("|");
			result = result || isNumber("é");
			assert(result == false);
		}
	};

	TEST_CLASS(isAlphanumeric_Test) {
	public:
		TEST_METHOD(alphanum_success) {
			bool result = isAlphanumeric("abc");
			result == result && isAlphanumeric("123");
			result == result && isAlphanumeric("x4y5z6");
			result == result && isAlphanumeric("0x1");
			result == result && isAlphanumeric("0b1");
			assert(result == true);
		}

		TEST_METHOD(nonAlphanum_failure) {
			bool result = isAlphanumeric(" ");
			result = result || isAlphanumeric("|");
			result = result || isAlphanumeric("é");
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
			bool result = isValidName("0123");
			result = result || isValidName("9");
			result = result || isValidName("10");
			result = result || isValidName("0x1");
			result = result || isValidName("0b1");
			assert(result == false); // failure
		}

		TEST_METHOD(leadingDigit_failure) {
			bool result = isValidName("0read");
			result = result || isValidName("10read");
			result = result || isValidName("0x1read");
			result = result || isValidName("0b1read");
			assert(result == false); // failure
		}

		TEST_METHOD(invalidCharacter_failure) {
			bool result = isValidName("b_read"); // invalid character: _
			result = result || isValidName("em@il"); // invalid character: @
			result = result || isValidName("one+two"); // invalid character: +
			result = result || isValidName("wrong*format"); // invalid character: *
			result = result || isValidName("true||false"); // invalid character: |
			result = result || isValidName("one day"); // invalid character: " "
			result = result || isValidName("éclaire"); // invalid character: é
			assert(result == false); // failure
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
}