#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/PQLTokenizer.h"
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
			vector<string> test = PQLTokenizer::tokenize("stmt v; Select v");
			Assert::IsTrue(test[0] == "stmt");
			Assert::IsTrue(test[1] == "v");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "Select");
			Assert::IsTrue(test[4] == "v");
		}

		TEST_METHOD(TestTokenizeSuchThatQuery)
		{
			vector<string> test = PQLTokenizer::tokenize("variable v; Select v such that Uses(14, v)");
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
			vector<string> test = PQLTokenizer::tokenize("variable \t\f\v\n\r v \t\t \t\r \t\v \f\t \f\f \f\n \f\r;\
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
			vector<string> test = PQLTokenizer::tokenize("; Select Parent*(14, v) and Uses* (a, 5)");
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
			vector<string> test = PQLTokenizer::tokenize(" ; Select Parent*(_, _ ) such that Modifies* (_ , _)");
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
			vector<string> test = PQLTokenizer::tokenize("Select pattern a (_, _ ) and pattern b (__ , _)");
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
			vector<string> test = PQLTokenizer::tokenize("pattern a (\"v1\", _\"  1 + 2  \"_) and pattern b(\"!@#$\", \"a123 / 3\")");
			Assert::IsTrue(test[0] == "pattern");
			Assert::IsTrue(test[1] == "a");
			Assert::IsTrue(test[2] == "(");
			Assert::IsTrue(test[3] == "\"v1\"");
			Assert::IsTrue(test[4] == ",");
			Assert::IsTrue(test[5] == "_");
			Assert::IsTrue(test[6] == "\"1 + 2\"");
			Assert::IsTrue(test[7] == "_");
			Assert::IsTrue(test[8] == ")");
			Assert::IsTrue(test[9] == "and");
			Assert::IsTrue(test[10] == "pattern");
			Assert::IsTrue(test[11] == "b");
			Assert::IsTrue(test[12] == "(");
			Assert::IsTrue(test[13] == "\"!@#$\"");
			Assert::IsTrue(test[14] == ",");
			Assert::IsTrue(test[15] == "\"a123 / 3\"");
			Assert::IsTrue(test[16] == ")");
		}

		TEST_METHOD(TestTokenizeSelectSingleSynTuple)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <a>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "a");
			Assert::IsTrue(test[3] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 4);
		}

		TEST_METHOD(TestTokenizeSelectMultipleSynTuple)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <a, a1, b1234>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "a");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == "a1");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "b1234");
			Assert::IsTrue(test[7] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 8);
		}

		TEST_METHOD(TestTokenizeSelectMultipleSynTupleWithCont)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <a, a1, b1234> such that");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "a");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == "a1");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "b1234");
			Assert::IsTrue(test[7] == ">");
			Assert::IsTrue(test[8] == "such");
			Assert::IsTrue(test[9] == "that");
		}

		TEST_METHOD(TestTokenizeSelectInvalidSyntax)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <!,, a, | @");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "!");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == ",");
			Assert::IsTrue(test[5] == "a");
			Assert::IsTrue(test[6] == ",");
			Assert::IsTrue(test[7] == "|");
			Assert::IsTrue(test[8] == "@");
		}

		TEST_METHOD(TestTokenizeSelectInvalidSyntaxWithCont)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <!,, a, | @ pattern v");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "!");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == ",");
			Assert::IsTrue(test[5] == "a");
			Assert::IsTrue(test[6] == ",");
			Assert::IsTrue(test[7] == "|");
			Assert::IsTrue(test[8] == "@");
			// vector of tokens stops here, intended behaviour, tokenizer will not tokenize into a valid query, allowing parser to fail
		}

		TEST_METHOD(TestTokenizeSelectTupNoSpaceBeforeAfter)
		{
			vector<string> test = PQLTokenizer::tokenize("Select<a>with");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "a");
			Assert::IsTrue(test[3] == ">");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(static_cast<int>(test.size()) == 5);
		}

		TEST_METHOD(TestTokenizeWholeQuerySingleSynTuple)
		{
			vector<string> test = PQLTokenizer::tokenize("constant c; variable v; Select <c>");
			Assert::IsTrue(test[0] == "constant");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "variable");
			Assert::IsTrue(test[4] == "v");
			Assert::IsTrue(test[5] == ";");
			Assert::IsTrue(test[6] == "Select");
			Assert::IsTrue(test[7] == "<");
			Assert::IsTrue(test[8] == "c");
			Assert::IsTrue(test[9] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}

		TEST_METHOD(TestTokenizeWithprocNameEqualIdent)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c.procName with c.procName = \"ident\"");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ".");
			Assert::IsTrue(test[3] == "procName");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "procName");
			Assert::IsTrue(test[8] == "=");
			Assert::IsTrue(test[9] == "\"ident\"");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}

		TEST_METHOD(TestTokenizeWithvarNameEqualAttrRef)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c.procName with c.varName = a.procName");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ".");
			Assert::IsTrue(test[3] == "procName");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "varName");
			Assert::IsTrue(test[8] == "=");
			Assert::IsTrue(test[9] == "a");
			Assert::IsTrue(test[10] == ".");
			Assert::IsTrue(test[11] == "procName");
			Assert::IsTrue(static_cast<int>(test.size()) == 12);
		}

		TEST_METHOD(TestTokenizeWithValueEqualInt)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c.value with c.value = 234");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ".");
			Assert::IsTrue(test[3] == "value");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "value");
			Assert::IsTrue(test[8] == "=");
			Assert::IsTrue(test[9] == "234");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}

		TEST_METHOD(TestTokenizeWithStmtNoEqualInt)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c.# with c.stmt# = 234");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ".");
			Assert::IsTrue(test[3] == "#");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "stmt#");
			Assert::IsTrue(test[8] == "=");
			Assert::IsTrue(test[9] == "234");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}
		
		TEST_METHOD(TestTokenizeWithIdentEqualStmtNo)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c.whatever123# with \"idnet123\" = c.stmt#");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c");
			Assert::IsTrue(test[2] == ".");
			Assert::IsTrue(test[3] == "whatever123#");
			Assert::IsTrue(test[4] == "with");
			Assert::IsTrue(test[5] == "\"idnet123\"");
			Assert::IsTrue(test[6] == "=");
			Assert::IsTrue(test[7] == "c");
			Assert::IsTrue(test[8] == ".");
			Assert::IsTrue(test[9] == "stmt#");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}

		TEST_METHOD(TestTokenizeSelectSingleWithTup)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <c.whatever123#>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "c");
			Assert::IsTrue(test[3] == ".");
			Assert::IsTrue(test[4] == "whatever123#");
			Assert::IsTrue(test[5] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 6);
		}

		TEST_METHOD(TestTokenizeSelectDoubleWithTup)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <c.whatever123#, c.procName>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "c");
			Assert::IsTrue(test[3] == ".");
			Assert::IsTrue(test[4] == "whatever123#");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "c");
			Assert::IsTrue(test[7] == ".");
			Assert::IsTrue(test[8] == "procName");
			Assert::IsTrue(test[9] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 10);
		}

		TEST_METHOD(TestTokenizeSelectWithAndSynTup)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <c.whatever123#, c123>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "c");
			Assert::IsTrue(test[3] == ".");
			Assert::IsTrue(test[4] == "whatever123#");
			Assert::IsTrue(test[5] == ",");
			Assert::IsTrue(test[6] == "c123");
			Assert::IsTrue(test[7] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 8);
		}

		TEST_METHOD(TestTokenizeSelectSynAndWithTup)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <c123, c.whatever123#>");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "c123");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == "c");
			Assert::IsTrue(test[5] == ".");
			Assert::IsTrue(test[6] == "whatever123#");
			Assert::IsTrue(test[7] == ">");
			Assert::IsTrue(static_cast<int>(test.size()) == 8);
		}

		TEST_METHOD(TestTokenizeTupWith)
		{
			vector<string> test = PQLTokenizer::tokenize("Select <c123, c.whatever123#> with \"ident\" = c.stmt#");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "<");
			Assert::IsTrue(test[2] == "c123");
			Assert::IsTrue(test[3] == ",");
			Assert::IsTrue(test[4] == "c");
			Assert::IsTrue(test[5] == ".");
			Assert::IsTrue(test[6] == "whatever123#");
			Assert::IsTrue(test[7] == ">");
			Assert::IsTrue(test[8] == "with");
			Assert::IsTrue(test[9] == "\"ident\"");
			Assert::IsTrue(test[10] == "=");
			Assert::IsTrue(test[11] == "c");
			Assert::IsTrue(test[12] == ".");
			Assert::IsTrue(test[13] == "stmt#");
			Assert::IsTrue(static_cast<int>(test.size()) == 14);
		}

		TEST_METHOD(TestTokenizeSynWith)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c123 with \"ident\" = c.stmt#");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c123");
			Assert::IsTrue(test[2] == "with");
			Assert::IsTrue(test[3] == "\"ident\"");
			Assert::IsTrue(test[4] == "=");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "stmt#");
			Assert::IsTrue(static_cast<int>(test.size()) == 8);
		}

		TEST_METHOD(TestTokenizeSynSuchThatWith)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c123 such that Calls*(a, b) with \"ident\" = c.stmt#");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c123");
			Assert::IsTrue(test[2] == "such");
			Assert::IsTrue(test[3] == "that");
			Assert::IsTrue(test[4] == "Calls*");
			Assert::IsTrue(test[5] == "(");
			Assert::IsTrue(test[6] == "a");
			Assert::IsTrue(test[7] == ",");
			Assert::IsTrue(test[8] == "b");
			Assert::IsTrue(test[9] == ")");
			Assert::IsTrue(test[10] == "with");
			Assert::IsTrue(test[11] == "\"ident\"");
			Assert::IsTrue(test[12] == "=");
			Assert::IsTrue(test[13] == "c");
			Assert::IsTrue(test[14] == ".");
			Assert::IsTrue(test[15] == "stmt#");
			Assert::IsTrue(static_cast<int>(test.size()) == 16);
		}

		TEST_METHOD(TestTokenizeSynWithSuchThat)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c123 with \"ident\" = c.stmt# such that Calls*(a, b)");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c123");
			Assert::IsTrue(test[2] == "with");
			Assert::IsTrue(test[3] == "\"ident\"");
			Assert::IsTrue(test[4] == "=");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "stmt#");
			Assert::IsTrue(test[8] == "such");
			Assert::IsTrue(test[9] == "that");
			Assert::IsTrue(test[10] == "Calls*");
			Assert::IsTrue(test[11] == "(");
			Assert::IsTrue(test[12] == "a");
			Assert::IsTrue(test[13] == ",");
			Assert::IsTrue(test[14] == "b");
			Assert::IsTrue(test[15] == ")");
			Assert::IsTrue(static_cast<int>(test.size()) == 16);
		}

		TEST_METHOD(TestTokenizeSynPatternWith)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c123 pattern a (13, \"1+3\") with \"ident\" = c.stmt#");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c123");
			Assert::IsTrue(test[2] == "pattern");
			Assert::IsTrue(test[3] == "a");
			Assert::IsTrue(test[4] == "(");
			Assert::IsTrue(test[5] == "13");
			Assert::IsTrue(test[6] == ",");
			Assert::IsTrue(test[7] == "\"1+3\"");
			Assert::IsTrue(test[8] == ")");
			Assert::IsTrue(test[9] == "with");
			Assert::IsTrue(test[10] == "\"ident\"");
			Assert::IsTrue(test[11] == "=");
			Assert::IsTrue(test[12] == "c");
			Assert::IsTrue(test[13] == ".");
			Assert::IsTrue(test[14] == "stmt#");
			Assert::IsTrue(static_cast<int>(test.size()) == 15);
		}

		TEST_METHOD(TestTokenizeSynWithPattern)
		{
			vector<string> test = PQLTokenizer::tokenize("Select c123 with \"ident\" = c.stmt# pattern a (_, \"1+3\")");
			Assert::IsTrue(test[0] == "Select");
			Assert::IsTrue(test[1] == "c123");
			Assert::IsTrue(test[2] == "with");
			Assert::IsTrue(test[3] == "\"ident\"");
			Assert::IsTrue(test[4] == "=");
			Assert::IsTrue(test[5] == "c");
			Assert::IsTrue(test[6] == ".");
			Assert::IsTrue(test[7] == "stmt#");
			Assert::IsTrue(test[8] == "pattern");
			Assert::IsTrue(test[9] == "a");
			Assert::IsTrue(test[10] == "(");
			Assert::IsTrue(test[11] == "_");
			Assert::IsTrue(test[12] == ",");
			Assert::IsTrue(test[13] == "\"1+3\"");
			Assert::IsTrue(test[14] == ")");

			Assert::IsTrue(static_cast<int>(test.size()) == 15);
		}




			TEST_METHOD(TestTokenizeValidQueryWhiteSpaces)
		{
			vector<string> test = PQLTokenizer::tokenize("stmt s    ;    read    r  ;    print   pn   ;    call  cl; while w; if i; assign a; variable v; constant c; procedure p;         Select    \t\f\v\n\r\b           pn                     such                     that               Follows                                     (                1      ,     2     )       ");
			Assert::IsTrue(test[0] == "stmt");
			Assert::IsTrue(test[1] == "s");
			Assert::IsTrue(test[2] == ";");
			Assert::IsTrue(test[3] == "read");
			Assert::IsTrue(test[4] == "r");
			Assert::IsTrue(test[5] == ";");
			Assert::IsTrue(test[6] == "print");
			Assert::IsTrue(test[7] == "pn");
			Assert::IsTrue(test[8] == ";");
			Assert::IsTrue(test[9] == "call");
			Assert::IsTrue(test[10] == "cl");
			Assert::IsTrue(test[11] == ";");
			Assert::IsTrue(test[12] == "while");
			Assert::IsTrue(test[13] == "w");
			Assert::IsTrue(test[14] == ";");
			Assert::IsTrue(test[15] == "if");
			Assert::IsTrue(test[16] == "i");
			Assert::IsTrue(test[17] == ";");
			Assert::IsTrue(test[18] == "assign");
			Assert::IsTrue(test[19] == "a");
			Assert::IsTrue(test[20] == ";");
			Assert::IsTrue(test[21] == "variable");
			Assert::IsTrue(test[22] == "v");
			Assert::IsTrue(test[23] == ";");
			Assert::IsTrue(test[24] == "constant");
			Assert::IsTrue(test[25] == "c");
			Assert::IsTrue(test[26] == ";");
			Assert::IsTrue(test[27] == "procedure");
			Assert::IsTrue(test[28] == "p");
			Assert::IsTrue(test[29] == ";");
			Assert::IsTrue(test[30] == "Select");
			Assert::IsTrue(test[31] == "pn");
			Assert::IsTrue(test[32] == "such");
			Assert::IsTrue(test[33] == "that");
			Assert::IsTrue(test[34] == "Follows");
			Assert::IsTrue(test[35] == "(");
			Assert::IsTrue(test[36] == "1");
			Assert::IsTrue(test[37] == ",");
			Assert::IsTrue(test[38] == "2");
			Assert::IsTrue(test[39] == ")");

			Assert::IsTrue(static_cast<int>(test.size()) == 40);
		}

		TEST_METHOD(TestTokenizePatternSpaceInIdent)
			{
				vector<string> test = PQLTokenizer::tokenize("Select a pattern a (\"te st\",_)");
				Assert::IsTrue(test[0] == "Select");
				Assert::IsTrue(test[1] == "a");
				Assert::IsTrue(test[2] == "pattern");
				Assert::IsTrue(test[3] == "a");
				Assert::IsTrue(test[4] == "(");
				Assert::IsTrue(test[5] == "\"te st\"");
				Assert::IsTrue(test[6] == ",");
				Assert::IsTrue(test[7] == "_");
				Assert::IsTrue(test[8] == ")");

				Assert::IsTrue(static_cast<int>(test.size()) == 9);
			}
	};
}