 #include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include "../source/QPS/Errors/QPSError.h"
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
			vector<string> test = tokenize("stmt v; Select v");
			vector<string_view> testSv{ sToSvVector(test) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
			vector<string_view> declarations = get<0>(testObj);
			vector<string_view> query = get<1>(testObj);
			Assert::IsTrue("stmt" == declarations[0]);
			Assert::IsTrue("v" == declarations[1]);
			Assert::IsTrue("Select" == query[0]);
			Assert::IsTrue("v" == query[1]);
			
		}

		TEST_METHOD(TestSplitDeclarationQueryEmptyDeclaration)
		{
			vector<string> test = tokenize("Select v");
			vector<string_view> testSv{ sToSvVector(test) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			try
			{
				tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual(ex.what(), "No Declaration clause found");
			}
		}

		TEST_METHOD(TestSplitDeclarationQueryEmptyQuery)
		{
			vector<string> test = tokenize("stmt v;");
			vector<string_view> testSv{ sToSvVector(test) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			try
			{
				tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual(ex.what(), "No Query or Declaration clause found");
			}

		}

		TEST_METHOD(TestValidateDeclarationWeirdSpacing)
		{
			vector<string> tokenizer = tokenize("  stmt s1,s2,   s3; variable  variable; assign a  ; Select v  ");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
		}

		TEST_METHOD(TestValidateDeclarationInvalidSynonym)
		{
			vector<string> tokenizer = tokenize("stmt dasd!; variable variable; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);
			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("Invalid synonym", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationInvalidDeclaration)
		{
			vector<string> tokenizer = tokenize("stmt; variable variable; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("Invalid declaration", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationInvalidDesign)
		{
			vector<string> tokenizer = tokenize("stmt12 ab; variable variable; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };

			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("Invalid string token for design object", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationNoComma)
		{
			vector<string> tokenizer = tokenize("stmt ab variable variable; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);
		
			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("Missing comma", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationExtraComma)
		{
			vector<string> tokenizer = tokenize("stmt ab, variable, v1,; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);
			
			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("SyntaxError", ex.getType());
				Assert::AreEqual("Extra comma", ex.what());
			}

		}

		TEST_METHOD(TestValidateDeclarationRepeatSynonym)
		{
			vector<string> tokenizer = tokenize("stmt a, variable, v1; assign a; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			try {
				vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(declarationQuery));
				Assert::Fail();
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual("Repeated synonym declaration", ex.what());
			}

		}

		TEST_METHOD(TestValidStmtDeclaration)
		{
			vector<string> tokenizer = tokenize("stmt a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");
			
		}

		TEST_METHOD(TestValidReadDeclaration)
		{
			vector<string> tokenizer = tokenize("read a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidPrintDeclaration)
		{
			vector<string> tokenizer = tokenize("print a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidCallDeclaration)
		{
			vector<string> tokenizer = tokenize("call a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidWhileDeclaration)
		{
			vector<string> tokenizer = tokenize("while a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidIfDeclaration)
		{
			vector<string> tokenizer = tokenize("if a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidAssignDeclaration)
		{
			vector<string> tokenizer = tokenize("assign a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidVariableDeclaration)
		{
			vector<string> tokenizer = tokenize("variable a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidConstantDeclaration)
		{
			vector<string> tokenizer = tokenize("constant a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}

		TEST_METHOD(TestValidProcedureDeclaration)
		{
			vector<string> tokenizer = tokenize("procedure a, a1; Select v");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> declarationQuery = p->splitDeclarationQuery(testSv);

			vector<shared_ptr<QueryObject>> declarations = p->validateDeclaration(get<0>(declarationQuery));
			Assert::IsTrue(declarations[0]->getQueryObjectName() == "a");
			Assert::IsTrue(declarations[1]->getQueryObjectName() == "a1");

		}


	};
}