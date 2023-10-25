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
		unordered_set<string> values = { "a","b" };
		vector<string> valuesA = { "x","y" };
		vector<string> valuesB = { "1","2" };
		shared_ptr<QueryResultsTable> singleSelectClauses = QueryResultsTable::createTable("s1", values);
		vector< shared_ptr<QueryResultsTable>> tupleSelectClauses = { QueryResultsTable::createTable("s1", values), QueryResultsTable::createTable("s1", values)};
		vector< shared_ptr<QueryResultsTable>> nonSelectClauses = { QueryResultsTable::create2DTable({"v1","v2"}, {valuesA, valuesB}), QueryResultsTable::createEmptyTable(), QueryResultsTable::create2DTable({"s1","s2"}, {valuesA, valuesB})};
		
		// A set to contain only unique select clauses
		set<string> getHeadersOfTableAsSet(vector<shared_ptr<QueryResultsTable>> selectClauseTables) {
			set<string> result;
			for (shared_ptr<QueryResultsTable> table : selectClauseTables) {
				result.insert(table->getHeaders()[0]);
			}
			return result;
		}

		// 1. Add all empty tables to the beginning
		// 2. Removes columns that the select clauses do not ask for
		void optimiseStepA(vector<shared_ptr<QueryResultsTable>> selectClauseTables, vector<shared_ptr<QueryResultsTable>>& nonSelectClauseTables) {
			vector<shared_ptr<QueryResultsTable>> emptyTables;
			for (shared_ptr<QueryResultsTable> table : nonSelectClauseTables) {
				if (table->isEmpty()) {
					emptyTables.emplace_back(table);
					//nonSelectClauseTables.erase(nonSelectClauseTables.begin() + index);
				}
			}
			vector<shared_ptr<QueryResultsTable>> result;
			// Get non empty tables by taking the opposite of an intersection between the emptyTables and nonSelectClauseTables
			std::set_symmetric_difference(emptyTables.begin(), emptyTables.end(), nonSelectClauseTables.begin(), nonSelectClauseTables.end(), std::inserter(result, result.begin()));
			result.insert(result.begin(), emptyTables.begin(), emptyTables.end());
			nonSelectClauseTables = result;
			//End of step 1

			set<string> selectClauses = getHeadersOfTableAsSet(selectClauseTables);

			for (shared_ptr<QueryResultsTable>& table : nonSelectClauseTables) {
				if (!table->isEmpty()) {
					vector< map<string, vector<string>> > columns = table->getColumns();
					vector< map<string, vector<string>> > result;
					for (map<string, vector<string>> column : columns) {
						if (find(selectClauses.begin(), selectClauses.end(), column.begin()->first) != selectClauses.end()) {
							result.emplace_back(column);
						}
					}
					table->setColumns(result);
				}
			}
		}

		TEST_METHOD(Test_Step_A_SingleSelect) {
			vector< shared_ptr<QueryResultsTable>> expected_result = { QueryResultsTable::createEmptyTable(), QueryResultsTable::createTable("s1", values) };
			optimiseStepA({ singleSelectClauses }, nonSelectClauses);
			bool isSame = false;
			int size = nonSelectClauses.size();
			if (nonSelectClauses.size() != expected_result.size()) {
				
			}
			else {
				if (!nonSelectClauses[0]->isEmpty()) {
					assert(false);
				}
				bool isSame = compare_vectors_of_maps(nonSelectClauses[1]->getColumns(), expected_result[1]->getColumns());
			}
			assert(isSame);
		}

		TEST_METHOD(Test_Step_A_TuplesSelect) {
			vector< shared_ptr<QueryResultsTable>> expected_result = { QueryResultsTable::createEmptyTable(), QueryResultsTable::createTable("s1", values) };
			optimiseStepA(tupleSelectClauses, nonSelectClauses);
			bool isSame = false;
			if (nonSelectClauses.size() != expected_result.size()) {
				
			}
			else {
				if (!nonSelectClauses[0]->isEmpty()) {
					assert(false);
				}
				bool isSame = compare_vectors_of_maps(nonSelectClauses[1]->getColumns(), expected_result[1]->getColumns());
			}
			assert(isSame);
		}
	};
}
