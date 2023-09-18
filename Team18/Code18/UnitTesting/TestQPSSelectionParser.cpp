#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
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

		TEST_METHOD(TestValidSelectUsesStmtQuery)
		{
			vector<string> testS = tokenize("assign a; variable v; Select v such that Uses(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(UsesObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<UsesObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Uses"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("variable v; Select v such that Uses(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(UsesObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<UsesObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Uses"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectUsesEntQuery)
		{
			vector<string> testEntRefSyn = tokenize("call c; variable v; Select v such that Uses(c, v)");
			vector<string_view> testEntRefSynSV{ sToSvVector(testEntRefSyn) };
			shared_ptr<QueryParser> p1 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefSynObj = p1->splitDeclarationQuery(testEntRefSynSV);
			vector<shared_ptr<QueryObject>> decs1 = p1->validateDeclaration(get<0>(testEntRefSynObj));
			vector<shared_ptr<QueryObject>> qo1 = p1->validateQuery(std::get<1>(testEntRefSynObj));

			const std::type_info& t1 = typeid(*qo1[1]);
			const std::type_info& t2 = typeid(UsesObject);

			Assert::IsTrue(typeid(*qo1[1]) == typeid(UsesEntityObject));

			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<UsesEntityObject>(qo1[1]);
			Assert::IsTrue(qo1[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co1->getQueryObjectName() == "Uses"sv
				&& co1->getArg1()->getArg() == "c"sv
				&& co1->getArg2()->getArg() == "v"sv);

			vector<string> testEntRefIdent = tokenize("variable v; Select v such that Uses(\"procName\", v)");
			vector<string_view> testEntRefIdentSV{ sToSvVector(testEntRefIdent) };
			shared_ptr<QueryParser> p2 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefIdentObj = p2->splitDeclarationQuery(testEntRefIdentSV);
			vector<shared_ptr<QueryObject>> decs2 = p2->validateDeclaration(get<0>(testEntRefIdentObj));
			vector<shared_ptr<QueryObject>> qo2 = p2->validateQuery(std::get<1>(testEntRefIdentObj));

			Assert::IsTrue(typeid(*qo2[1]) == typeid(UsesEntityObject));

			std::shared_ptr<ClauseObject> co2 = std::static_pointer_cast<UsesEntityObject>(qo2[1]);
			Assert::IsTrue(qo2[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co2->getQueryObjectName() == "Uses"sv
				&& co2->getArg1()->getArg() == "\"procName\""sv
				&& co2->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestInvalidSelectUsesEntQuery)
		{
			vector<string> testEntRefSyn = tokenize("call c; read v; Select v such that Uses(c, v)");
			vector<string_view> testEntRefSynSV{ sToSvVector(testEntRefSyn) };
			shared_ptr<QueryParser> p1 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefSynObj = p1->splitDeclarationQuery(testEntRefSynSV);
			vector<shared_ptr<QueryObject>> decs1 = p1->validateDeclaration(get<0>(testEntRefSynObj));
			try {
				vector<shared_ptr<QueryObject>> qo1 = p1->validateQuery(std::get<1>(testEntRefSynObj));
				std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<UsesObject>(qo1[1]);
				cout << co1->getArg2() << std::endl;
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual(ex.what(), "Semantic error: 2nd argument is not a variable entity");
			}
			
		}
		TEST_METHOD(TestValidSelectModifiesStmtQuery)
		{
			vector<string> testS = tokenize("assign a; variable v; Select v such that Modifies(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(ModifiesObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<ModifiesObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Modifies"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("variable v; Select v such that Modifies(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(ModifiesObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<ModifiesObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Modifies"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectModifiesEntQuery)
		{
			vector<string> testEntRefSyn = tokenize("call c; variable v; Select v such that Modifies(c, v)");
			vector<string_view> testEntRefSynSV{ sToSvVector(testEntRefSyn) };
			shared_ptr<QueryParser> p1 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefSynObj = p1->splitDeclarationQuery(testEntRefSynSV);
			vector<shared_ptr<QueryObject>> decs1 = p1->validateDeclaration(get<0>(testEntRefSynObj));
			vector<shared_ptr<QueryObject>> qo1 = p1->validateQuery(std::get<1>(testEntRefSynObj));


			Assert::IsTrue(typeid(*qo1[1]) == typeid(ModifiesEntityObject));

			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<ModifiesEntityObject>(qo1[1]);
			Assert::IsTrue(qo1[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co1->getQueryObjectName() == "Modifies"sv
				&& co1->getArg1()->getArg() == "c"sv
				&& co1->getArg2()->getArg() == "v"sv);

			vector<string> testEntRefIdent = tokenize("variable v; Select v such that Modifies(\"procName\", v)");
			vector<string_view> testEntRefIdentSV{ sToSvVector(testEntRefIdent) };
			shared_ptr<QueryParser> p2 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefIdentObj = p2->splitDeclarationQuery(testEntRefIdentSV);
			vector<shared_ptr<QueryObject>> decs2 = p2->validateDeclaration(get<0>(testEntRefIdentObj));
			vector<shared_ptr<QueryObject>> qo2 = p2->validateQuery(std::get<1>(testEntRefIdentObj));

			Assert::IsTrue(typeid(*qo2[1]) == typeid(ModifiesEntityObject));

			std::shared_ptr<ClauseObject> co2 = std::static_pointer_cast<ModifiesEntityObject>(qo2[1]);
			Assert::IsTrue(qo2[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co2->getQueryObjectName() == "Modifies"sv
				&& co2->getArg1()->getArg() == "\"procName\""sv
				&& co2->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectFollowsStmtQuery)
		{
			vector<string> testS = tokenize("assign a; print v; Select v such that Follows(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(FollowsObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<FollowsObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Follows"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("while v; Select v such that Follows(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(FollowsObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<FollowsObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Follows"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectFollowsStarStmtQuery)
		{
			vector<string> testS = tokenize("assign a; if v; Select v such that Follows*(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(FollowsStarObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<FollowsStarObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Follows*"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("stmt v; Select v such that Follows*(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(FollowsStarObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<FollowsStarObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Follows*"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectParentStmtQuery)
		{
			vector<string> testS = tokenize("stmt a; read v; Select v such that Parent(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(ParentObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<ParentObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Parent"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("call v; Select v such that Parent(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(ParentObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<ParentObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Parent"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestValidSelectParentStarStmtQuery)
		{
			vector<string> testS = tokenize("if a; read v; Select v such that Parent*(a, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(ParentStarObject));

			std::shared_ptr<ClauseObject> co = std::static_pointer_cast<ParentStarObject>(qo[1]);
			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(co->getQueryObjectName() == "Parent*"sv
				&& co->getArg1()->getArg() == "a"sv
				&& co->getArg2()->getArg() == "v"sv);

			vector<string> testSInt = tokenize("assign v; Select v such that Parent*(10, v)");
			vector<string_view> testSIntSV{ sToSvVector(testSInt) };
			shared_ptr<QueryParser> pInt = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObjInt = pInt->splitDeclarationQuery(testSIntSV);
			vector<shared_ptr<QueryObject>> currInt = pInt->validateDeclaration(get<0>(testObjInt));
			vector<shared_ptr<QueryObject>> qoInt = pInt->validateQuery(std::get<1>(testObjInt));

			Assert::IsTrue(typeid(*qoInt[1]) == typeid(ParentStarObject));

			std::shared_ptr<ClauseObject> coInt = std::static_pointer_cast<ParentStarObject>(qoInt[1]);
			Assert::IsTrue(qoInt[0]->getQueryObjectName() == "v"sv);
			Assert::IsTrue(coInt->getQueryObjectName() == "Parent*"sv
				&& coInt->getArg1()->getArg() == "10"sv
				&& coInt->getArg2()->getArg() == "v"sv);
		}

	};
}