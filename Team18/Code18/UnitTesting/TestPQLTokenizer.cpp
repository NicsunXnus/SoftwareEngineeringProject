#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include <string_view>
#include <vector>
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestPQLTokenizer)
	{
	public:
		TEST_METHOD(TestTokenizeSelectQuery)
		{
			vector<string_view> test = tokenize("stmt v; Select v");
			Assert::IsTrue(test[0] == "stmt"sv);
			Assert::IsTrue(test[1] == "v"sv);
			Assert::IsTrue(test[2] == ";"sv);
			Assert::IsTrue(test[3] == "Select"sv);
			Assert::IsTrue(test[4] == "v"sv);
		}

		TEST_METHOD(TestTokenizeSuchThatQuery)
		{
			vector<string_view> test = tokenize("variable v; Select v such that Uses(14, v)");
			Assert::IsTrue(test[0] == "variable"sv);
			Assert::IsTrue(test[1] == "v"sv);
			Assert::IsTrue(test[2] == ";"sv);
			Assert::IsTrue(test[3] == "Select"sv);
			Assert::IsTrue(test[4] == "v"sv);
			Assert::IsTrue(test[5] == "such"sv);
			Assert::IsTrue(test[6] == "that"sv);
			Assert::IsTrue(test[7] == "Uses"sv);
			Assert::IsTrue(test[8] == "("sv);
			Assert::IsTrue(test[9] == "14"sv);
			Assert::IsTrue(test[10] == ","sv);
			Assert::IsTrue(test[11] == "v"sv);
			Assert::IsTrue(test[12] == ")"sv);
		}

		TEST_METHOD(TestTokenizeSuchThatQueryWithWhiteSpace)
		{
			vector<string_view> test = tokenize("variable \t\f\v\n\r v \t\t \t\r \t\v \f\t \f\f \f\n \f\r;\
				\v\t \v\f \v\v \v\r Select \n\t \n\f \n\v \n\n  v \r\t \r\f \r\v \r\n \r\r  such  that  Uses (14, v)   ");

			Assert::IsTrue(test[0] == "variable"sv);
			Assert::IsTrue(test[1] == "v"sv);
			Assert::IsTrue(test[2] == ";"sv);
			Assert::IsTrue(test[3] == "Select"sv);
			Assert::IsTrue(test[4] == "v"sv);
			Assert::IsTrue(test[5] == "such"sv);
			Assert::IsTrue(test[6] == "that"sv);
			Assert::IsTrue(test[7] == "Uses"sv);
			Assert::IsTrue(test[8] == "("sv);
			Assert::IsTrue(test[9] == "14"sv);
			Assert::IsTrue(test[10] == ","sv);
			Assert::IsTrue(test[11] == "v"sv);
			Assert::IsTrue(test[12] == ")"sv);
		}

		TEST_METHOD(TestTokenizeStarQuery)
		{
			vector<string_view> test = tokenize("; Select Parent*(14, v) and Uses* (a, 5)");
			Assert::IsTrue(test[0] == ";"sv);
			Assert::IsTrue(test[1] == "Select"sv);
			Assert::IsTrue(test[2] == "Parent*"sv);
			Assert::IsTrue(test[3] == "("sv);
			Assert::IsTrue(test[4] == "14"sv);
			Assert::IsTrue(test[5] == ","sv);
			Assert::IsTrue(test[6] == "v"sv);
			Assert::IsTrue(test[7] == ")"sv);
			Assert::IsTrue(test[8] == "and"sv);
			Assert::IsTrue(test[9] == "Uses*"sv);
			Assert::IsTrue(test[10] == "("sv);
			Assert::IsTrue(test[11] == "a"sv);
			Assert::IsTrue(test[12] == ","sv);
			Assert::IsTrue(test[13] == "5"sv);
			Assert::IsTrue(test[14] == ")"sv);
		}

		TEST_METHOD(TestTokenizeWildcardQuery)
		{
			vector<string_view> test = tokenize(" ; Select Parent*(_, _ ) such that Modifies* (_ , _)");
			Assert::IsTrue(test[0] == ";"sv);
			Assert::IsTrue(test[1] == "Select"sv);
			Assert::IsTrue(test[2] == "Parent*"sv);
			Assert::IsTrue(test[3] == "("sv);
			Assert::IsTrue(test[4] == "_"sv);
			Assert::IsTrue(test[5] == ","sv);
			Assert::IsTrue(test[6] == "_"sv);
			Assert::IsTrue(test[7] == ")"sv);
			Assert::IsTrue(test[8] == "such"sv);
			Assert::IsTrue(test[9] == "that"sv);
			Assert::IsTrue(test[10] == "Modifies*"sv);
			Assert::IsTrue(test[11] == "("sv);
			Assert::IsTrue(test[12] == "_"sv);
			Assert::IsTrue(test[13] == ","sv);
			Assert::IsTrue(test[14] == "_"sv);
			Assert::IsTrue(test[15] == ")"sv);
		}
	};
}