#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/QPS/PQLTokenizer.h"
#include "../source/QPS/QueryBuilder.h"
#include "../source/QPS/DataAccessLayerStub.h"
#include <QPS/QueryObjects/PatternClauseObject.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestQPSQueryEval) {
	private:
		unordered_set<string> vectorToSet(vector<string> in) {
			unordered_set<string> out(in.begin(), in.end());
			return out;
		}
	public:

		TEST_METHOD(TestValidFollowsSynSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][0] == "2");
			Assert::IsTrue(table->getColumns()[1]["s1"][1] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][2] == "4");
			Assert::IsTrue(qo[0]->callAndProcess(dataAccessLayer)->getPrimaryKey() == "s");
		}

		TEST_METHOD(TestValidFollowsSynSynSameSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant()); // empty table, since s == s is not possible
		}

		TEST_METHOD(TestValidFollowsIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidFollowsIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(7, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidFollowsSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidFollowsSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, 7)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidFollowsSynWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidFollowsWildCardSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
		}

		TEST_METHOD(TestValidFollowsIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(6, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(_, 7)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsWildcardWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Follows(3, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" && table->getColumns()[1]["s1"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "1" && table->getColumns()[1]["s1"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "2" && table->getColumns()[1]["s1"][2] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "2" && table->getColumns()[1]["s1"][3] == "4");
			Assert::IsTrue(table->getColumns()[0]["s"][4] == "3" && table->getColumns()[1]["s1"][4] == "4");
		}

		TEST_METHOD(TestValidParentStarSynSynSameSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "1");
		}

		TEST_METHOD(TestInvalidParentStarSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, 5)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(1, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestInvalidParentStarIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(9, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidParentStarWildcardSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
		}

		TEST_METHOD(TestValidParentStarIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(3, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(90, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, 86)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(9, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarWildcardWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidUsesSynSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select s such that Uses(s, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" && table->getColumns()[1]["v"][0] == "a");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2" && table->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3" && table->getColumns()[1]["v"][2] == "c");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "3" && table->getColumns()[1]["v"][3] == "b");
		}

		TEST_METHOD(TestValidUsesSynWildCard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select s such that Uses(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidUsesSynIdent) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select s such that Uses(s, \"b\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestValidUsesIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select v such that Uses(2, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["v"][0] == "b");
		}

		TEST_METHOD(TestValidUsesIntWildCard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select s such that Uses(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidUsesIntIdent) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; variable v; Select s such that Uses(3, \"c\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}
		

		TEST_METHOD(TestValidPatternWildcardWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "2");
		}
                /*
		TEST_METHOD(TestValidPatternWildcardPartialConstant) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(_, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "4");
		}

		TEST_METHOD(TestValidPatternWildcardPartialVariable) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(_, _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
		}
        
		TEST_METHOD(TestValidPatternCharStringWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(\"b\", _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
		}

		
		TEST_METHOD(TestValidPatternCharStringPartialConstant) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(\"c\", _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
		}

		TEST_METHOD(TestValidPatternCharStringPartialVariable) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s pattern s(\"c\", _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
		}
		*/

		TEST_METHOD(TestValidPatternVarSynWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; variable v; Select s pattern s(v, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(tables->getColumns()[0]["s"][3] == "3");

			Assert::IsTrue(tables->getColumns()[1]["v"][0] == "a");
			Assert::IsTrue(tables->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(tables->getColumns()[1]["v"][2] == "b");
			Assert::IsTrue(tables->getColumns()[1]["v"][3] == "c");
		}
                /*
		TEST_METHOD(TestValidPatternVarSynPartialConstant) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; variable v; Select s pattern s(v, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables->getColumns()[1]["v"][0] == "b");
			Assert::IsTrue(tables->getColumns()[1]["v"][1] == "c");
		}

		TEST_METHOD(TestValidPatternVarSynPartialVariable) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; variable v; Select s pattern s(v, _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables->getColumns()[1]["v"][0] == "b");
			Assert::IsTrue(tables->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(tables->getColumns()[1]["v"][2] == "c");
		}


		TEST_METHOD(TestValidPatternVarSynPartialVariableSuchThatFollowsSynWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; variable v; Select s pattern s(v, _\"b\"_) such that Follows(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables->getSignificant());
			Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables->getColumns()[0]["s"][1] == "4");

		}


		TEST_METHOD(TestValidSuchThatFollowsWildcardIntPatternWildcardPartialConstant) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; variable v; Select s such that Uses(s, \"b\") pattern s(_, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "a" && table->getColumns()[1]["s1"][0] == "b");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "b" && table->getColumns()[1]["s1"][1] == "c");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "c" && table->getColumns()[1]["s1"][2] == "d");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "c" && table->getColumns()[1]["s1"][3] == "e");

		}*/

		TEST_METHOD(TestValidCallsSynWildCard) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "a");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "b");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "c");
		}

		TEST_METHOD(TestValidCallsSynIdent) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(s, \"c\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "b");
		}

		TEST_METHOD(TestValidCallsWildcardSyn) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "b");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "c");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "d");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "e");
		}

		TEST_METHOD(TestValidCallsWildcardWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidCallsWildcardIdent) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(_, \"c\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidCallsIdentSyn) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(\"c\", s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "d");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "e");
		}

		TEST_METHOD(TestValidCallsIdentWildCard) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(\"c\", _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidCallsIdentWildCard) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(\"e\", _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidCallsIdentIdent) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(\"c\", \"d\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidCallsIdentIdent) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(\"b\", \"d\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidNextSynSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][0] == "2");
			Assert::IsTrue(table->getColumns()[1]["s1"][1] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][2] == "4");
		}

		TEST_METHOD(TestValidNextSynSynSameSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant()); // empty table, since s == s is not possible
		}

		TEST_METHOD(TestValidNextIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidNextIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(7, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidNextSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidNextSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, 7)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidNextSynWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidNextWildCardSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
		}

		TEST_METHOD(TestValidNextIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidNextIntWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(86, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidNextWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidNextWildcardInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(_, 96)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidNextWildcardWildcard) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidNextIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidNextIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next(3, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidNextStarIntIntNormal) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidNextStarIntIntForLoop) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(3, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidNextStarIntIntForLoopSame) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(3, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidNextStarIntInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(96, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestInvalidNextStarIntIntInvalidFirst) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(3, 96)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidNextStarIntSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "3", "4", "2" }; // note that its assign statements only
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestInvalidNextStarIntSynBigInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(100, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			unordered_set<string> expected = {};
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidNextStarSynInt) {
			vector<string> testS = PQLTokenizer::tokenize("assign s; Select s such that Next*(s, 6)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "1", "2", "3", "4" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidNextStarSynSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next*(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "1", "2", "3", "4" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidNextStarSynSynSameSyn) {
			vector<string> testS = PQLTokenizer::tokenize("assign s, s1; Select s such that Next*(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "2", "3", "4" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidSelectProcProcName) {
			vector<string> testS = PQLTokenizer::tokenize("procedure s, s1; Select s.procName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.procName"][0] == "a");
			Assert::IsTrue(table->getPrimaryKey() == "s.procName");
		}

		TEST_METHOD(TestValidSelectCallProcName) {
			vector<string> testS = PQLTokenizer::tokenize("call s, s1; Select s.procName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.procName"][0] == "a");
			Assert::IsTrue(table->getPrimaryKey() == "s.procName");
		}

		TEST_METHOD(TestValidSelectVarVarName) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
			Assert::IsTrue(table->getPrimaryKey() == "s.varName");
		}

		TEST_METHOD(TestValidSelectReadVarName) {
			vector<string> testS = PQLTokenizer::tokenize("read s, s1; Select s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "c");
			Assert::IsTrue(table->getPrimaryKey() == "s.varName");
		}

		TEST_METHOD(TestValidSelectPrintVarName) {
			vector<string> testS = PQLTokenizer::tokenize("print s, s1; Select s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "e");
			Assert::IsTrue(table->getPrimaryKey() == "s.varName");
		}

		TEST_METHOD(TestValidSelectConstantValue) {
			vector<string> testS = PQLTokenizer::tokenize("constant s, s1; Select s.value");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.value"][0] == "100");
			Assert::IsTrue(table->getPrimaryKey() == "s.value");
		}

		TEST_METHOD(TestValidSelectStmtStmtNo) {
			vector<string> testS = PQLTokenizer::tokenize("stmt s, s1; Select s.stmt#");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.stmt#"][0] == "1");
			Assert::IsTrue(table->getPrimaryKey() == "s.stmt#");
		}

		TEST_METHOD(TestInvalidSelectProcName) {
			vector<string> testS = PQLTokenizer::tokenize("stmt s, s1; Select s.procName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestInvalidSelectValue) {
			vector<string> testS = PQLTokenizer::tokenize("stmt s, s1; Select s.value");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestInvalidSelectStmtNo) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.stmt#");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidWithClauseStaticStatic) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.stmt# with \"test\" = \"test\"");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->isEmpty());
		}

		TEST_METHOD(TestValidWithClauseStaticStaticNotSame) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.stmt# with \"test\" = \"test1\"");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->isEmpty());
		}

		TEST_METHOD(TestValidWithClauseStaticAttr) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName with \"a\" = s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
		}

		TEST_METHOD(TestValidWithClauseAttrStatic) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName with s.varName = \"a\"");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
		}

		TEST_METHOD(TestValidWithClauseAttrAttr) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName with s.varName = s1.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "a", "b", "c" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidWithClauseAttrAttrSameAttr) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName with s.varName = s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "a", "b", "c" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
		}

		TEST_METHOD(TestValidSelectAttrVarName) {
			vector<string> testS = PQLTokenizer::tokenize("variable s, s1; Select s.varName with s.varName = s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expected = { "a", "b", "c" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
			Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) == expected);
		}

		TEST_METHOD(TestValidSelectAttrCallProcName) {
			vector<string> testS = PQLTokenizer::tokenize("call s, s1; Select s.procName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expectedS = { "1", "2" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
			unordered_set<string> expectedProcName = { "a", "b" };
			Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.procName"]) == expectedProcName);
		}

		TEST_METHOD(TestValidSelectAttrReadVarName) {
			vector<string> testS = PQLTokenizer::tokenize("read s, s1; Select s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expectedS = { "3", "4" };
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
			unordered_set<string> expectedProcName = { "c", "d" };
			Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) == expectedProcName);
		}

		TEST_METHOD(TestValidSelectAttrPrintVarName) {
			vector<string> testS = PQLTokenizer::tokenize("print s, s1; Select s.varName");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
			Assert::IsTrue(table->getSignificant());
			unordered_set<string> expectedS = { "1"};
			Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
			unordered_set<string> expectedProcName = { "e"};
			Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) == expectedProcName);
		}

	};
}
