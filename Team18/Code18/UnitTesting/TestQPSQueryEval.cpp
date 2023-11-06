#include "stdafx.h"
#include "../source/QPS/DataAccessLayerAffectsStub.h"
#include "../source/QPS/DataAccessLayerStub.h"
#include "../source/QPS/PQLTokenizer.h"
#include "../source/QPS/QueryBuilder.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/QPS/QueryObjects/PatternClauseObject.h"
#include "../source/QPS/QueryParser.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting {
TEST_CLASS(TestQPSQueryEval){private : unordered_set<string> vectorToSet(
    vector<string> in){unordered_set<string> out(in.begin(), in.end());
return out;
}  // namespace UnitTesting
public:
TEST_METHOD(TestValidFollowsSynSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, s1)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
  Assert::IsTrue(table->getColumns()[1]["s1"][0] == "2");
  Assert::IsTrue(table->getColumns()[1]["s1"][1] == "3");
  Assert::IsTrue(table->getColumns()[1]["s1"][2] == "4");
  Assert::IsTrue(qo[0]->callAndProcess(dataAccessLayer)->getPrimaryKey() ==
                 "s");
}

TEST_METHOD(TestValidFollowsSynSynSameSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(
      !table->getSignificant());  // empty table, since s == s is not possible
}

TEST_METHOD(TestInalidFollowsSynInt) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; Select s such that Follows(s, 3123)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getNumberOfCols() == 1);
}

TEST_METHOD(TestValidFollowsIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(2, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
}

TEST_METHOD(TestInvalidFollowsIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(7, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
}

TEST_METHOD(TestValidFollowsSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
}

TEST_METHOD(TestInvalidFollowsSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Follows(s, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
}

TEST_METHOD(TestValidFollowsSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
}

TEST_METHOD(TestValidFollowsWildCardSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(_, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
}

TEST_METHOD(TestValidFollowsIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(2, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidFollowsIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(6, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidFollowsWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(_, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidFollowsWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(_, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidFollowsWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidFollowsIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(2, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidFollowsIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Follows(3, 2)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarSynSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, s1)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" &&
                 table->getColumns()[1]["s1"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "1" &&
                 table->getColumns()[1]["s1"][1] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "2" &&
                 table->getColumns()[1]["s1"][2] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"][3] == "2" &&
                 table->getColumns()[1]["s1"][3] == "4");
  Assert::IsTrue(table->getColumns()[0]["s"][4] == "3" &&
                 table->getColumns()[1]["s1"][4] == "4");
}

TEST_METHOD(TestValidParentStarSynSynSameSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "1");
}

TEST_METHOD(TestInvalidParentStarSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, 5)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(1, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
}

TEST_METHOD(TestInvalidParentStarIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(9, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
}

TEST_METHOD(TestValidParentStarWildcardSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
}

TEST_METHOD(TestValidParentStarIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(3, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidParentStarIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(90, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidParentStarWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, 86)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(2, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidParentStarIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(9, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidParentStarWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Parent*(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidUsesSynSyn) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select s such that Uses(s, v)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1" &&
                 table->getColumns()[1]["v"][0] == "a");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2" &&
                 table->getColumns()[1]["v"][1] == "b");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3" &&
                 table->getColumns()[1]["v"][2] == "c");
  Assert::IsTrue(table->getColumns()[0]["s"][3] == "3" &&
                 table->getColumns()[1]["v"][3] == "b");
}

TEST_METHOD(TestValidUsesSynWildCard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select s such that Uses(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
}

TEST_METHOD(TestValidUsesSynIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select s such that Uses(s, \"b\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
}

TEST_METHOD(TestValidUsesIntSyn) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select v such that Uses(2, v)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["v"][0] == "b");
}

TEST_METHOD(TestValidUsesIntWildCard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select s such that Uses(2, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidUsesIntIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s, s1; variable v; Select s such that Uses(3, \"c\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidPatternWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(tables->getColumns()[0]["s"][2] == "3");
  Assert::IsTrue(tables->getColumns()[0]["s"][3] == "4");
}

TEST_METHOD(TestValidAssignPatternWildcardPartialMatch) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(_, _\"300\"_)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "4");
}

TEST_METHOD(TestValidAssignPatternWildcardExactMatch) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(_, \"300\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
}

TEST_METHOD(TestValidAssignPatternCharStringWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(\"b\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
}

TEST_METHOD(TestInvalidAssignPatternCharStringWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(\"k\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsFalse(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"].empty());
}

TEST_METHOD(TestValidAssignPatternCharStringPartialMatch) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(\"c\", _\"300\"_)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
}

TEST_METHOD(TestValidAssignPatternCharStringExactMatch) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s pattern s(\"c\", \"300\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
}

TEST_METHOD(TestValidPatternVarSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; variable v; Select s pattern s(v, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(tables->getColumns()[0]["s"][2] == "3");
  Assert::IsTrue(tables->getColumns()[0]["s"][3] == "3");

  Assert::IsTrue(tables->getColumns()[1]["v"][0] == "a");
  Assert::IsTrue(tables->getColumns()[1]["v"][1] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][2] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][3] == "c");
}

TEST_METHOD(TestValidAssignPatternVarSynPartialMatch) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s; variable v; Select s pattern s(v, _\"300\"_)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
  Assert::IsTrue(tables->getColumns()[1]["v"][0] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][1] == "c");
}

TEST_METHOD(TestValidAssignPatternVarSynExactMatch) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s; variable v; Select s pattern s(v, \"300\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(tables->getColumns()[0]["s"][1] == "3");
  Assert::IsTrue(tables->getColumns()[1]["v"][0] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][1] == "c");
}

TEST_METHOD(TestValidIfPatternWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("if ifs; Select ifs pattern ifs(_, _, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["ifs"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][1] == "2");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][2] == "3");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][3] == "4");
}

TEST_METHOD(TestValidIfPatternVarSyn) {
  vector<string> testS = PQLTokenizer::tokenize(
      "if ifs; variable v; Select ifs pattern ifs(v, _, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["ifs"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][1] == "1");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][2] == "2");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][3] == "2");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][4] == "4");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][5] == "4");
  Assert::IsTrue(tables->getColumns()[1]["v"][0] == "a");
  Assert::IsTrue(tables->getColumns()[1]["v"][1] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][2] == "d");
  Assert::IsTrue(tables->getColumns()[1]["v"][3] == "c");
  Assert::IsTrue(tables->getColumns()[1]["v"][4] == "b");
  Assert::IsTrue(tables->getColumns()[1]["v"][5] == "c");
}

TEST_METHOD(TestValidIfPatternVarString) {
  vector<string> testS =
      PQLTokenizer::tokenize("if ifs; Select ifs pattern ifs(\"b\", _, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["ifs"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["ifs"][1] == "4");
}

TEST_METHOD(TestInvalidIfPatternVarString) {
  vector<string> testS =
      PQLTokenizer::tokenize("if ifs; Select ifs pattern ifs(\"k\", _, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsFalse(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["ifs"].empty());
}

TEST_METHOD(TestValidWhilePatternWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("while w; Select w pattern w(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["w"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["w"][1] == "2");
  Assert::IsTrue(tables->getColumns()[0]["w"][2] == "3");
  Assert::IsTrue(tables->getColumns()[0]["w"][3] == "4");
}

TEST_METHOD(TestValidWhilePatternVarSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("while w; variable v; Select w pattern w(v, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[1]["w"][0] == "1");
  Assert::IsTrue(tables->getColumns()[1]["w"][1] == "1");
  Assert::IsTrue(tables->getColumns()[1]["w"][2] == "2");
  Assert::IsTrue(tables->getColumns()[1]["w"][3] == "2");
  Assert::IsTrue(tables->getColumns()[1]["w"][4] == "4");
  Assert::IsTrue(tables->getColumns()[1]["w"][5] == "4");
  Assert::IsTrue(tables->getColumns()[0]["v"][0] == "a");
  Assert::IsTrue(tables->getColumns()[0]["v"][1] == "b");
  Assert::IsTrue(tables->getColumns()[0]["v"][2] == "d");
  Assert::IsTrue(tables->getColumns()[0]["v"][3] == "c");
  Assert::IsTrue(tables->getColumns()[0]["v"][4] == "b");
  Assert::IsTrue(tables->getColumns()[0]["v"][5] == "c");
}

TEST_METHOD(TestValidWhilePatternVarString) {
  vector<string> testS =
      PQLTokenizer::tokenize("while w; Select w pattern w(\"b\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["w"][0] == "1");
  Assert::IsTrue(tables->getColumns()[0]["w"][1] == "4");
}

TEST_METHOD(TestInvalidWhilePatternVarString) {
  vector<string> testS =
      PQLTokenizer::tokenize("while w; Select w pattern w(\"k\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsFalse(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["w"].empty());
}

TEST_METHOD(
    TestValidAssignPatternVarSynPartialVariableSuchThatFollowsSynWildcard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s; variable v; Select s pattern s(v, _\"b\"_) such that "
      "Follows(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  auto clause = std::dynamic_pointer_cast<PatternObject>(qo[1]);
  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> tables = qo[1]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(tables->getSignificant());
  Assert::IsTrue(tables->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(tables->getColumns()[1]["v"][0] == "a");
}

TEST_METHOD(
    TestValidSuchThatFollowsWildcardIntAssignPatternWildcardPartialConstant) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign s; variable v; Select s such that Uses(s, \"b\") pattern s(_, "
      "_\"300\"_)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> usesTable =
      qo[1]->callAndProcess(dataAccessLayer);
  shared_ptr<QueryResultsTable> patternTable =
      qo[2]->callAndProcess(dataAccessLayer);

  Assert::IsTrue(usesTable->getSignificant());
  Assert::IsTrue(usesTable->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(usesTable->getColumns()[0]["s"][1] == "3");

  Assert::IsTrue(patternTable->getSignificant());
  Assert::IsTrue(patternTable->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(patternTable->getColumns()[0]["s"][1] == "4");
}

TEST_METHOD(TestValidCallsSynWildCard) {
  vector<string> testS =
      PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "a");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "b");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "c");
}

TEST_METHOD(TestValidCallsSynIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(s, \"c\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "b");
}

TEST_METHOD(TestValidCallsWildcardSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(_, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "b");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "c");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "d");
  Assert::IsTrue(table->getColumns()[0]["s"][3] == "e");
}

TEST_METHOD(TestValidCallsWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("procedure s, s1; Select s such that Calls(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidCallsWildcardIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(_, \"c\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidCallsIdentSyn) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(\"c\", s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "d");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "e");
}

TEST_METHOD(TestValidCallsIdentWildCard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(\"c\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidCallsIdentWildCard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(\"e\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidCallsIdentIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(\"c\", \"d\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidCallsIdentIdent) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure s, s1; Select s such that Calls(\"b\", \"d\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidNextSynSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, s1)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
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
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(
      !table->getSignificant());  // empty table, since s == s is not possible
}

TEST_METHOD(TestValidNextIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next(2, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
}

TEST_METHOD(TestInvalidNextIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next(7, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
}

TEST_METHOD(TestValidNextSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("stmt s, s1; Select s such that Next(s, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 1);
}

TEST_METHOD(TestInvalidNextSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next(s, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"].size() == 0);
}

TEST_METHOD(TestValidNextSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(s, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "3");
}

TEST_METHOD(TestValidNextWildCardSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(_, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["s"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["s"][1] == "3");
  Assert::IsTrue(table->getColumns()[0]["s"][2] == "4");
}

TEST_METHOD(TestValidNextIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(2, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidNextIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(86, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidNextWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(_, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidNextWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(_, 96)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidNextWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidNextIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(2, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidNextIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next(3, 2)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidNextStarIntIntNormal) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(2, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidNextStarIntIntForLoop) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(8, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidNextStarIntIntForLoopSame) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(7, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidNextStarIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(96, 3)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestInvalidNextStarIntIntInvalidFirst) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(3, 96)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidNextStarIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(14, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {
      "15"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestInvalidNextStarIntSynBigInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(100, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  unordered_set<string> expected = {};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestValidNextStarSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(s, 6)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {"4", "5", "7", "9"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestInvalidNextStarSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s; Select s such that Next*(s, 12323)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns().size() == 1);
}

TEST_METHOD(TestValidNextStarSynSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next*(s, s1)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = { "4", "5", "7", "9", "11",
    "12", "13", "14", "16"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestValidNextStarSynSynSameSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign s, s1; Select s such that Next*(s, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {"7", "9"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
  Assert::IsTrue(table->getColumns().size() == 1);
}

TEST_METHOD(TestValidSelectProcProcName) {
  vector<string> testS =
      PQLTokenizer::tokenize("procedure s, s1; Select s.procName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.procName"][0] == "a");
  Assert::IsTrue(table->getPrimaryKey() == "s.procName");
}

TEST_METHOD(TestValidSelectCallProcName) {
  vector<string> testS =
      PQLTokenizer::tokenize("call s, s1; Select s.procName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.procName"][0] == "a");
  Assert::IsTrue(table->getPrimaryKey() == "s.procName");
}

TEST_METHOD(TestValidSelectVarVarName) {
  vector<string> testS =
      PQLTokenizer::tokenize("variable s, s1; Select s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
  Assert::IsTrue(table->getPrimaryKey() == "s.varName");
}

TEST_METHOD(TestValidSelectReadVarName) {
  vector<string> testS = PQLTokenizer::tokenize("read s, s1; Select s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "c");
  Assert::IsTrue(table->getPrimaryKey() == "s.varName");
}

TEST_METHOD(TestValidSelectPrintVarName) {
  vector<string> testS =
      PQLTokenizer::tokenize("print s, s1; Select s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "e");
  Assert::IsTrue(table->getPrimaryKey() == "s.varName");
}

TEST_METHOD(TestValidSelectConstantValue) {
  vector<string> testS =
      PQLTokenizer::tokenize("constant s, s1; Select s.value");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.value"][0] == "100");
  Assert::IsTrue(table->getPrimaryKey() == "s.value");
}

TEST_METHOD(TestValidSelectStmtStmtNo) {
  vector<string> testS = PQLTokenizer::tokenize("stmt s, s1; Select s.stmt#");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  unordered_map<string_view, shared_ptr<QueryObject>> synonyms =
      p->getSynonyms();
  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.stmt#"][0] == "1");
  Assert::IsTrue(table->getPrimaryKey() == "s.stmt#");
}

TEST_METHOD(TestValidWithClauseStaticAttr) {
  vector<string> testS = PQLTokenizer::tokenize(
      "variable s, s1; Select s.varName with \"a\" = s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
}

TEST_METHOD(TestValidWithClauseAttrStatic) {
  vector<string> testS = PQLTokenizer::tokenize(
      "variable s, s1; Select s.varName with s.varName = \"a\"");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[1]["s.varName"][0] == "a");
}

TEST_METHOD(TestValidWithClauseAttrAttr) {
  vector<string> testS = PQLTokenizer::tokenize(
      "variable s, s1; Select s.varName with s.varName = s1.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {"a", "b", "c"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestValidWithClauseAttrAttrSameAttr) {
  vector<string> testS = PQLTokenizer::tokenize(
      "variable s, s1; Select s.varName with s.varName = s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {"a", "b", "c"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
}

TEST_METHOD(TestValidSelectAttrVarName) {
  vector<string> testS = PQLTokenizer::tokenize(
      "variable s, s1; Select s.varName with s.varName = s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expected = {"a", "b", "c"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expected);
  Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) == expected);
}

TEST_METHOD(TestValidSelectAttrCallProcName) {
  vector<string> testS =
      PQLTokenizer::tokenize("call s, s1; Select s.procName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expectedS = {"1", "2"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
  unordered_set<string> expectedProcName = {"a", "b"};
  Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.procName"]) ==
                 expectedProcName);
}

TEST_METHOD(TestValidSelectAttrReadVarName) {
  vector<string> testS = PQLTokenizer::tokenize("read s, s1; Select s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expectedS = {"3", "4"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
  unordered_set<string> expectedProcName = {"c", "d"};
  Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) ==
                 expectedProcName);
}

TEST_METHOD(TestValidSelectAttrPrintVarName) {
  vector<string> testS =
      PQLTokenizer::tokenize("print s, s1; Select s.varName");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  unordered_set<string> expectedS = {"1"};
  Assert::IsTrue(vectorToSet(table->getColumns()[0]["s"]) == expectedS);
  unordered_set<string> expectedProcName = {"e"};
  Assert::IsTrue(vectorToSet(table->getColumns()[1]["s.varName"]) ==
                 expectedProcName);
}

TEST_METHOD(TestValidAffectsSynSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(a, a1)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"4",  "5",  "7",  "9", "11",
                                    "12", "13", "14", "16"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);

  unordered_set<string> expectedCol1 = {"7", "9", "11", "13", "14", "15", "17"};
  vector<string> col2 = table->getColumns()[1]["a1"];
  unordered_set<string> colSet2 = vectorToSet(col2);
  Assert::IsTrue(expectedCol1 == colSet2);
}

TEST_METHOD(TestValidAffectsSynSynSame) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(a, a)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"7", "9"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);
  Assert::IsTrue(table->getNumberOfCols() == 1);
}

TEST_METHOD(TestValidAffectsSynInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(a, 13)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"4", "5", "7", "9", "11", "12"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);
}

TEST_METHOD(TestInvalidAffectsSynInt) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign a, a1; Select a such that Affects(a, 138)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
  Assert::IsTrue(table->getColumns().size() == 1);
}

TEST_METHOD(TestValidAffectsSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(a, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"4",  "5",  "7",  "9", "11",
                                    "12", "13", "14", "16"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);
}

TEST_METHOD(TestInvalidAffectsSynWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("read a, a1; Select a such that Affects(a, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(5, a)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"9", "13"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);
}

TEST_METHOD(TestInvalidAffectsIntSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("read a, a1; Select a such that Affects(5, a)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsIntInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(5, 9)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidAffectsIntInt) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign a, a1; Select a such that Affects(111, 9)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsIntWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(4, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidAffectsIntWildcard) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign a, a1; Select a such that Affects(100, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsWildcardSyn) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(_, a)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());

  unordered_set<string> expected = {"7", "9", "11", "13", "14", "15", "17"};
  vector<string> col1 = table->getColumns()[0]["a"];
  unordered_set<string> colSet1 = vectorToSet(col1);
  Assert::IsTrue(expected == colSet1);
}

TEST_METHOD(TestInvalidAffectsWildcardSyn) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign a, a1; read s; Select a such that Affects(_, s)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsWildcardInt) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(_, 7)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestInvalidAffectsWildcardInt) {
  vector<string> testS = PQLTokenizer::tokenize(
      "assign a, a1; Select a such that Affects(_, 3123)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(!table->getSignificant());
}

TEST_METHOD(TestValidAffectsWildcardWildcard) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a, a1; Select a such that Affects(_, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(std::get<1>(testObj));

  shared_ptr<DataAccessLayerAffectsStub> dataAccessLayer =
      make_shared<DataAccessLayerAffectsStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
}

TEST_METHOD(TestValidNotZeroSynonymAbstraction) {
  vector<string> testS = PQLTokenizer::tokenize(
      "Select BOOLEAN such that not Calls (\"c\", \"d\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsFalse(table->getSignificant());
}

TEST_METHOD(TestValidNotZeroSynonymWith) {
  vector<string> testS =
      PQLTokenizer::tokenize("constant c; Select c with not 5 = 10");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[0]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["c"][0] == "100");
  Assert::IsTrue(table->getColumns()[0]["c"][1] == "300");
}

TEST_METHOD(TestValidNotOneSynonymAbstraction) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure p; Select p such that not Calls*(p, \"e\")");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["p"][0] == "a");
  Assert::IsTrue(table->getColumns()[0]["p"][1] == "b");
  Assert::IsTrue(table->getColumns()[0]["p"][2] == "d");
  Assert::IsTrue(table->getColumns()[0]["p"][3] == "e");
}

TEST_METHOD(TestValidNotOneSynonymPattern) {
  vector<string> testS =
      PQLTokenizer::tokenize("while w; Select w pattern not w(\"a\", _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["w"][0] == "2");
  Assert::IsTrue(table->getColumns()[0]["w"][1] == "3");
  Assert::IsTrue(table->getColumns()[0]["w"][2] == "4");
}

TEST_METHOD(TestValidNotOneSynonymWith) {
  vector<string> testS =
      PQLTokenizer::tokenize("assign a; Select a with not 3 = a.stmt#");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["a"][0] == "1");
  Assert::IsTrue(table->getColumns()[0]["a"][1] == "2");
  Assert::IsTrue(table->getColumns()[0]["a"][2] == "4");
}

TEST_METHOD(TestValidNotTwoSynonymsAbstraction) {
  vector<string> testS = PQLTokenizer::tokenize(
      "procedure p, q; Select p such that not Calls*(p, q)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  int size = (int)table->getColumns()[0]["p"].size();
  Assert::IsTrue(size == 21);
  Assert::IsTrue(table->getColumns()[0]["p"][14] == "d");
  Assert::IsTrue(table->getColumns()[1]["q"][14] == "d");
}

TEST_METHOD(TestValidNotTwoSynonymsPattern) {
  vector<string> testS = PQLTokenizer::tokenize(
      "while w; variable v; Select <w, v> pattern not w(v, _)");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table =
      qo[2]->callAndProcess(dataAccessLayer);  // tuple is one qo
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["v"][0] == "c");
  Assert::IsTrue(table->getColumns()[0]["v"][1] == "a");
  Assert::IsTrue(table->getColumns()[0]["v"][2] == "b");
  Assert::IsTrue(table->getColumns()[0]["v"][3] == "a");
  Assert::IsTrue(table->getColumns()[0]["v"][4] == "b");
  Assert::IsTrue(table->getColumns()[0]["v"][5] == "c");
  Assert::IsTrue(table->getColumns()[0]["v"][6] == "a");
  Assert::IsTrue(table->getColumns()[1]["w"][0] == "1");
  Assert::IsTrue(table->getColumns()[1]["w"][1] == "2");
  Assert::IsTrue(table->getColumns()[1]["w"][2] == "2");
  Assert::IsTrue(table->getColumns()[1]["w"][3] == "3");
  Assert::IsTrue(table->getColumns()[1]["w"][4] == "3");
  Assert::IsTrue(table->getColumns()[1]["w"][5] == "3");
  Assert::IsTrue(table->getColumns()[1]["w"][6] == "4");
}

TEST_METHOD(TestValidNotTwoSynonymsWith) {
  vector<string> testS = PQLTokenizer::tokenize(
      "while w; constant c; Select c with not c.value = w.stmt#");
  vector<string_view> test{sToSvVector(testS)};
  shared_ptr<QueryParser> p = make_shared<QueryParser>();
  tuple<vector<string_view>, vector<string_view>> testObj =
      p->splitDeclarationQuery(test);
  vector<shared_ptr<QueryObject>> curr = p->parseDeclaration(get<0>(testObj));
  vector<shared_ptr<QueryObject>> qo = p->parseQuery(get<1>(testObj));

  shared_ptr<DataAccessLayerStub> dataAccessLayer =
      make_shared<DataAccessLayerStub>();
  shared_ptr<QueryResultsTable> table = qo[1]->callAndProcess(dataAccessLayer);
  Assert::IsTrue(table->getSignificant());
  Assert::IsTrue(table->getColumns()[0]["c"][0] == "100");
  Assert::IsTrue(table->getColumns()[0]["c"][1] == "100");
  Assert::IsTrue(table->getColumns()[0]["c"][2] == "100");
  Assert::IsTrue(table->getColumns()[0]["c"][3] == "100");
  Assert::IsTrue(table->getColumns()[0]["c"][4] == "300");
  Assert::IsTrue(table->getColumns()[0]["c"][5] == "300");
  Assert::IsTrue(table->getColumns()[0]["c"][6] == "300");
  Assert::IsTrue(table->getColumns()[0]["c"][7] == "300");
  Assert::IsTrue(table->getColumns()[1]["w"][0] == "1");
  Assert::IsTrue(table->getColumns()[1]["w"][1] == "2");
  Assert::IsTrue(table->getColumns()[1]["w"][2] == "3");
  Assert::IsTrue(table->getColumns()[1]["w"][3] == "4");
  Assert::IsTrue(table->getColumns()[1]["w"][4] == "1");
  Assert::IsTrue(table->getColumns()[1]["w"][5] == "2");
  Assert::IsTrue(table->getColumns()[1]["w"][6] == "3");
  Assert::IsTrue(table->getColumns()[1]["w"][7] == "4");
}
}
;
}
