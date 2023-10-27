#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/QPS/QueryResultsTable.h"
#include "../source/QPS/ResultsHandler.h"
#include"../source/QPS/ResultsHandlerStub.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
#include <random>
namespace UnitTesting {
	TEST_CLASS(TestQPSResultsHandler) {
private:
	vector<string> singleTable = {"1", "2", "3"};
	vector<string> singleTableVars = { "a", "b", "c" };
	map<string, vector<string>> twoDTable = { { "3", { "5", "6", "7"}}};
	map<string, vector<string>> twoDTableEmpty;

public:
		TEST_METHOD(TestSingleSynonymNoSuchThatClause) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = {};
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1");
		}

		TEST_METHOD(TestSingleSynonymEmptyClause) {
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { QueryResultsTable::createTable("a", singleTable) };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = {make_shared<QueryResultsTable>()};
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 0);
		}

		TEST_METHOD(TestSingleSynonymNonEmptyClause) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable("b", singleTableVars) };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1");
		}

		TEST_METHOD(TestSingleSynonymNonEmptyClauseSharedSynonym) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable("a", singleTable) };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1");
		}

		TEST_METHOD(TestTupleNoSuchThatClause) {
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { QueryResultsTable::createTable("a", singleTable), QueryResultsTable::createTable("b", singleTableVars) };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = {};
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 9);
			Assert::IsTrue(*l_front == "1 a");
			advance(l_front, 8);
			Assert::IsTrue(*l_front == "3 c");
		}

		TEST_METHOD(TestTupleEmptyClause) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { make_shared<QueryResultsTable>() };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 0);

		}

		TEST_METHOD(TestTupleEmptyClauseSameSyn) { // select <s, s>
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("a", singleTableVars);
			table2->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1 1");
			advance(l_front, 2);
			Assert::IsTrue(*l_front == "3 3");
		}

		TEST_METHOD(TestTupleEmptyClauseSameSynTriple) { // select <s, s, s>
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("a", singleTableVars);
			table2->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table3 = QueryResultsTable::createTable("a", singleTableVars);
			table3->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 , table3};
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1 1 1");
			advance(l_front, 2);
			Assert::IsTrue(*l_front == "3 3 3");
		}

		TEST_METHOD(TestTupleEmptyClauseSameSynCombination) { // select <s, a, s>
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			shared_ptr<QueryResultsTable> table3 = QueryResultsTable::createTable("a", singleTableVars);
			table3->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 , table3 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 9);
			Assert::IsTrue(*l_front == "1 a 1");
			advance(l_front, 8);
			Assert::IsTrue(*l_front == "3 c 3");
		}

		TEST_METHOD(TestTupleEmptyButSignificantClause) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			shared_ptr<QueryResultsTable> emptyTable = QueryResultsTable::createEmptyTable();
			emptyTable->setSignificant(true);
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { emptyTable };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 9);
			Assert::IsTrue(*l_front == "1 a");
			advance(l_front, 8);
			Assert::IsTrue(*l_front == "3 c");
		}

		TEST_METHOD(TestTupleNonEmptyClause) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable("c", singleTable) };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 9);
			Assert::IsTrue(*l_front == "1 a");
			advance(l_front, 8);
			Assert::IsTrue(*l_front == "3 c");
		}

		TEST_METHOD(TestTupleNonEmptyClauseSameSyn) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("a", singleTable);
			table2->setPrimaryKey("a");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable("c", singleTable) };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "1 1");
			advance(l_front, 2);
			Assert::IsTrue(*l_front == "3 3");
		}

		TEST_METHOD(TestTupleNonEmptyClauseSharedVars) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable("a", singleTable) };
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 9);
			Assert::IsTrue(*l_front == "1 a");
			advance(l_front, 8);
			Assert::IsTrue(*l_front == "3 c");
		}

		TEST_METHOD(TestTupleNonEmptyClauseSharedVars2DTable) {
			shared_ptr<QueryResultsTable> table1 = QueryResultsTable::createTable("a", singleTable);
			table1->setPrimaryKey("a");
			shared_ptr<QueryResultsTable> table2 = QueryResultsTable::createTable("b", singleTableVars);
			table2->setPrimaryKey("b");
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { table1, table2 };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable({"a", "c"}, twoDTable)};
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 3);
			Assert::IsTrue(*l_front == "3 a");
			advance(l_front, 2);
			Assert::IsTrue(*l_front == "3 c");
		}

		TEST_METHOD(TestTupleNonEmptyClauseSharedVarsEmpty2DTable) {
			vector<shared_ptr<QueryResultsTable>> selectClauseTable = { QueryResultsTable::createTable("a", singleTable), QueryResultsTable::createTable("b", singleTableVars) };
			vector<shared_ptr<QueryResultsTable>> nonSelectClauseTable = { QueryResultsTable::createTable({"a", "c"}, twoDTableEmpty)};
			shared_ptr<ResultHandler> resultHandler = make_shared<ResultHandler>();
			list<string> finalResult = resultHandler->processTables(selectClauseTable, nonSelectClauseTable);
			auto l_front = finalResult.begin();
			advance(l_front, 0);
			Assert::IsTrue(finalResult.size() == 0);

		}
	};
	TEST_CLASS(TestOptimisation) {
private:

	// PROMPT: GIVE ME A FUNCTION THAT RANDOMLY GENERATES A PAIR OF STRINGS WHERE EACH STRING IS EITHER A NUMBER FROM 0 TO 9 OR AN ALPHABET
	// ai-gen start 0
	vector<std::string> generateRandomPair() {
		std::string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		std::string numbers = "0123456789";
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(0, 1);
		std::uniform_int_distribution<> alphaDis(0, alphabet.size() - 1);
		std::uniform_int_distribution<> numDis(0, numbers.size() - 1);
		std::string first, second;
		for (int i = 0; i < 2; ++i) {
			if (dis(gen) == 0) {
				first += alphabet[alphaDis(gen)];
			}
			else {
				first += numbers[numDis(gen)];
			}
			if (dis(gen) == 0) {
				second += alphabet[alphaDis(gen)];
			}
			else {
				second += numbers[numDis(gen)];
			}
		}
		return { first, second };
	}
	// ai-gen end

	shared_ptr<QueryResultsTable> singleSelectClausesS1 = QueryResultsTable::createTable("s1", generateRandomPair());
	shared_ptr<QueryResultsTable> singleSelectClausesS2 = QueryResultsTable::createTable("s2", generateRandomPair());
	shared_ptr<QueryResultsTable> singleSelectClausesV1 = QueryResultsTable::createTable("v1", generateRandomPair());
	shared_ptr<QueryResultsTable> singleSelectClausesV2 = QueryResultsTable::createTable("v2", generateRandomPair());
	shared_ptr<QueryResultsTable> singleSelectClausesA1 = QueryResultsTable::createTable("a1", generateRandomPair());
	shared_ptr<QueryResultsTable> singleSelectClausesA = QueryResultsTable::createTable("a", generateRandomPair());

public:
	TEST_METHOD(Test_Step_A_SingleSelect) {
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1","v2" }, { generateRandomPair(), generateRandomPair() }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() }));
		vector< shared_ptr<QueryResultsTable>> selectClauses;
		selectClauses.emplace_back(singleSelectClausesS1);
		vector< shared_ptr<QueryResultsTable>> expected_result;
		expected_result.emplace_back(QueryResultsTable::createEmptyTable());
		expected_result.emplace_back(QueryResultsTable::createEmptyTable());
		expected_result.emplace_back(QueryResultsTable::createTable("s1", generateRandomPair()));
		
		ResultsHandlerStub::optimiseStepA(selectClauses, nonSelectClauses);
		bool isSame = false;
		int size = nonSelectClauses.size();
		if (nonSelectClauses.size() == expected_result.size()) {
			if (!nonSelectClauses[0]->isEmpty() || !nonSelectClauses[1]->isEmpty()) {
				Assert::Fail();
			}
			isSame = compare_vectors_of_maps(nonSelectClauses[0]->getColumns(), expected_result[0]->getColumns()) && 
				compare_vectors_of_maps(nonSelectClauses[1]->getColumns(), expected_result[1]->getColumns()) &&
				compare_vectors_of_maps(nonSelectClauses[2]->getColumns(), expected_result[2]->getColumns());
			
			Assert::AreEqual(true, isSame);
		}
		else {
			Assert::Fail();
		}	
	}

	TEST_METHOD(Test_Step_A_TuplesSelectA) {
		vector< shared_ptr<QueryResultsTable>> expected_result;
		expected_result.emplace_back(QueryResultsTable::createEmptyTable());
		expected_result.emplace_back(QueryResultsTable::createEmptyTable());
		expected_result.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() }));
		vector< shared_ptr<QueryResultsTable> > tupleSelectClauses;
		tupleSelectClauses.emplace_back(singleSelectClausesS1);
		tupleSelectClauses.emplace_back(singleSelectClausesS2);
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1","v2" }, { generateRandomPair(), generateRandomPair() }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() }));
		ResultsHandlerStub::optimiseStepA(tupleSelectClauses, nonSelectClauses);
		bool isSame = false;
		if (nonSelectClauses.size() == expected_result.size()) {
			if (!nonSelectClauses[0]->isEmpty() && !nonSelectClauses[1]->isEmpty()) {
				Assert::Fail();
			}
			isSame = compare_vectors_of_maps(nonSelectClauses[0]->getColumns(), expected_result[0]->getColumns()) &&
				compare_vectors_of_maps(nonSelectClauses[1]->getColumns(), expected_result[1]->getColumns()) &&
				compare_vectors_of_maps(nonSelectClauses[2]->getColumns(), expected_result[2]->getColumns());

			Assert::AreEqual(true, isSame);
		}
		else {
			Assert::Fail();
		}
	}

	TEST_METHOD(Test_Step_A_TuplesSelectB) {
		vector< shared_ptr<QueryResultsTable>> expected_result;
		expected_result.emplace_back(QueryResultsTable::createEmptyTable());
		expected_result.emplace_back(QueryResultsTable::createTable("v1", generateRandomPair()));
		expected_result.emplace_back(QueryResultsTable::createTable("s1", generateRandomPair()));
		vector< shared_ptr<QueryResultsTable> > tupleSelectClauses;
		tupleSelectClauses.emplace_back(singleSelectClausesS1);
		tupleSelectClauses.emplace_back(singleSelectClausesV1);
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1","v2" }, { generateRandomPair(), generateRandomPair() }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() }));
		ResultsHandlerStub::optimiseStepA(tupleSelectClauses, nonSelectClauses);
		bool isSame = false;
		if (nonSelectClauses.size() == expected_result.size()) {
			if (!nonSelectClauses[0]->isEmpty()) {
				Assert::Fail();
			}
			isSame = compare_vectors_of_maps(nonSelectClauses[0]->getColumns(), expected_result[0]->getColumns()) &&
				compare_vectors_of_maps(nonSelectClauses[1]->getColumns(), expected_result[1]->getColumns()) &&
				compare_vectors_of_maps(nonSelectClauses[2]->getColumns(), expected_result[2]->getColumns());

			Assert::AreEqual(true, isSame);
		}
		else {
			Assert::Fail();
		}
	}

	TEST_METHOD(Test_Visualise) {
		//Reference to CS3203 Optimisation Slides Slide 19
		/*
		assign a, a1, a2; stmt s1, s2, s3; variable v1, v2;
		 Select <s1, a, a1, v2> 
			 such that Uses (s1, v1) and Uses (5, "y")
			 Affects (a1, a2) and Modifies (6 , "x")
			 with a1.stmt = 20 
			 such that Parent (s1, s2) 
			 such that Next (s2, s3) and 
			 Modifies (s1, "x") and 
			 Modifies (a, v2) 
		*/
		vector< shared_ptr<QueryResultsTable> > tupleSelectClauses;
		tupleSelectClauses.emplace_back(singleSelectClausesS1);
		tupleSelectClauses.emplace_back(singleSelectClausesA);
		tupleSelectClauses.emplace_back(singleSelectClausesA1);
		tupleSelectClauses.emplace_back(singleSelectClausesV2);
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","v1" }, { generateRandomPair(), generateRandomPair() })); // Uses (s1,v1)
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Uses(5,"y")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a1","a2" }, { generateRandomPair(), generateRandomPair() })); // Affects (a1, a2) with a1.stmt = 20 
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Modifies (6 , "x")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() })); //such that Parent (s1, s2)
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s2","s3" }, { generateRandomPair(), generateRandomPair() })); //such that Next (s2, s3)
		nonSelectClauses.emplace_back(QueryResultsTable::createTable("s1", generateRandomPair())); //Modifies (s1, "x")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a","v2" }, { generateRandomPair(), generateRandomPair() })); //Modifies (s1, "x")
		std::stringstream output;
		std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
		int index = 0;
		for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
			table->setId(index); index++;
			cout << "Table " + to_string(table->getId()) << endl;
			table->printTable();
			cout << endl;
		}

		std::cout.rdbuf(oldCoutBuffer);

		Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
		Logger::WriteMessage(output.str().c_str());

		ResultsHandlerStub::optimiseStepA(tupleSelectClauses, nonSelectClauses);

		std::stringstream outputA;
		std::streambuf* oldCoutBufferA = std::cout.rdbuf(outputA.rdbuf());

		for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
			cout << "Table " + to_string(table->getId()) << endl;
			table->printTable();
			cout << endl;
		}

		std::cout.rdbuf(oldCoutBufferA);

		Logger::WriteMessage("Order of tables AFTER STEP A:\n");
		Logger::WriteMessage(outputA.str().c_str());

		nonSelectClauses = ResultsHandlerStub::optimiseStepB(nonSelectClauses);

		std::stringstream outputB;
		std::streambuf* oldCoutBufferB = std::cout.rdbuf(outputB.rdbuf());
		for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
			cout << "Table " + to_string(table->getId()) << endl;
			table->printTable();
			cout << endl;
		}

		std::cout.rdbuf(oldCoutBufferB);

		Logger::WriteMessage("Order of tables AFTER STEP B AND C:\n");
		Logger::WriteMessage(outputB.str().c_str());
	}
	};
}
