#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/QPS/QueryObjects/PatternClauseObject.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSErrorQueries)
	{
	public:
		TEST_METHOD(TestSemanticAndSyntaxError)
		{
			vector<string> tokenizer = tokenize("assign a; variable v; constant c; Select a such that Uses (a, c) pattern a (\"8\", _)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);

			try {
				vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("SyntaxError", ex.getType());
			}
		}

		TEST_METHOD(TestFollowsClauseWithIdent1stArg)
		{
			vector<string> tokenizer = tokenize("stmt s, s1; Select s1 such that Follows (\"s\", s1)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();

			try {
				vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("SyntaxError", ex.getType());
			}
		}

		TEST_METHOD(TestFollowsClauseWithIdent2ndArg)
		{
			vector<string> tokenizer = tokenize("stmt s, s1; Select s1 such that Follows (s, \"s1\")");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();

			try {
				vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("SyntaxError", ex.getType());
			}
		}

		TEST_METHOD(TestEmptyDeclaration)
		{
			vector<string> tokenizer = tokenize("Select s");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();

			try {
				vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("SemanticError", ex.getType());
			}
		}
	};
}