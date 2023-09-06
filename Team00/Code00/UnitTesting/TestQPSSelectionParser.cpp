#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/DesignObjectsFactory.h"
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
			vector<string_view> test = tokenize("stmt v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			StmtObjectFactory factory = StmtObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectReadQuery)
		{
			vector<string_view> test = tokenize("read v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			ReadObjectFactory factory = ReadObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectPrintQuery)
		{
			vector<string_view> test = tokenize("print v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			PrintObjectFactory factory = PrintObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectCallQuery)
		{
			vector<string_view> test = tokenize("call v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			CallObjectFactory factory = CallObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectWhileQuery)
		{
			vector<string_view> test = tokenize("while v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			CallObjectFactory factory = CallObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectIfQuery)
		{
			vector<string_view> test = tokenize("if v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			IfObjectFactory factory = IfObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectAssignQuery)
		{
			vector<string_view> test = tokenize("assign v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			AssignObjectFactory factory = AssignObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectVariableQuery)
		{
			vector<string_view> test = tokenize("variable v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			VariableObjectFactory factory = VariableObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectConstantQuery)
		{
			vector<string_view> test = tokenize("constant v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			ConstantObjectFactory factory = ConstantObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

		TEST_METHOD(TestValidSelectProcedureQuery)
		{
			vector<string_view> test = tokenize("procedure v; Select v");
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<vector<string_view>> splittedDeclarations = p->splitDeclarations(std::get<0>(testObj));
			vector<shared_ptr<QueryObject>> decObjs = p->validateDeclaration(splittedDeclarations[0]);
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			ProcedureObjectFactory factory = ProcedureObjectFactory();
			Assert::IsTrue(qo[0] == factory.create("v"sv));
		}

	};
}