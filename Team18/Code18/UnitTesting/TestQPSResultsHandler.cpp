#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/QPS/QueryResultsTable.h"
#include "../source/QPS/ResultsHandler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestQPSResultsHandler) {
private:
	vector<string> singleTable = {"1", "2", "3"};
	vector<string> singleTableVars = { "a", "b", "c" };
	unordered_map<string, vector<string>> twoDTable = { { "3", { "5", "6", "7"}}};
	unordered_map<string, vector<string>> twoDTableEmpty;

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
}
