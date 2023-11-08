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
		TEST_METHOD(TestMajorTableOperationsLeftToRight) {
            vector<string> map1 = { "s3,v1", "5,y", "5,z", "7,x", "8,y", "2,y" };
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            vector<string> map3 = { "s3", "6", "5","7" };
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            vector<string> map4 = { "s1,s2", "6,20", "3,4", "10,15", "8,11"} ;
            shared_ptr<QueryResultsTable> tab3 = make_shared<QueryResultsTable>(map4);


            vector<string> map6 = { "s3,s1", "5,6", "5,8", "2,7", "2,3", "7,6"};
            shared_ptr<QueryResultsTable> tab4 = make_shared<QueryResultsTable>(map6);

            vector<string> map8 = { "s2,v1", "11,y", "20,x", "4,x", "20,y", "11,z", "10,z"};
            shared_ptr<QueryResultsTable> tab5 = make_shared<QueryResultsTable>(map8);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            shared_ptr<QueryResultsTable> immediate1 = tab1->innerJoin(tab2);
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
            vector<string> map1 = { "s3,v1", "5,y", "5,z", "7,x", "8,y", "2,y" };
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            vector<string> map3 = { "s3", "6", "5","7" };
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            vector<string> map4 = { "s1,s2", "6,20", "3,4", "10,15", "8,11" };
            shared_ptr<QueryResultsTable> tab3 = make_shared<QueryResultsTable>(map4);


            vector<string> map6 = { "s3,s1", "5,6", "5,8", "2,7", "2,3", "7,6" };
            shared_ptr<QueryResultsTable> tab4 = make_shared<QueryResultsTable>(map6);

            vector<string> map8 = { "s2,v1", "11,y", "20,x", "4,x", "20,y", "11,z", "10,z" };
            shared_ptr<QueryResultsTable> tab5 = make_shared<QueryResultsTable>(map8);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            shared_ptr<QueryResultsTable> immediate1 = tab5->crossProduct(tab4);
            immediate1->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate2 = immediate1->innerJoin(tab3);
            immediate2->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate3 = immediate2->innerJoin(tab2);
            immediate3->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate4 = immediate3->innerJoin(tab1);
            immediate4->printTable();
            cout << endl;

  cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }

        TEST_METHOD(TestCrossProduct) {
            vector<string> map1 = { "s2,v1", "5,y", "5,z", "7,x", "8,y", "2,y"};
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            vector<string> map3 = { "s3", "6", "5","7" };
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

 

            immediate1->printTable();
            cout << endl;
            immediate2->printTable();
            cout << endl;

            std::cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }

        TEST_METHOD(TestInnerJoin) {
            vector<string> map1 = { "s3,v1", "5,y", "5,z", "7,x", "8,y", "2,y" };
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            vector<string> map3 = { "s3", "6", "5","7" };
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            shared_ptr<QueryResultsTable> immediate1 = tab2->innerJoin(tab1);
            shared_ptr<QueryResultsTable> immediate2 = tab1->innerJoin(tab2);
            
            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            immediate1->printTable();
            cout << endl;
            immediate2->printTable();
            cout << endl;

            std::cout.rdbuf(oldCoutBuffer);
        }

        TEST_METHOD(TestFilterCol1Col2) {
            vector<string> map1 = { "s3,v1,s4,v2", "5,y,5,y", "5,z,5,z", "7,x,7,x1", "8,y,8,y1", "2,y,2,y1"};
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);
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


        TEST_METHOD(TestRemoveDuplicates) {
            map<string, vector<string>> map1;
            map1.insert({ "m", {"1", "2", "1", "2", "3", "2"} });

            map<string, vector<string>> map2;
            map2.insert({ "s", {"a", "a", "a", "b", "c", "d"} });

            map<string, vector<string>> map3;
            map3.insert({ "v", {"a", "a", "a", "d", "c", "b"} });

            vector<map<string, vector<string>>> columns = { map1, map2, map3 };
            shared_ptr<QueryResultsTable> table =
                make_shared<QueryResultsTable>(columns);

            shared_ptr<QueryResultsTable> new_table = table->removeDuplicates();
            vector<string> new_col_1 = new_table->getColumnData("m");
            vector<string> new_col_2 = new_table->getColumnData("s");
            vector<string> new_col_3 = new_table->getColumnData("v");
            Assert::IsTrue(compare_vectors(new_col_1, { "1", "2", "2", "3", "2" }));
            Assert::IsTrue(compare_vectors(new_col_2, { "a", "a", "b", "c", "d" }));
            Assert::IsTrue(compare_vectors(new_col_3, { "a", "a", "d", "c", "b" }));
        }

        TEST_METHOD(TestDifferenceOneColumn) {
            map<string, vector<string>> map1;
            map1.insert({ "s", {"x", "z", "y", "w"} });

            map<string, vector<string>> map2;
            map2.insert({ "s", {"a", "x", "b", "y", "w", "z", "c"} });

            vector<map<string, vector<string>>> columns1 = { map1 };
            shared_ptr<QueryResultsTable> table1 =
                make_shared<QueryResultsTable>(columns1);

            vector<map<string, vector<string>>> columns2 = { map2 };
            shared_ptr<QueryResultsTable> table2 =
                make_shared<QueryResultsTable>(columns2);

            shared_ptr<QueryResultsTable> diff_table = table1->difference(table2);
            vector<string> diff_col = diff_table->getColumnData("s");
            Assert::IsTrue(compare_vectors(diff_col, { "a", "b", "c" }));
        }

        TEST_METHOD(TestDifferenceTwoColumns) {
            map<string, vector<string>> map1;
            map1.insert({ "s", {"1", "3", "5"} });

            map<string, vector<string>> map2;
            map2.insert({ "v", {"x", "z", "y"} });

            map<string, vector<string>> map3;
            map3.insert({ "s", {"1", "2", "3", "4", "5"} });

            map<string, vector<string>> map4;
            map4.insert({ "v", {"x", "a", "z", "b", "y"} });

            vector<map<string, vector<string>>> columns1 = { map1 };
            shared_ptr<QueryResultsTable> table1 =
                make_shared<QueryResultsTable>(columns1);

            vector<map<string, vector<string>>> columns4 = { map2 };
            shared_ptr<QueryResultsTable> table4 =
                make_shared<QueryResultsTable>(columns4);

            shared_ptr<QueryResultsTable> table5 = table1->crossProduct(table4);

	};
}