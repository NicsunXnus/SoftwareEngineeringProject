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
            unordered_set < unordered_map<string, string>,QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map1;
            unordered_map<string, string> m1;
            m1.insert({ "s3","5" }); m1.insert({ "v1","y" });
            map1.insert(m1);
            unordered_map<string, string> m2;
            m2.insert({ "s3","5" }); m2.insert({ "v1","z" });
            map1.insert(m2);
            unordered_map<string, string> m3;
            m3.insert({ "s3","7" }); m3.insert({ "v1","x" });
            map1.insert(m3);
            unordered_map<string, string> m4;
            m4.insert({ "s3","8" }); m4.insert({ "v1","y" });
            map1.insert(m4);
            unordered_map<string, string> m5;
            m5.insert({ "s3","2" }); m5.insert({ "v1","y" });
            map1.insert(m5);
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            unordered_set < unordered_map<string, string>,QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map3;
            unordered_map<string, string> m6;
            m6.insert({ "s3","6" });
            map3.insert(m6);
            unordered_map<string, string> m7;
            m7.insert({ "s3","5" });
            map3.insert(m7);
            unordered_map<string, string> m8;
            m8.insert({ "s3","7" }); 
            map3.insert(m8);
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map4;
            unordered_map<string, string> m11;
            m11.insert({ "s1","6" }); m11.insert({ "s2","20" });
            map4.insert(m11);
            unordered_map<string, string> m21;
            m21.insert({ "s1","3" }); m21.insert({ "s2","4" });
            map4.insert(m21);
            unordered_map<string, string> m31;
            m31.insert({ "s1","10" }); m31.insert({ "s2","15" });
            map4.insert(m31);
            unordered_map<string, string> m41;
            m41.insert({ "s1","8" }); m41.insert({ "s2","11" });
            map4.insert(m41);
            shared_ptr<QueryResultsTable> tab3 = make_shared<QueryResultsTable>(map4);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map6;
            unordered_map<string, string> m12;
            m12.insert({ "s3","5" }); m12.insert({ "s1","6" });
            map6.insert(m12);
            unordered_map<string, string> m22;
            m22.insert({ "s3","5" }); m22.insert({ "s1","8" });
            map6.insert(m22);
            unordered_map<string, string> m32;
            m32.insert({ "s3","2" }); m32.insert({ "s1","7" });
            map6.insert(m32);
            unordered_map<string, string> m42;
            m42.insert({ "s3","2" }); m42.insert({ "s1","3" });
            map6.insert(m42);
            unordered_map<string, string> m52;
            m52.insert({ "s3","7" }); m52.insert({ "s1","6" });
            map6.insert(m52);
            shared_ptr<QueryResultsTable> tab4 = make_shared<QueryResultsTable>(map6);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map8;
            unordered_map<string, string> m13;
            m13.insert({ "s2","11" }); m13.insert({ "v1","y" });
            map8.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s2","20" }); m23.insert({ "v1","x" });
            map8.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s2","4" }); m33.insert({ "v1","x" });
            map8.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s2","20" }); m43.insert({ "v1","y" });
            map8.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s2","11" }); m53.insert({ "v1","z" });
            map8.insert(m53);
            unordered_map<string, string> m63;
            m63.insert({ "s2","10" }); m63.insert({ "v1","z" });
            map8.insert(m63);
            shared_ptr<QueryResultsTable> tab5 = make_shared<QueryResultsTable>(map8);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            shared_ptr<QueryResultsTable> immediate1 = tab1->innerJoinSet(tab2);
            immediate1->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate2 = immediate1->crossProductSet(tab3);
            immediate2->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate3 = immediate2->innerJoinSet(tab4);
            immediate3->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate4 = immediate3->innerJoinSet(tab5);
            immediate4->printTable();
            cout << endl;

            cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
            }  // namespace UnitTesting

        TEST_METHOD(TestMajorTableOperationsRightToLeft) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map1;
            unordered_map<string, string> m1;
            m1.insert({ "s3","5" }); m1.insert({ "v1","y" });
            map1.insert(m1);
            unordered_map<string, string> m2;
            m2.insert({ "s3","5" }); m2.insert({ "v1","z" });
            map1.insert(m2);
            unordered_map<string, string> m3;
            m3.insert({ "s3","7" }); m3.insert({ "v1","x" });
            map1.insert(m3);
            unordered_map<string, string> m4;
            m4.insert({ "s3","8" }); m4.insert({ "v1","y" });
            map1.insert(m4);
            unordered_map<string, string> m5;
            m5.insert({ "s3","2" }); m5.insert({ "v1","y" });
            map1.insert(m5);
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map3;
            unordered_map<string, string> m6;
            m6.insert({ "s3","6" });
            map3.insert(m6);
            unordered_map<string, string> m7;
            m7.insert({ "s3","5" });
            map3.insert(m7);
            unordered_map<string, string> m8;
            m8.insert({ "s3","7" });
            map3.insert(m8);
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map4;
            unordered_map<string, string> m11;
            m11.insert({ "s1","6" }); m11.insert({ "s2","20" });
            map4.insert(m11);
            unordered_map<string, string> m21;
            m21.insert({ "s1","3" }); m21.insert({ "s2","4" });
            map4.insert(m21);
            unordered_map<string, string> m31;
            m31.insert({ "s1","10" }); m31.insert({ "s2","15" });
            map4.insert(m31);
            unordered_map<string, string> m41;
            m41.insert({ "s1","8" }); m41.insert({ "s2","11" });
            map4.insert(m41);
            shared_ptr<QueryResultsTable> tab3 = make_shared<QueryResultsTable>(map4);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map6;
            unordered_map<string, string> m12;
            m12.insert({ "s3","5" }); m12.insert({ "s1","6" });
            map6.insert(m12);
            unordered_map<string, string> m22;
            m22.insert({ "s3","5" }); m22.insert({ "s1","8" });
            map6.insert(m22);
            unordered_map<string, string> m32;
            m32.insert({ "s3","2" }); m32.insert({ "s1","7" });
            map6.insert(m32);
            unordered_map<string, string> m42;
            m42.insert({ "s3","2" }); m42.insert({ "s1","3" });
            map6.insert(m42);
            unordered_map<string, string> m52;
            m52.insert({ "s3","7" }); m52.insert({ "s1","6" });
            map6.insert(m52);
            shared_ptr<QueryResultsTable> tab4 = make_shared<QueryResultsTable>(map6);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map8;
            unordered_map<string, string> m13;
            m13.insert({ "s2","11" }); m13.insert({ "v1","y" });
            map8.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s2","20" }); m23.insert({ "v1","x" });
            map8.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s2","4" }); m33.insert({ "v1","x" });
            map8.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s2","20" }); m43.insert({ "v1","y" });
            map8.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s2","11" }); m53.insert({ "v1","z" });
            map8.insert(m53);
            unordered_map<string, string> m63;
            m63.insert({ "s2","10" }); m63.insert({ "v1","z" });
            map8.insert(m63);
            shared_ptr<QueryResultsTable> tab5 = make_shared<QueryResultsTable>(map8);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            shared_ptr<QueryResultsTable> immediate1 = tab5->crossProductSet(tab4);
            immediate1->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate2 = immediate1->innerJoinSet(tab3);
            immediate2->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate3 = immediate2->innerJoinSet(tab2);
            immediate3->printTable();
            cout << endl;
            shared_ptr<QueryResultsTable> immediate4 = immediate3->innerJoinSet(tab1);
            immediate4->printTable();
            cout << endl;

            cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }

        TEST_METHOD(Visualise_Duplicate_removal) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map1;
            unordered_map<string, string> m1;
            m1.insert({ "s3","5" }); m1.insert({ "v1","y" });  m1.insert({ "v1","y" });
            map1.insert(m1);
            unordered_map<string, string> m2;
            m2.insert({ "s3","5" }); m2.insert({ "v1","y" });  m1.insert({ "v1","y" });
            map1.insert(m2);
            unordered_map<string, string> m3;
            m3.insert({ "s3","7" }); m3.insert({ "v1","x" }); m3.insert({ "v1","x" });
            map1.insert(m3);
            unordered_map<string, string> m4;
            m4.insert({ "s3","8" }); m4.insert({ "v1","y" }); m4.insert({ "v1","y" });
            map1.insert(m4);
            unordered_map<string, string> m5;
            m5.insert({ "s3","8" }); m5.insert({ "v1","y" }); m5.insert({ "v1","y" });
            map1.insert(m5);
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);
            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            tab1->printTable();

            cout.rdbuf(oldCoutBuffer);

            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }

        /*TEST_METHOD(TestCrossProduct) {
            vector<string> map1 = { "s2,v1", "5,y", "5,z", "7,x", "8,y", "2,y"};
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map1);

            vector<string> map3 = { "s3", "6", "5","7" };
            shared_ptr<QueryResultsTable> tab2 = make_shared<QueryResultsTable>(map3);

            shared_ptr<QueryResultsTable> immediate1 = tab2->crossProduct(tab1);
            shared_ptr<QueryResultsTable> immediate2 = tab1->crossProduct(tab2);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

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
        }*/

        TEST_METHOD(TestFilterCol1Col2) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map8;
            unordered_map<string, string> m13;
            m13.insert({ "s3","5" }); m13.insert({ "v1","y" }); m13.insert({ "s4","5" }); m13.insert({ "v2","y" });
            map8.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s3","5" }); m23.insert({ "v1","z" }); m23.insert({ "s4","5" }); m23.insert({ "v2","z" });
            map8.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s3","7" }); m33.insert({ "v1","x" }); m33.insert({ "s4","7" }); m33.insert({ "v2","x1" });
            map8.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s3","8" }); m43.insert({ "v1","y" }); m43.insert({ "s4","8" }); m43.insert({ "v2","y1" });
            map8.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s3","2" }); m53.insert({ "v1","y" }); m53.insert({ "s4","2" }); m53.insert({ "v2","y1" });
            map8.insert(m53);
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map8);
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

        TEST_METHOD(TestDifferenceOneColumn) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map8;
            unordered_map<string, string> m1;
            m1.insert({ "s","x" });
            map8.insert(m1);
            unordered_map<string, string> m2;
            m2.insert({ "s","z" });
            map8.insert(m2);
            unordered_map<string, string> m3;
            m3.insert({ "s","y" });
            map8.insert(m3);
            unordered_map<string, string> m4;
            m4.insert({ "s","w" });
            map8.insert(m4);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map81;
            unordered_map<string, string> m13;
            m13.insert({ "s","a" });
            map81.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s","x" });
            map81.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s","b" });
            map81.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s","y" });
            map81.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s","w" });
            map81.insert(m53);
            unordered_map<string, string> m63;
            m63.insert({ "s","z" });
            map81.insert(m63);
            unordered_map<string, string> m73;
            m73.insert({ "s","c" });
            map81.insert(m73);

            shared_ptr<QueryResultsTable> table1 =
                make_shared<QueryResultsTable>(map8);


            shared_ptr<QueryResultsTable> table2 =
                make_shared<QueryResultsTable>(map81);

            shared_ptr<QueryResultsTable> diff_table = table1->difference(table2);
            vector<string> diff_col = diff_table->getColumnData("s");
            Assert::IsTrue(compare_vectors(diff_col, { "a", "b", "c" }));
        }

        TEST_METHOD(TestRenameDelete) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map8;
            unordered_map<string, string> m13;
            m13.insert({ "s3","5" }); m13.insert({ "v1","y" }); m13.insert({ "s4","5" }); m13.insert({ "v2","y" });
            map8.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s3","5" }); m23.insert({ "v1","z" }); m23.insert({ "s4","5" }); m23.insert({ "v2","z" });
            map8.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s3","7" }); m33.insert({ "v1","x" }); m33.insert({ "s4","7" }); m33.insert({ "v2","x1" });
            map8.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s3","8" }); m43.insert({ "v1","y" }); m43.insert({ "s4","8" }); m43.insert({ "v2","y1" });
            map8.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s3","2" }); m53.insert({ "v1","y" }); m53.insert({ "s4","2" }); m53.insert({ "v2","y1" });
            map8.insert(m53);
            shared_ptr<QueryResultsTable> tab1 = make_shared<QueryResultsTable>(map8);

            std::stringstream output;
            std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());

            tab1->printTable();
            cout << endl;
            tab1->renameColumn("BRANDNEW","s3");
            tab1->printTable();
            cout << endl;
            tab1->deleteColumn("s4");
            tab1->printTable();
            cout << endl;
            std::cout.rdbuf(oldCoutBuffer);
            Logger::WriteMessage("Output of table operations:\n");
            Logger::WriteMessage(output.str().c_str());
        }

        TEST_METHOD(TestDifferenceTwoColumns) {
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map1;
            unordered_map<string, string> m1;
            m1.insert({ "s","1" });
            map1.insert(m1);
            unordered_map<string, string> m2;
            m2.insert({ "s","3" });
            map1.insert(m2);
            unordered_map<string, string> m3;
            m3.insert({ "s","5" });
            map1.insert(m3);
           
            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map2;
            unordered_map<string, string> m12;
            m12.insert({ "v","x" });
            map2.insert(m12);
            unordered_map<string, string> m22;
            m22.insert({ "v","z" });
            map2.insert(m22);
            unordered_map<string, string> m32;
            m32.insert({ "v","y" });
            map2.insert(m32);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map3;
            unordered_map<string, string> m13;
            m13.insert({ "s","1" });
            map3.insert(m13);
            unordered_map<string, string> m23;
            m23.insert({ "s","2" });
            map3.insert(m23);
            unordered_map<string, string> m33;
            m33.insert({ "s","3" });
            map3.insert(m33);
            unordered_map<string, string> m43;
            m43.insert({ "s","4" });
            map3.insert(m43);
            unordered_map<string, string> m53;
            m53.insert({ "s","5" });
            map3.insert(m53);

            unordered_set < unordered_map<string, string>, QueryResultsTable::HashFunc, QueryResultsTable::EqualFunc> map4;
            unordered_map<string, string> m14;
            m14.insert({ "v","x" });
            map4.insert(m14);
            unordered_map<string, string> m24;
            m24.insert({ "v","a" });
            map4.insert(m24);
            unordered_map<string, string> m34;
            m34.insert({ "v","z" });
            map4.insert(m34);
            unordered_map<string, string> m44;
            m44.insert({ "v","b" });
            map4.insert(m44);
            unordered_map<string, string> m54;
            m54.insert({ "v","y" });
            map4.insert(m54);

            shared_ptr<QueryResultsTable> table1 =
                make_shared<QueryResultsTable>(map1);

            shared_ptr<QueryResultsTable> table4 =
                make_shared<QueryResultsTable>(map2);

            shared_ptr<QueryResultsTable> table5 = table1->crossProductSet(table4);

            shared_ptr<QueryResultsTable> table2 =
                make_shared<QueryResultsTable>(map3);

            shared_ptr<QueryResultsTable> table3 =
                make_shared<QueryResultsTable>(map4);

            shared_ptr<QueryResultsTable> diff_table = table5->difference(table2, table3);
            vector<string> diff_col_1 = diff_table->getColumnData("s");
            vector<string> diff_col_2 = diff_table->getColumnData("v");
            Assert::IsTrue(
                compare_vectors(diff_col_1, { "1", "1", "2", "2", "2", "2", "2", "3", "3",
                                             "4", "4", "4", "4", "4", "5", "5" }));
            Assert::IsTrue(
                compare_vectors(diff_col_2, { "a", "b", "x", "a", "z", "b", "y", "a", "b",
                                             "x", "a", "z", "b", "y", "a", "b" }));
        }

	};
}