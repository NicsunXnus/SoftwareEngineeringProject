#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include"../source/QPS/OptimisedFunctionsStub.h"
#include <random>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
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
	TEST_METHOD(Test_Step_A) {
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1","v2" }, { { "1","2" }, { "a","b" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));

		OptimisedFunctionsStub::optimiseStepA(nonSelectClauses);
		bool isSame = false;
		int expectedEmptyTables = 5;
		vector< shared_ptr<QueryResultsTable>> emptyTables(nonSelectClauses.begin(), nonSelectClauses.begin() + expectedEmptyTables);
		vector< shared_ptr<QueryResultsTable>> nonEmptyTables(nonSelectClauses.begin() + expectedEmptyTables, nonSelectClauses.end());
		for (shared_ptr<QueryResultsTable> table : emptyTables) {
			if (!table->isEmpty()) {
				Assert::Fail();
			}
		}
		for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
			if (table->isEmpty()) {
				Assert::Fail();
			}
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
		/*tupleSelectClauses.emplace_back(singleSelectClausesS1);
		tupleSelectClauses.emplace_back(singleSelectClausesA);
		tupleSelectClauses.emplace_back(singleSelectClausesA1);
		tupleSelectClauses.emplace_back(singleSelectClausesV2);*/
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","v1" }, { generateRandomPair(), generateRandomPair() })); // Uses (s1,v1)
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Uses(5,"y")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a1","a2" }, { generateRandomPair(), generateRandomPair() })); // Affects (a1, a2) with a1.stmt = 20 
		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Modifies (6 , "x")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() })); //such that Parent (s1, s2)
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s2","s3" }, { generateRandomPair(), generateRandomPair() })); //such that Next (s2, s3)
		nonSelectClauses.emplace_back(QueryResultsTable::createTable("s1", generateRandomPair())); //Modifies (s1, "x")
		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a","v2" }, { generateRandomPair(), generateRandomPair() })); //Modifies (a, v2)
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

		OptimisedFunctionsStub::optimiseStepA(nonSelectClauses);

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

		OptimisedFunctionsStub::optimiseStepB(nonSelectClauses);

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