#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/QPS/QueryObjects/PatternClauseObject.h"
#include "../source/QPS/QueryObjects/WithClauseObject.h"
#include "../source/QPS/QueryObjects/ComparisonQueryObject.h"
#include "../source/QPS/PQLTokenizer.h"
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
			vector<string> testS = PQLTokenizer::tokenize("stmt v; Select v");
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
			vector<string> testS = PQLTokenizer::tokenize("variable c; read v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectPrintQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("print p, v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectCallQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("call v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectWhileQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("while v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectIfQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("if v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectAssignQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectVariableQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("variable v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectConstantQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("constant v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectProcedureQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("procedure v; Select v");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(qo[0]->getQueryObjectName() == "v"sv);
		}

		TEST_METHOD(TestValidSelectUsesStmtQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("call a; variable v; Select v such that Uses(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("variable v; Select v such that Uses(10, v)");
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
			vector<string> testEntRefSyn = PQLTokenizer::tokenize("constant c; variable v; Select v such that Uses(c, v)");
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

			vector<string> testEntRefIdent = PQLTokenizer::tokenize("variable v; Select v such that Uses(\"procName\", v)");
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
			vector<string> testEntRefSyn = PQLTokenizer::tokenize("call c; read v; Select v such that Uses(c, v)");
			vector<string_view> testEntRefSynSV{ sToSvVector(testEntRefSyn) };
			shared_ptr<QueryParser> p1 = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testEntRefSynObj = p1->splitDeclarationQuery(testEntRefSynSV);
			vector<shared_ptr<QueryObject>> decs1 = p1->validateDeclaration(get<0>(testEntRefSynObj));
			try {
				vector<shared_ptr<QueryObject>> qo1 = p1->validateQuery(std::get<1>(testEntRefSynObj));
				std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<UsesObject>(qo1[1]);
			}
			catch (const QPSError& ex)
			{
				Assert::AreEqual(ex.what(), "Semantic error: 2nd argument is not a variable entity for Uses");
			}
			
		}
		TEST_METHOD(TestValidSelectModifiesStmtQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign a; variable v; Select v such that Modifies(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("variable v; Select v such that Modifies(10, v)");
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
			vector<string> testEntRefSyn = PQLTokenizer::tokenize("constant c; variable v; Select v such that Modifies(c, v)");
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

			vector<string> testEntRefIdent = PQLTokenizer::tokenize("variable v; Select v such that Modifies(\"procName\", v)");
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
			vector<string> testS = PQLTokenizer::tokenize("assign a; print v; Select v such that Follows(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("while v; Select v such that Follows(10, v)");
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
			vector<string> testS = PQLTokenizer::tokenize("assign a; if v; Select v such that Follows*(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("stmt v; Select v such that Follows*(10, v)");
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
			vector<string> testS = PQLTokenizer::tokenize("stmt a; read v; Select v such that Parent(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("call v; Select v such that Parent(10, v)");
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
			vector<string> testS = PQLTokenizer::tokenize("if a; read v; Select v such that Parent*(a, v)");
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

			vector<string> testSInt = PQLTokenizer::tokenize("assign v; Select v such that Parent*(10, v)");
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

		TEST_METHOD(TestValidSelectLongQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3; Select s1 such that Uses(s3, v1) such that Modifies(s3, \"x\") such that Follows(s1, s2) such that Parent(s3, s1) such that Uses(s2, v1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(UsesObject));
			Assert::IsTrue(typeid(*qo[2]) == typeid(ModifiesObject));
			Assert::IsTrue(typeid(*qo[3]) == typeid(FollowsObject)); 
			Assert::IsTrue(typeid(*qo[4]) == typeid(ParentObject));
			Assert::IsTrue(typeid(*qo[5]) == typeid(UsesObject));

		}

		
		TEST_METHOD(TestValidSimplePatternQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign a; Select a pattern a (\"x\", _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(PatternObject));
			
			std::shared_ptr<PatternObject> po = std::static_pointer_cast<PatternObject>(qo[1]);
			Assert::IsTrue(po->getPatternSynonym()->getArg() == "a"sv);
			Assert::IsTrue(po->getArg1()->getArg() == "\"x\""sv);
			Assert::IsTrue(po->getArg2()->getArg() == "_"sv);

			vector<string> testPMS = PQLTokenizer::tokenize("assign a; variable v; Select a pattern a (v, _\"1\"_)");
			vector<string_view> testPMSV{ sToSvVector(testPMS) };
			shared_ptr<QueryParser> pPM = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testPMObj = pPM->splitDeclarationQuery(testPMSV);
			vector<shared_ptr<QueryObject>> currPM = pPM->validateDeclaration(get<0>(testPMObj));
			vector<shared_ptr<QueryObject>> qoPM = pPM->validateQuery(std::get<1>(testPMObj));

			Assert::IsTrue(typeid(*qoPM[1]) == typeid(PatternObject));

			std::shared_ptr<PatternObject> poPM = std::static_pointer_cast<PatternObject>(qoPM[1]);
			Assert::IsTrue(poPM->getPatternSynonym()->getArg() == "a"sv);
			Assert::IsTrue(poPM->getArg1()->getArg() == "v"sv);
			Assert::IsTrue(poPM->getArg2()->getArg() == "\"1\""sv && poPM->getArg2()->isPartialMatchingExprSpec());
		}

		TEST_METHOD(TestValidPatternSuchThatQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign a; variable v; Select a pattern a (\"x\", _\"1\"_) such that Modifies(\"procName\", v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(PatternObject));
			std::shared_ptr<PatternObject> co1 = std::static_pointer_cast<PatternObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "pattern"sv
				&& co1->getArg1()->getArg() == "\"x\""sv
				&& co1->getArg2()->getArg() == "\"1\""sv && co1->getArg2()->isPartialMatchingExprSpec());

			Assert::IsTrue(typeid(*qo[2]) == typeid(ModifiesEntityObject));
			std::shared_ptr<ClauseObject> co2 = std::static_pointer_cast<ModifiesEntityObject>(qo[2]);
			Assert::IsTrue(co2->getQueryObjectName() == "Modifies"sv
				&& co2->getArg1()->getArg() == "\"procName\""sv
				&& co2->getArg2()->getArg() == "v"sv);
		}


		TEST_METHOD(TestValidSuchThatPatternQuery)
		{
			vector<string> testS = PQLTokenizer::tokenize("assign a; stmt s; Select a such that Parent*(10, s) pattern a (_, _\"1\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(ParentStarObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<ParentStarObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Parent*"sv
				&& co1->getArg1()->getArg() == "10"sv
				&& co1->getArg2()->getArg() == "s"sv);

			Assert::IsTrue(typeid(*qo[2]) == typeid(PatternObject));
			std::shared_ptr<PatternObject> co2 = std::static_pointer_cast<PatternObject>(qo[2]);
			Assert::IsTrue(co2->getQueryObjectName() == "pattern"sv
				&& co2->getArg1()->getArg() == "_"sv
				&& co2->getArg2()->getArg() == "\"1\""sv
				&& co2->getArg2()->isPartialMatchingExprSpec());
		}



		TEST_METHOD(TestModifiesWithPrint1stArg)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("print s; variable v; Select s such that Modifies (s, v)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(ModifiesObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<ModifiesObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Modifies"sv
				&& co1->getArg1()->getArg() == "s"sv
				&& co1->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestUsesWithRead1stArg)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("read s; variable v; Select s such that Uses (s, v)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(UsesObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<UsesObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Uses"sv
				&& co1->getArg1()->getArg() == "s"sv
				&& co1->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestCallsWithRead1stArg)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("read s; variable v; Select s such that Uses (s, v)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(testSv);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			Assert::IsTrue(typeid(*qo[1]) == typeid(UsesObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<UsesObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Uses"sv
				&& co1->getArg1()->getArg() == "s"sv
				&& co1->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestParentWcWcPatternWcWc)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("stmt n; assign a; Select n such that Parent (_, _) pattern a (_, _)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[1]) == typeid(ParentObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<ParentObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Parent"sv
				&& co1->getArg1()->getArg() == "_"sv
				&& co1->getArg2()->getArg() == "_"sv);

			Assert::IsTrue(typeid(*qo[2]) == typeid(PatternObject));
			std::shared_ptr<PatternObject> co2 = std::static_pointer_cast<PatternObject>(qo[2]);
			Assert::IsTrue(co2->getQueryObjectName() == "pattern"sv
				&& co2->getPatternSynonym()->getArg() == "a"sv
				&& co2->getArg1()->getArg() == "_"sv
				&& co2->getArg2()->getArg() == "_"sv);
		}

		TEST_METHOD(TestValidCallsSynWc)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("procedure p1, p2; Select p1 such that Calls (p1, _)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[1]) == typeid(CallsObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<CallsObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Calls"sv
				&& co1->getArg1()->getArg() == "p1"sv
				&& co1->getArg2()->getArg() == "_"sv);
		}

		TEST_METHOD(TestValidCallsWcIdent)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("procedure p1, p2; Select p1 such that Calls (_, \"main\")");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[1]) == typeid(CallsObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<CallsObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Calls"sv
				&& co1->getArg1()->getArg() == "_"sv
				&& co1->getArg2()->getArg() == "\"main\""sv);
		}

		TEST_METHOD(TestValidCallsIdentSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("procedure p1, p2; Select p1 such that Calls (\"main\", p1)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[1]) == typeid(CallsObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<CallsObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Calls"sv
				&& co1->getArg1()->getArg() == "\"main\""sv
				&& co1->getArg2()->getArg() == "p1"sv);
		}

		TEST_METHOD(TestValidCallsConstVar)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select c such that Calls (c, v)");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[1]) == typeid(CallsObject));
			std::shared_ptr<ClauseObject> co1 = std::static_pointer_cast<CallsObject>(qo[1]);
			Assert::IsTrue(co1->getQueryObjectName() == "Calls"sv
				&& co1->getArg1()->getArg() == "c"sv
				&& co1->getArg2()->getArg() == "v"sv);
		}

		TEST_METHOD(TestTupleSingleDeclaredSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select <c>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ConstantObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "c");
		}

		TEST_METHOD(TestTupleMultDeclaredSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select <c,v>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ConstantObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "c");
			Assert::IsTrue(typeid(*qo[1]) == typeid(VariableObject));
			Assert::IsTrue(qo[1]->getQueryObjectName() == "v");
		}

		TEST_METHOD(TestTupleSingleUndeclaredSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("Select <c>");
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

		TEST_METHOD(TestTupleMultUndeclaredSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c, b>");
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

		TEST_METHOD(TestTupleNoCloseBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c, b");
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

		TEST_METHOD(TestTupleNoOpenBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c, b>");
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

		TEST_METHOD(TestTupleNoBrackets)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c, b");
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

		TEST_METHOD(TestTupleInvalidSymbolsInTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c,@ .sd# ,b>");
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

		TEST_METHOD(TestTupleEndWithComma)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c,>");
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

		TEST_METHOD(TestTupleEndWithCommaNoBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c,");
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

		TEST_METHOD(TestTupleEndWithSuch)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c> such");
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

		TEST_METHOD(TestTupleEndWithSuchThat)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c> such that");
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

		TEST_METHOD(TestTupleEndWithPattern)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c> pattern");
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

		TEST_METHOD(TestTupleEndWithPatternSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c> pattern c");
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

		TEST_METHOD(TestSelectProcName)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.procName");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ProcNameObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "procName");
		}

		TEST_METHOD(TestSelectVarName)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.varName");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(VarNameObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "varName");
		}

		TEST_METHOD(TestSelectValue)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.value");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ValueObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "value");
		}

		TEST_METHOD(TestSelectStmtNo)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.stmt#");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "stmt#");
		}

		TEST_METHOD(TestSelectInvalidName)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.asdf#");
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

		TEST_METHOD(TestSelectDoubleDot)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c..procName");
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

		TEST_METHOD(TestSelectNoDot)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c procName");
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

		TEST_METHOD(TestSelectWrongSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c,procName");
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

		TEST_METHOD(TestSelectSingleAttrRefIncomplete)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.");
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

		TEST_METHOD(TestSelectUndeclaredSyn)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select b.procName");
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

		TEST_METHOD(TestSelectSingleProcNameTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ProcNameObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "procName");
		}

		TEST_METHOD(TestSelectSingleVarNameTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.varName>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(VarNameObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "varName");
		}

		TEST_METHOD(TestSelectSingleValueTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.value>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ValueObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "value");
		}

		TEST_METHOD(TestSelectSingleStmtNoTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.stmt#>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "stmt#");
		}

		TEST_METHOD(TestSelectDoubleStmtNoTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.stmt#, c.stmt#>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "stmt#");
			Assert::IsTrue(typeid(*qo[1]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[1]->getQueryObjectName() == "stmt#");
		}

		TEST_METHOD(TestSelectStmtNoSynTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.stmt#, c>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "stmt#");
			Assert::IsTrue(typeid(*qo[1]) == typeid(ConstantObject));
			Assert::IsTrue(qo[1]->getQueryObjectName() == "c");
		}

		TEST_METHOD(TestSelectSynStmtNoTuple)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c, c.stmt#>");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(ConstantObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "c");
			Assert::IsTrue(typeid(*qo[1]) == typeid(StmtNoObject));
			Assert::IsTrue(qo[1]->getQueryObjectName() == "stmt#");
		}

		TEST_METHOD(TestSelectSingleProcNameTupNoOpenBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.procName>");
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

		TEST_METHOD(TestSelectSingleProcNameTupNoCloseBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName");
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

		TEST_METHOD(TestSelectSingleProcNameTupDoubleOpenBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <<c.procName>");
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

		TEST_METHOD(TestSelectSingleProcNameTupDoubleCloseBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName>>");
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

		TEST_METHOD(TestSelectSingleProcNameTupDoubleDot)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c..procName>");
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

		TEST_METHOD(TestSelectSingleProcNameTupNoDot)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c procName>");
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

		TEST_METHOD(TestSelectSingleProcNameTupWrongSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c;procName>");
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

		TEST_METHOD(TestSelectSingleProcNameTupIncomplete)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.>");
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

		TEST_METHOD(TestSelectSingleProcNameTupIncompleteNoBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.");
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

		TEST_METHOD(TestSelectSingleProcNameTupCommaSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c,procName>"); // parsed as undeclared 'procName' synonym
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

		TEST_METHOD(TestSelectProcNameValueTupNoCommaSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName c.value>");
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

		TEST_METHOD(TestSelectProcNameValueTupDoubleCommaSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName,, c.value>");
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

		TEST_METHOD(TestSelectProcNameValueTupWrongSeparator)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName; c.value>");
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

		TEST_METHOD(TestSelectProcNameValueDoubleDot)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName, c..value>");
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

		TEST_METHOD(TestSelectProcNameValueNoOpeningBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select c.procName, c.value>");
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

		TEST_METHOD(TestSelectProcNameValueNoClosingBracket)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName, c.value");
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

		TEST_METHOD(TestSelectProcNameValueNoClosingBracketInvalidEnd)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName, c.value;");
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

		TEST_METHOD(TestSelectProcNameValueNoIncomplete)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; Select <c.procName, c.>");
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

		TEST_METHOD(TestWithIdentEqualValue)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select c with \"alpha\" = 15");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StaticStaticComparisonQueryObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "Static=Static"sv);
		}

		TEST_METHOD(TestWithAttrRefEqualIdent)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select c with c.value = \"alpha\"");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StaticAttrRefComparisonQueryObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "Static=AttrRef"sv);
		}

		TEST_METHOD(TestWithAttrRefEqualAttrRef)
		{
			vector<string> tokenizer = PQLTokenizer::tokenize("constant c; variable v; Select c with c.value = v.stmt#");
			vector<string_view> testSv{ sToSvVector(tokenizer) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			vector<shared_ptr<QueryObject>> qo = p->parsePQL(testSv);

			Assert::IsTrue(typeid(*qo[0]) == typeid(StaticAttrRefComparisonQueryObject));
			Assert::IsTrue(qo[0]->getQueryObjectName() == "AttrRef=AttrRef"sv);
		}

	};

}