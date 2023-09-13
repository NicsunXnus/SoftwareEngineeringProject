#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSSelectionParser)
	{
	public:

		TEST_METHOD(TestValidSelectStmtQuery)
		{
			vector<string> testS = tokenize("stmt v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			string_view sv = qo[0]->getQueryObjectName();

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectReadQuery)
		{
			vector<string> testS = tokenize("variable c; read v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectPrintQuery)
		{
			vector<string> testS = tokenize("print p, v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectCallQuery)
		{
			vector<string> testS = tokenize("call v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectWhileQuery)
		{
			vector<string> testS = tokenize("while v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectIfQuery)
		{
			vector<string> testS = tokenize("if v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectAssignQuery)
		{
			vector<string> testS = tokenize("assign v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectVariableQuery)
		{
			vector<string> testS = tokenize("variable v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectConstantQuery)
		{
			vector<string> testS = tokenize("constant v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectProcedureQuery)
		{
			vector<string> testS = tokenize("procedure v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

	};
}