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
			vector<string_view> test = tokenize("variable v;  Select  v  such  that   Uses    (  14   ,   v   )   ");
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

	};
}