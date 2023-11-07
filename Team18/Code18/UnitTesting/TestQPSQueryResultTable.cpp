#include "stdafx.h"
#include <cassert>

#include "../source/QPS/QueryResultsTable.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSQueryResultsTable)
	{
	public:
        //PURELY FOR VISUALISATION
		/*TEST_METHOD(TestMajorTableOperationsLeftToRight) {
            map<string, vector<string>> map1;
            map1.insert({ "s3", {"5", "5", "7", "8", "2"} });

map<string, vector<string>> map2;
map2.insert({"v1", {"y", "z", "x", "y", "y"}});

vector<map<string, vector<string>>> columnsUses = {map1, map2};
shared_ptr<QueryResultsTable> table1 =
    make_shared<QueryResultsTable>(columnsUses);

map<string, vector<string>> map3;
map3.insert({"s3", {"6", "5", "7"}});

vector<map<string, vector<string>>> columnsModEnt = {map3};
shared_ptr<QueryResultsTable> table2 =
    make_shared<QueryResultsTable>(columnsModEnt);

map<string, vector<string>> map4;
map4.insert({"s1", {"6", "3", "10", "8"}});

map<string, vector<string>> map5;
map5.insert({"s2", {"20", "4", "15", "11"}});

vector<map<string, vector<string>>> columnsFollows = {map4, map5};
shared_ptr<QueryResultsTable> tab3 =
    make_shared<QueryResultsTable>(columnsFollows);

map<string, vector<string>> map6;
map6.insert({"s3", {"5", "5", "2", "2", "7"}});

map<string, vector<string>> map7;
map7.insert({"s1", {"6", "8", "7", "3", "6"}});

vector<map<string, vector<string>>> columnsParent = {map6, map7};
shared_ptr<QueryResultsTable> tab4 =
    make_shared<QueryResultsTable>(columnsParent);

map<string, vector<string>> map8;
map8.insert({"s2", {"11", "20", "4", "20", "11", "10"}});

map<string, vector<string>> map9;
map9.insert({"v1", {"y", "x", "x", "y", "z", "z"}});

vector<map<string, vector<string>>> columnsUses2 = {map8, map9};
shared_ptr<QueryResultsTable> tab5 =
    make_shared<QueryResultsTable>(columnsUses2);

stringstream output;
streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());

shared_ptr<QueryResultsTable> immediate1 = table1->innerJoin(table2);
immediate1->printTable();
cout << endl;
shared_ptr<QueryResultsTable> immediate2 = immediate1->crossProduct(tab3);
immediate2->printTable();
cout << endl;
shared_ptr<QueryResultsTable> immediate3 = immediate2->innerJoin(tab4);
immediate3->printTable();
cout << endl;
shared_ptr<QueryResultsTable> immediate4 = immediate3->innerJoin(tab5);
immediate4->printTable();
cout << endl;

cout.rdbuf(oldCoutBuffer);

Logger::WriteMessage("Output of table operations:\n");
Logger::WriteMessage(output.str().c_str());
}  // namespace UnitTesting

TEST_METHOD(TestMajorTableOperationsRightToLeft) {
  map<string, vector<string>> map1;
  map1.insert({"s3", {"5", "5", "7", "8", "2"}});

  map<string, vector<string>> map2;
  map2.insert({"v1", {"y", "z", "x", "y", "y"}});

  vector<map<string, vector<string>>> columnsUses = {map1, map2};
  shared_ptr<QueryResultsTable> table1 =
      make_shared<QueryResultsTable>(columnsUses);

  map<string, vector<string>> map3;
  map3.insert({"s3", {"6", "5", "7"}});

  vector<map<string, vector<string>>> columnsModEnt = {map3};
  shared_ptr<QueryResultsTable> table2 =
      make_shared<QueryResultsTable>(columnsModEnt);

  map<string, vector<string>> map4;
  map4.insert({"s1", {"6", "3", "10", "8"}});

  map<string, vector<string>> map5;
  map5.insert({"s2", {"20", "4", "15", "11"}});

  vector<map<string, vector<string>>> columnsFollows = {map4, map5};
  shared_ptr<QueryResultsTable> tab3 =
      make_shared<QueryResultsTable>(columnsFollows);

  map<string, vector<string>> map6;
  map6.insert({"s3", {"5", "5", "2", "2", "7"}});

  map<string, vector<string>> map7;
  map7.insert({"s1", {"6", "8", "7", "3", "6"}});

  vector<map<string, vector<string>>> columnsParent = {map6, map7};
  shared_ptr<QueryResultsTable> tab4 =
      make_shared<QueryResultsTable>(columnsParent);

  map<string, vector<string>> map8;
  map8.insert({"s2", {"11", "20", "4", "20", "11", "10"}});

  map<string, vector<string>> map9;
  map9.insert({"v1", {"y", "x", "x", "y", "z", "z"}});

  vector<map<string, vector<string>>> columnsUses2 = {map8, map9};
  shared_ptr<QueryResultsTable> tab5 =
      make_shared<QueryResultsTable>(columnsUses2);

  stringstream output;
  streambuf* oldCoutBuffer = cout.rdbuf(output.rdbuf());

  shared_ptr<QueryResultsTable> immediate1 = tab5->crossProduct(tab4);
  immediate1->printTable();
  cout << endl;
  shared_ptr<QueryResultsTable> immediate2 = immediate1->innerJoin(tab3);
  immediate2->printTable();
  cout << endl;
  shared_ptr<QueryResultsTable> immediate3 = immediate2->innerJoin(table2);
  immediate3->printTable();
  cout << endl;
  shared_ptr<QueryResultsTable> immediate4 = immediate3->innerJoin(table1);
  immediate4->printTable();
  cout << endl;

  cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }*/

TEST_METHOD(TestCrossProduct) {
  map<string, vector<string>> map1;
  map1.insert({"s2", {"5", "5", "7", "8", "2"}});

  map<string, vector<string>> map2;
  map2.insert({"v1", {"y", "z", "x", "y", "y"}});

  vector<map<string, vector<string>>> columnsUses = {map1, map2};
  shared_ptr<QueryResultsTable> table1 =
      make_shared<QueryResultsTable>(columnsUses);

  map<string, vector<string>> map3;
  map3.insert({"s3", {"6", "5", "7"}});

  vector<map<string, vector<string>>> columnsModEnt = {map3};
  shared_ptr<QueryResultsTable> table2 =
      make_shared<QueryResultsTable>(columnsModEnt);

  shared_ptr<QueryResultsTable> immediate1 = table2->crossProduct(table1);
  shared_ptr<QueryResultsTable> immediate2 = table1->crossProduct(table2);
  bool isSame = compare_vectors_of_maps(immediate1->getColumns(),
                                        immediate2->getColumns());
  assert(isSame);
}

TEST_METHOD(TestDifferenceOneColumn) {
  map<string, vector<string>> map1;
  map1.insert({"s", {"x", "z", "y", "w"}});

  map<string, vector<string>> map2;
  map2.insert({"s", {"a", "x", "b", "y", "w", "z", "c"}});

  vector<map<string, vector<string>>> columns1 = {map1};
  shared_ptr<QueryResultsTable> table1 =
      make_shared<QueryResultsTable>(columns1);

  vector<map<string, vector<string>>> columns2 = {map2};
  shared_ptr<QueryResultsTable> table2 =
      make_shared<QueryResultsTable>(columns2);

  shared_ptr<QueryResultsTable> diff_table = table1->difference(table2);
  vector<string> diff_col = diff_table->getColumnData("s");
  Assert::IsTrue(compare_vectors(diff_col, {"a", "b", "c"}));
}

TEST_METHOD(TestDifferenceTwoColumns) {
  map<string, vector<string>> map1;
  map1.insert({"s", {"1", "3", "5"}});

  map<string, vector<string>> map2;
  map2.insert({"v", {"x", "z", "y"}});

  map<string, vector<string>> map3;
  map3.insert({"s", {"1", "2", "3", "4", "5"}});

  map<string, vector<string>> map4;
  map4.insert({"v", {"x", "a", "z", "b", "y"}});

  vector<map<string, vector<string>>> columns1 = {map1};
  shared_ptr<QueryResultsTable> table1 =
      make_shared<QueryResultsTable>(columns1);

  vector<map<string, vector<string>>> columns4 = {map2};
  shared_ptr<QueryResultsTable> table4 =
      make_shared<QueryResultsTable>(columns4);

  shared_ptr<QueryResultsTable> table5 = table1->crossProduct(table4);

  vector<map<string, vector<string>>> columns2 = {map3};
  shared_ptr<QueryResultsTable> table2 =
      make_shared<QueryResultsTable>(columns2);

  vector<map<string, vector<string>>> columns3 = {map4};
  shared_ptr<QueryResultsTable> table3 =
      make_shared<QueryResultsTable>(columns3);

  shared_ptr<QueryResultsTable> diff_table = table5->difference(table2, table3);
  vector<string> diff_col_1 = diff_table->getColumnData("s");
  vector<string> diff_col_2 = diff_table->getColumnData("v");
  Assert::IsTrue(
      compare_vectors(diff_col_1, {"1", "1", "2", "2", "2", "2", "2", "3", "3",
                                   "4", "4", "4", "4", "4", "5", "5"}));
  Assert::IsTrue(
      compare_vectors(diff_col_2, {"a", "b", "x", "a", "z", "b", "y", "a", "b",
                                   "x", "a", "z", "b", "y", "a", "b"}));
}

TEST_METHOD(TestInnerJoin) {
  map<string, vector<string>> map1;
  map1.insert({"s3", {"5", "5", "7", "8", "2"}});

  map<string, vector<string>> map2;
  map2.insert({"v1", {"y", "z", "x", "y", "y"}});

  vector<map<string, vector<string>>> columnsUses = {map1, map2};
  shared_ptr<QueryResultsTable> table1 =
      make_shared<QueryResultsTable>(columnsUses);

  map<string, vector<string>> map3;
  map3.insert({"s3", {"6", "5", "7"}});

  vector<map<string, vector<string>>> columnsModEnt = {map3};
  shared_ptr<QueryResultsTable> table2 =
      make_shared<QueryResultsTable>(columnsModEnt);

  shared_ptr<QueryResultsTable> immediate1 = table2->innerJoin(table1);
  shared_ptr<QueryResultsTable> immediate2 = table1->innerJoin(table2);
  bool isSame = compare_vectors_of_maps(immediate1->getColumns(),
                                        immediate2->getColumns());
  assert(isSame);
}

TEST_METHOD(TestFilterCol1Col2) {
  map<string, vector<string>> map1;
  map1.insert({"s3", {"5", "5", "7", "8", "2"}});

  map<string, vector<string>> map2;
  map2.insert({"v1", {"y", "z", "x", "y", "y"}});

  map<string, vector<string>> map3;
  map3.insert({"s4", {"5", "5", "7", "8", "2"}});

            map<string, vector<string>> map4;
            map4.insert({ "v2", {"y", "z", "x1", "y1", "y1"} });

            vector<map<string, vector<string>>> columnsUses = { map1,map2,map3, map4 };
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(columnsUses);
            shared_ptr<QueryResultsTable> tab2 = tab1->innerJoinOnTwoColumns("v1", "v2");
            
            tab2->getColumnData("v2");
            Assert::IsTrue(tab2->getColumnData("v1") == tab2->getColumnData("v2"));
        
        }

        TEST_METHOD(TestFilterCol1Col2Empty) {
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>();
            shared_ptr<QueryResultsTable> tab2 = tab1->innerJoinOnTwoColumns("v1", "v2");

            tab2->getColumnData("v2");
            Assert::IsTrue(tab2->getColumnData("v1") == tab2->getColumnData("v2"));

        }

	};
}