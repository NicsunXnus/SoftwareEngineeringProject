#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include "../source/QPS/QueryBuilder.h"
#include "../source/QPS/DataAccessLayerStub.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSQueryEval)
	{
	public:

		TEST_METHOD(TestValidAssignFollowsIntInt)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(1, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			/*
			// Create mock data to insert
			StringMap entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
			StringMap variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
			StringMap procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}} };
			StringMap constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}} };

			shared_ptr<StringMap> toInsertStatement = make_shared<StringMap>(entityStatementData);
			shared_ptr<StringMap> toInsertVariable = make_shared<StringMap>(variableData);
			shared_ptr<StringMap> toInsertProcedure = make_shared<StringMap>(procedureData);
			shared_ptr<StringMap> toInsertConstant = make_shared<StringMap>(constantData);

			// Insertion
			PKB::insertor.addEntity(toInsertStatement);
			PKB::insertor.addEntity(toInsertConstant, CONSTANT);
			PKB::insertor.addEntity(toInsertProcedure, PROCEDURE);
			PKB::insertor.addEntity(toInsertVariable, VARIABLE);
			StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
			StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
			shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsStarData);

			// Insertion
			PKB::insertor.addAbstraction(toInsert, FOLLOWS);
			*/
			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			Assert::IsTrue(clause->getArg1()->isInteger());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns().size() == 0);
	


		}

		TEST_METHOD(TestValidAssignStatementFollowsIntStatement)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "4");
			



		}

		TEST_METHOD(TestValidAssignStatementFollowsStmtWildcard)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][2] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][3] == "3");
			Assert::IsTrue(tables[1]->getNumberOfCols() == 1);



		}

		TEST_METHOD(TestValidAssignStatementFollowsWildcardStmt)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][2] == "4");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][3] == "4");
			Assert::IsTrue(tables[1]->getNumberOfCols() == 1);




		}

		TEST_METHOD(TestValidAssignStatementFollowsWildcardInt)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getNumberOfCols() == 0);




		}

		TEST_METHOD(TestValidAssignParent)
		{
			vector<string> testS = tokenize("assign s; Select s such that Parent(1, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			Assert::IsTrue(clause->getArg1()->isInteger());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns().size() == 0);



		}

		TEST_METHOD(TestValidAssignStatementParentStatement)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");




		}

		TEST_METHOD(TestValidAssignUsesStatementIdent)
		{
			vector<string> testS = tokenize("assign s; variable v; Select s such that Uses(s, \"b\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");



		}

		TEST_METHOD(TestValidAssignUsesStatementWildCard)
		{
			vector<string> testS = tokenize("assign s; variable v; Select s such that Uses(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][3] == "3");



		}

		TEST_METHOD(TestValidAssignStatementUsesIntegerVariable)
		{
			vector<string> testS = tokenize("assign s; variable v; Select v such that Uses(2, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["v"][0] == "a");
			Assert::IsTrue(tables[1]->getColumns()[0]["v"][0] == "b");




		}

		TEST_METHOD(TestValidAssignStatementUsesIntegerWildcard)
		{
			vector<string> testS = tokenize("assign s; variable v; Select v such that Uses(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[1]->getNumberOfCols() == 0);
			Assert::IsTrue(tables[0]->getColumns()[0]["v"][0] == "a");



		}

		TEST_METHOD(TestValidAssignStatementUsesIntegerWildcardNotSignificant)
		{
			vector<string> testS = tokenize("assign s; variable v; Select v such that Uses(69, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(!tables[1]->getSignificant()); // table is not significant
			Assert::IsTrue(tables[1]->getNumberOfCols() == 0);
			Assert::IsTrue(tables[0]->getColumns()[0]["v"][0] == "a");



		}

		TEST_METHOD(TestValidAssignModifiesStatementIdent)
		{
			vector<string> testS = tokenize("assign s; variable v; Select s such that Modifies(s, \"a\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
		
		}

		TEST_METHOD(TestValidAssignModifiesStatementWildcard)
		{
			vector<string> testS = tokenize("assign s; variable v; Select s such that Modifies(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][2] == "3");

		}

		TEST_METHOD(TestValidAssignStatementModifiesIntegerVariable)
		{
			vector<string> testS = tokenize("assign s; variable v; Select v such that Modifies(3, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<ClauseObject>(qo[1]);
			//Assert::IsTrue(clause->getArg2()->isSynonym());
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["v"][0] == "a");
			Assert::IsTrue(tables[1]->getColumns()[0]["v"][0] == "b");
			Assert::IsTrue(tables[1]->getColumns()[0]["v"][1] == "c");


		}

	};

}
