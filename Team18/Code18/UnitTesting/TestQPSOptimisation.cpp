#include "stdafx.h"
#include "CppUnitTest.h"
#include <cassert>
#include"../source/QPS/OptimisedFunctionsStub.h"
#include <random>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

//namespace UnitTesting
//
//{
//	TEST_CLASS(TestOptimisation) {
//private:
//
//	// PROMPT: GIVE ME A FUNCTION THAT RANDOMLY GENERATES A PAIR OF STRINGS WHERE EACH STRING IS EITHER A NUMBER FROM 0 TO 9 OR AN ALPHABET
//	// ai-gen start 0
//	vector<string> generateRandomPair() {
//		string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//		string numbers = "0123456789";
//		random_device rd;
//		mt19937 gen(rd());
//		uniform_int_distribution<> dis(0, 1);
//		uniform_int_distribution<> alphaDis(0, alphabet.size() - 1);
//		uniform_int_distribution<> numDis(0, numbers.size() - 1);
//		string first, second;
//		for (int i = 0; i < 1; ++i) { // Changed 2 to 1
//			if (dis(gen) == 0) {
//				first += alphabet[alphaDis(gen)];
//			}
//			else {
//				first += numbers[numDis(gen)];
//			}
//			if (dis(gen) == 0) {
//				second += alphabet[alphaDis(gen)];
//			}
//			else {
//				second += numbers[numDis(gen)];
//			}
//		}
//		return { first, second };
//	}
//	// ai-gen end
//
//	shared_ptr<QueryResultsTable> singleSelectClausesS1 = QueryResultsTable::createTable("s1", generateRandomPair());
//	shared_ptr<QueryResultsTable> singleSelectClausesS2 = QueryResultsTable::createTable("s2", generateRandomPair());
//	shared_ptr<QueryResultsTable> singleSelectClausesV1 = QueryResultsTable::createTable("v1", generateRandomPair());
//	shared_ptr<QueryResultsTable> singleSelectClausesV2 = QueryResultsTable::createTable("v2", generateRandomPair());
//	shared_ptr<QueryResultsTable> singleSelectClausesA1 = QueryResultsTable::createTable("a1", generateRandomPair());
//	shared_ptr<QueryResultsTable> singleSelectClausesA = QueryResultsTable::createTable("a", generateRandomPair());
//
//public:
//	TEST_METHOD(Test_Step_A) {
//		vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1","v2" }, { { "1","2" }, { "a","b" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//		nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//		nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { { "3","4" }, { "c","d" } }));
//
//		OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//		bool isSame = false;
//		int expectedEmptyTables = 5;
//		vector< shared_ptr<QueryResultsTable>> emptyTables(nonSelectClauses.begin(), nonSelectClauses.begin() + expectedEmptyTables);
//		vector< shared_ptr<QueryResultsTable>> nonEmptyTables(nonSelectClauses.begin() + expectedEmptyTables, nonSelectClauses.end());
//		for (shared_ptr<QueryResultsTable> table : emptyTables) {
//			if (!table->isEmpty()) {
//				Assert::Fail();
//			}
//		}
//		for (shared_ptr<QueryResultsTable> table : nonEmptyTables) {
//			if (table->isEmpty()) {
//				Assert::Fail();
//			}
//		}
//	}
//	// TEST_METHOD(Test_Visualise_A) {
//	//	vector< shared_ptr<QueryResultsTable> > tupleSelectClauses;
//	//	tupleSelectClauses.emplace_back(singleSelectClausesS1);
//	//	tupleSelectClauses.emplace_back(singleSelectClausesA);
//	//	tupleSelectClauses.emplace_back(singleSelectClausesA1);
//	//	tupleSelectClauses.emplace_back(singleSelectClausesV2);
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","v1" }, { generateRandomPair(), generateRandomPair() })); // Uses (s1,v1)
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Uses(5,"y")
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a1","a2" }, { generateRandomPair(), generateRandomPair() })); // Affects (a1, a2) with a1.stmt = 20 
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable()); // Modifies (6 , "x")
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","s2" }, { generateRandomPair(), generateRandomPair() })); //such that Parent (s1, s2)
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s2","s3" }, { generateRandomPair(), generateRandomPair() })); //such that Next (s2, s3)
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createTable("s1", generateRandomPair())); //Modifies (s1, "x")
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a2","v2" }, { generateRandomPair(), generateRandomPair() })); //Modifies (a, v2)
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	//nonSelectClauses = OptimisedFunctionsStub::revert1DTables(OptimisedFunctionsStub::optimiseStepB(nonSelectClauses));
//	//	vector<shared_ptr<GroupClause>> groups = OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false);
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (int i = 0; i < groups.size();i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table :groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B AND C:\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_B) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1"}, { generateRandomPair()}));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","v1" }, { generateRandomPair(), generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s1","a", "z", "b"}, {generateRandomPair(), generateRandomPair(), generateRandomPair() , generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s2" }, { generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "s2","x1" }, { generateRandomPair(), generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a","v2" }, { generateRandomPair(), generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "v1"}, { generateRandomPair() }));
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	nonSelectClauses = OptimisedFunctionsStub::revert1DTables(OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false));
//
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B AND C:\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_C) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a"}, {generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//
//
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "a" }, { generateRandomPair(),generateRandomPair() }));
//
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//	//	
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	//nonSelectClauses = OptimisedFunctionsStub::revert1DTables(OptimisedFunctionsStub::optimiseStepB(nonSelectClauses));
//	//	vector<shared_ptr<GroupClause>> groups = OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false);
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//
//	//	stringstream outputC;
//	//	streambuf* oldCoutBufferC = cout.rdbuf(outputC.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepC(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferC);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP C\n");
//	//	Logger::WriteMessage(outputC.str().c_str());
//
//	//	stringstream outputD;
//	//	streambuf* oldCoutBufferD = cout.rdbuf(outputD.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepD(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferD);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP D\n");
//	//	Logger::WriteMessage(outputD.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_D) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	//Leave linking one at the end
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//		
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	nonSelectClauses = OptimisedFunctionsStub::revert1DTables(OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false));
//
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B:\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_E) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "e" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "b","c","d","e"}, {generateRandomPair(),generateRandomPair(),generateRandomPair() ,generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	nonSelectClauses = OptimisedFunctionsStub::revert1DTables(OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false));
//
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B:\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_F) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "a" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "c" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "b", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "d", "d" }, { generateRandomPair(),generateRandomPair() }));
//
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "b" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "c", "d" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::createEmptyTable());
//
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	vector<shared_ptr<GroupClause>> groups = OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false);
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//
//	//	stringstream outputC;
//	//	streambuf* oldCoutBufferC = cout.rdbuf(outputC.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepC(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferC);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP C\n");
//	//	Logger::WriteMessage(outputC.str().c_str());
//
//	//	stringstream outputD;
//	//	streambuf* oldCoutBufferD = cout.rdbuf(outputD.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepD(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferD);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP D\n");
//	//	Logger::WriteMessage(outputD.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_G) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "a", "b","c","d","e"}, {generateRandomPair(),generateRandomPair(),generateRandomPair() ,generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "l", "m" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "e", "z" }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ "l", "z" }, { generateRandomPair(),generateRandomPair() }));
//	//	
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	vector<shared_ptr<GroupClause>> groups = OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false);
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//
//	//	stringstream outputC;
//	//	streambuf* oldCoutBufferC = cout.rdbuf(outputC.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepC(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferC);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP C\n");
//	//	Logger::WriteMessage(outputC.str().c_str());
//
//	//	stringstream outputD;
//	//	streambuf* oldCoutBufferD = cout.rdbuf(outputD.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepD(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferD);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP D\n");
//	//	Logger::WriteMessage(outputD.str().c_str());
//	//}
//
//	//TEST_METHOD(Test_Visualise_Random) {
//	//	vector< shared_ptr<QueryResultsTable>> nonSelectClauses;
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair() ,generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//	//	nonSelectClauses.emplace_back(QueryResultsTable::create2DTable({ generateRandomPair() }, { generateRandomPair(),generateRandomPair() }));
//
//	//	stringstream output;
//	//	streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());
//	//	int index = 0;
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		table->setId(index); index++;
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBuffer);
//
//	//	Logger::WriteMessage("Order of tables BEFORE STEP A:\n");
//	//	Logger::WriteMessage(output.str().c_str());
//
//	//	OptimisedFunctionsStub::optimiseStepA(nonSelectClauses, false);
//
//	//	stringstream outputA;
//	//	streambuf* oldCoutBufferA = cout.rdbuf(outputA.rdbuf());
//
//	//	for (shared_ptr<QueryResultsTable> table : nonSelectClauses) {
//	//		cout << "Table " + to_string(table->getId()) << endl;
//	//		table->printTable();
//	//		cout << endl;
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferA);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP A:\n");
//	//	Logger::WriteMessage(outputA.str().c_str());
//
//	//	vector<shared_ptr<GroupClause>> groups = OptimisedFunctionsStub::optimiseStepB(nonSelectClauses, false);
//	//	stringstream outputB;
//	//	streambuf* oldCoutBufferB = cout.rdbuf(outputB.rdbuf());
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferB);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP B\n");
//	//	Logger::WriteMessage(outputB.str().c_str());
//
//	//	stringstream outputC;
//	//	streambuf* oldCoutBufferC = cout.rdbuf(outputC.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepC(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferC);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP C\n");
//	//	Logger::WriteMessage(outputC.str().c_str());
//
//	//	stringstream outputD;
//	//	streambuf* oldCoutBufferD = cout.rdbuf(outputD.rdbuf());
//	//	OptimisedFunctionsStub::optimiseStepD(groups);
//	//	for (int i = 0; i < groups.size(); i++) {
//	//		cout << "Group " + to_string(i) << endl;
//	//		for (shared_ptr<QueryResultsTable> table : groups[i]->getMembers()) {
//	//			cout << "Table " + to_string(table->getId()) << endl;
//	//			table->printTable();
//	//			cout << endl;
//	//		}
//	//	}
//
//	//	cout.rdbuf(oldCoutBufferD);
//
//	//	Logger::WriteMessage("Order of tables AFTER STEP D\n");
//	//	Logger::WriteMessage(outputD.str().c_str());
//	//}
//
//
//	};
//}