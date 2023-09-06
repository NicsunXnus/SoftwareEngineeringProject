#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSDeclarationParser)
	{
	public:


		TEST_METHOD(TestSplitDeclarationQuery)
		{
			vector<string_view> test = tokenize("stmt v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
		}

		TEST_METHOD(TestSplitDeclarationQueryEmptyDeclaration)
		{
			vector<string_view> test = tokenize("Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			try
			{
				tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual(ex.what(), "No Declaration clause found");
			}
		}

		TEST_METHOD(TestSplitDeclarationQueryEmptyQuery)
		{
			vector<string_view> test = tokenize("stmt v;");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			try
			{
				tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual(ex.what(), "No Query or Declaration clause found");
			}

		}

		TEST_METHOD(TestSplitDeclarations)
		{
			vector<string_view> test = tokenize("    stmt s, s2,     s3; variable  v  ; assign   a  ; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(testObj));
			Assert::IsTrue(splittedDeclarations.size() == 3);

			Assert::IsTrue(splittedDeclarations[0][0] == "stmt");
			Assert::IsTrue(splittedDeclarations[0][1] == "s");
			Assert::IsTrue(splittedDeclarations[0][2] == ",");
			Assert::IsTrue(splittedDeclarations[0][3] == "s2");
			Assert::IsTrue(splittedDeclarations[0][4] == ",");
			Assert::IsTrue(splittedDeclarations[0][5] == "s3");
			Assert::IsTrue(splittedDeclarations[1][0] == "variable");
			Assert::IsTrue(splittedDeclarations[1][1] == "v");
			Assert::IsTrue(splittedDeclarations[2][0] == "assign");
			Assert::IsTrue(splittedDeclarations[2][1] == "a");

		}

		TEST_METHOD(TestValidateDeclaration)
		{
			vector<string_view> tokenizer = tokenize("  stmt s1,s2,   s3; variable  variable; assign a  ; Select v  ");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			Assert::IsTrue(splittedDeclarations.size() == 3);
			vector<vector<shared_ptr<QueryObject>>> res;
			for (int i = 0; i < splittedDeclarations.size(); i++) {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
				res.push_back(curr);

			}
			int tes1t = 1;
		}

		TEST_METHOD(TestValidateDeclarationInvalidSynonym)
		{
			vector<string_view> tokenizer = tokenize("stmt dasd!; variable variable; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			Assert::IsTrue(splittedDeclarations.size() == 3);
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Invalid synonym", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationInvalidDeclaration)
		{
			vector<string_view> tokenizer = tokenize("stmt; variable variable; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			Assert::IsTrue(splittedDeclarations.size() == 3);
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Invalid declaration", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationInvalidDesign)
		{
			vector<string_view> tokenizer = tokenize("stmt12 ab; variable variable; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			Assert::IsTrue(splittedDeclarations.size() == 3);
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Invalid string token for design object", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationNoComma)
		{
			vector<string_view> tokenizer = tokenize("stmt ab variable variable; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Missing comma", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationExtraComma)
		{
			vector<string_view> tokenizer = tokenize("stmt ab, variable, v1,; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Extra comma", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationRepeatSynonym)
		{
			vector<string_view> tokenizer = tokenize("stmt a, variable, v1; assign a; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(tokenizer);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(get<0>(declarationQuery));
			vector<vector<shared_ptr<QueryObject>>> res;
			try {
				for (int i = 0; i < splittedDeclarations.size(); i++) {
					vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(splittedDeclarations[i]);
					res.push_back(curr);

				}
				Assert::Fail();
			}
			catch (const exception& ex)
			{
				Assert::AreEqual("Repeated synonym declaration", ex.what());
			}

		}


	};
}