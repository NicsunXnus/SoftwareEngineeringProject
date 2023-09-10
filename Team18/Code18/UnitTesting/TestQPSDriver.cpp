#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryDriver.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSDriver)
	{
	public:


		//TEST_METHOD(TestQPSDriverValid)
		//{
		//	string query = "constant c; Select c";
		//	shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
		//	list<string> result = driver->execute();
		//	//Assert::IsTrue("2" == result.back());
		//}


	};
}