#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include "../source/QPS/QueryBuilder.h"
#include "../source/QPS/DataAccessLayerStub.h"
#include <QPS/QueryObjects/PatternClauseObject.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
	TEST_CLASS(TestQPSQueryEval) {
	public:

		TEST_METHOD(TestValidFollowsSynSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][0] == "2");
			Assert::IsTrue(table->getColumns()[1]["s1"][1] == "3");
			Assert::IsTrue(table->getColumns()[1]["s1"][2] == "4");
		}

		TEST_METHOD(TestValidFollowsSynSynSameSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant()); // empty table, since s == s is not possible
		}

		TEST_METHOD(TestValidFollowsIntSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(2, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidFollowsIntSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(7, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidFollowsSynInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(s, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
		}

		TEST_METHOD(TestInvalidFollowsSynInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Follows(s, 7)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
		}

		TEST_METHOD(TestValidFollowsSynWildcard) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidFollowsWildCardSyn) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
		}

		TEST_METHOD(TestValidFollowsIntWildcard) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsIntWildcard) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(6, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsWildcardInt) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsWildcardInt) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, 7)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsWildcardWildcard) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidFollowsIntInt) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidFollowsIntInt) {
			vector<string> testS = tokenize("assign s; Select s such that Follows(3, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(s, s1)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" && table->getColumns()[1]["s1"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "1" && table->getColumns()[1]["s1"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "2" && table->getColumns()[1]["s1"][2] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "2" && table->getColumns()[1]["s1"][3] == "4");
			Assert::IsTrue(table->getColumns()[0]["s"][4] == "3" && table->getColumns()[1]["s1"][4] == "4");
		}

		TEST_METHOD(TestValidParentStarSynSynSameSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(s, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(s, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "1");
		}

		TEST_METHOD(TestInvalidParentStarSynInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(s, 5)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarIntSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(1, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestInvalidParentStarIntSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(9, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarSynWildcard) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidParentStarWildcardSyn) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(_, s)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
		}

		TEST_METHOD(TestValidParentStarIntWildcard) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(3, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarIntWildcard) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(90, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarWildcardInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(_, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarWildcardInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(_, 86)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarIntInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(2, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestInvalidParentStarIntInt) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(9, 3)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(!table->getSignificant());
		}

		TEST_METHOD(TestValidParentStarWildcardWildcard) {
			vector<string> testS = tokenize("assign s, s1; Select s such that Parent*(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidUsesSynSyn) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select s such that Uses(s, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" && table->getColumns()[1]["v"][0] == "a");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2" && table->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3" && table->getColumns()[1]["v"][2] == "c");
			Assert::IsTrue(table->getColumns()[0]["s"][3] == "3" && table->getColumns()[1]["v"][3] == "b");
		}

		TEST_METHOD(TestValidUsesSynWildCard) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select s such that Uses(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
		}

		TEST_METHOD(TestValidUsesSynIdent) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select s such that Uses(s, \"b\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestValidUsesIntSyn) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select v such that Uses(2, v)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
			Assert::IsTrue(table->getColumns()[0]["v"][0] == "b");
		}

		TEST_METHOD(TestValidUsesIntWildCard) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select s such that Uses(2, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}

		TEST_METHOD(TestValidUsesIntIdent) {
			vector<string> testS = tokenize("assign s, s1; variable v; Select s such that Uses(3, \"c\")");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer, synonyms);
			Assert::IsTrue(table->getSignificant());
		}
		

		TEST_METHOD(TestValidPatternWildcardWildcard) {
			vector<string> testS = tokenize("assign s; Select s pattern s(_, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "2");
		}

		TEST_METHOD(TestValidPatternWildcardPartialConstant) {
			vector<string> testS = tokenize("assign s; Select s pattern s(_, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "4");
		}

		TEST_METHOD(TestValidPatternWildcardPartialVariable) {
			vector<string> testS = tokenize("assign s; Select s pattern s(_, _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestValidPatternCharStringWildcard) {
			vector<string> testS = tokenize("assign s; Select s pattern s(\"b\", _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
		}

		TEST_METHOD(TestValidPatternCharStringPartialConstant) {
			vector<string> testS = tokenize("assign s; Select s pattern s(\"c\", _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");
		}

		TEST_METHOD(TestValidPatternCharStringPartialVariable) {
			vector<string> testS = tokenize("assign s; Select s pattern s(\"c\", _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");
		}

		TEST_METHOD(TestValidPatternVarSynWildcard) {
			vector<string> testS = tokenize("assign s; variable v; Select s pattern s(v, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][2] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][3] == "3");

			Assert::IsTrue(tables[1]->getColumns()[1]["v"][0] == "a");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][2] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][3] == "c");
		}

		TEST_METHOD(TestValidPatternVarSynPartialConstant) {
			vector<string> testS = tokenize("assign s; variable v; Select s pattern s(v, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][0] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][1] == "c");
		}

		TEST_METHOD(TestValidPatternVarSynPartialVariable) {
			vector<string> testS = tokenize("assign s; variable v; Select s pattern s(v, _\"b\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][0] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][2] == "c");
		}

		TEST_METHOD(TestValidPatternVarSynPartialVariableSuchThatFollowsSynWildcard) {
			vector<string> testS = tokenize("assign s; variable v; Select s pattern s(v, _\"b\"_) such that Follows(s, _)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][0] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][1] == "b");
			Assert::IsTrue(tables[1]->getColumns()[1]["v"][2] == "c");
			Assert::IsTrue(tables[2]->getSignificant());
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][1] == "2");
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][2] == "2");
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][3] == "3");
			Assert::IsTrue(tables[2]->getNumberOfCols() == 1);
		}

		TEST_METHOD(TestValidSuchThatFollowsWildcardIntPatternWildcardPartialConstant) {
			vector<string> testS = tokenize("assign s; variable v; Select s such that Uses(s, \"b\") pattern s(_, _\"300\"_)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayerStub> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayer);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();

			Assert::IsTrue(tables[1]->getSignificant());
			Assert::IsTrue(tables[0]->getColumns()[0]["s"][0] == "1");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][0] == "2");
			Assert::IsTrue(tables[1]->getColumns()[0]["s"][1] == "3");
			Assert::IsTrue(tables[2]->getSignificant());
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][0] == "3");
			Assert::IsTrue(tables[2]->getColumns()[0]["s"][1] == "4");

		}
	};
}
