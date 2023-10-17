#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/PQLTokenizer.h"
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
			vector<string> test = tokenize("stmt v; Select v");
			Assert::IsTrue(test[0] == "stmt");
			Assert::IsTrue(test[1] == "v");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "Select");
			Assert::IsTrue(test[4] == "v");
		}

		TEST_METHOD(TestTokenizeSuchThatQuery)
		{
			vector<string> test = tokenize("variable v; Select v such that Uses(14, v)");
			Assert::IsTrue(test[0] == "variable");
			Assert::IsTrue(test[1] == "v");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "Select");
			Assert::IsTrue(test[4] == "v");
			Assert::IsTrue(test[5] == "such");
			Assert::IsTrue(test[6] == "that");
			Assert::IsTrue(test[7] == "Uses");
			Assert::IsTrue(test[8] == "(");
			Assert::IsTrue(test[9] == "14");
			Assert::IsTrue(test[10] == ",");
			Assert::IsTrue(test[11] == "v");
			Assert::IsTrue(test[12] == ")");
		}

		TEST_METHOD(TestTokenizeSuchThatQueryWithWhiteSpace)
		{
			vector<string> test = tokenize("variable \t\f\v\n\r v \t\t \t\r \t\v \f\t \f\f \f\n \f\r;\
				\v\t \v\f \v\v \v\r Select \n\t \n\f \n\v \n\n  v \r\t \r\f \r\v \r\n \r\r  such  that  Uses (14, v)   ");

			Assert::IsTrue(test[0] == "variable");
			Assert::IsTrue(test[1] == "v");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "Select");
			Assert::IsTrue(test[4] == "v");
			Assert::IsTrue(test[5] == "such");
			Assert::IsTrue(test[6] == "that");
			Assert::IsTrue(test[7] == "Uses");
			Assert::IsTrue(test[8] == "(");
			Assert::IsTrue(test[9] == "14");
			Assert::IsTrue(test[10] == ",");
			Assert::IsTrue(test[11] == "v");
			Assert::IsTrue(test[12] == ")");
		}

		TEST_METHOD(TestTokenizeStarQuery)
		{
			vector<string> test = tokenize("; Select Parent*(14, v) and Uses* (a, 5)");
			Assert::IsTrue(test[0] == ";");
			Assert::IsTrue(test[1] == "Select");
			Assert::IsTrue(test[2] == "Parent*");
			Assert::IsTrue(test[3] == "(");
			Assert::IsTrue(test[4] == "14");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "v");
			Assert::IsTrue(test[7] == ")");
			Assert::IsTrue(test[8] == "and");
			Assert::IsTrue(test[9] == "Uses*");
			Assert::IsTrue(test[10] == "(");
			Assert::IsTrue(test[11] == "a");
			Assert::IsTrue(test[12] == ",");
			Assert::IsTrue(test[13] == "5");
			Assert::IsTrue(test[14] == ")");
		}

		TEST_METHOD(TestTokenizeWildcardQuery)
		{
			vector<string> test = tokenize(" ; Select Parent*(_, _ ) such that Modifies* (_ , _)");
			Assert::IsTrue(test[0] == ";");
			Assert::IsTrue(test[1] == "Select");
			Assert::IsTrue(test[2] == "Parent*");
			Assert::IsTrue(test[3] == "(");
			Assert::IsTrue(test[4] == "_");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "_");
			Assert::IsTrue(test[7] == ")");
			Assert::IsTrue(test[8] == "such");
			Assert::IsTrue(test[9] == "that");
			Assert::IsTrue(test[10] == "Modifies*");
			Assert::IsTrue(test[11] == "(");
			Assert::IsTrue(test[12] == "_");
			Assert::IsTrue(test[13] == ",");
			Assert::IsTrue(test[14] == "_");
			Assert::IsTrue(test[15] == ")");
		}

		TEST_METHOD(TestTokenizePatternQuery)
		{
			vector<string> test = tokenize("Select pattern a (_, _ ) and pattern b (__ , _)");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "pattern");
			Assert::IsTrue(test[2] == "a");
			Assert::IsTrue(test[3] == "(");
			Assert::IsTrue(test[4] == "_");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "_");
			Assert::IsTrue(test[7] == ")");
			Assert::IsTrue(test[8] == "and");
			Assert::IsTrue(test[9] == "pattern");
			Assert::IsTrue(test[10] == "b");
			Assert::IsTrue(test[11] == "(");
			Assert::IsTrue(test[12] == "_");
			Assert::IsTrue(test[13] == "_");
			Assert::IsTrue(test[14] == ",");
			Assert::IsTrue(test[15] == "_");
			Assert::IsTrue(test[16] == ")");
		}

		TEST_METHOD(TestTokenizePatternQueryWithQuotes)
		{
			vector<string> test = tokenize("pattern a (\"v1\", _\"  1 + 2  \"_) and pattern b(\"!@#$\", \"a123 / 3\")");
			Assert::IsTrue(test[0] == "pattern");
			Assert::IsTrue(test[1] == "a");
			Assert::IsTrue(test[2] == "(");
			Assert::IsTrue(test[3] == "\"v1\"");
			Assert::IsTrue(test[4] == ",");
			Assert::IsTrue(test[5] == "_");
			Assert::IsTrue(test[6] == "\"1+2\"");
			Assert::IsTrue(test[7] == "_");
			Assert::IsTrue(test[8] == ")");
			Assert::IsTrue(test[9] == "and");
			Assert::IsTrue(test[10] == "pattern");
			Assert::IsTrue(test[11] == "b");
			Assert::IsTrue(test[12] == "(");
			Assert::IsTrue(test[13] == "\"!@#$\"");
			Assert::IsTrue(test[14] == ",");
			Assert::IsTrue(test[15] == "\"a123/3\"");
			Assert::IsTrue(test[16] == ")");
		}
	};
}