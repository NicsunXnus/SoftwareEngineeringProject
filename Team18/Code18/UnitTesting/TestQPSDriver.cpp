#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryDriver.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include "../source/QPS/Errors/SyntaxError.h"
#include "../source/QPS/Errors/SemanticError.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSDriver)
	{
	public:


		TEST_METHOD(TestQPSDriverInvalidQueryThrownSyntaxError)
		{
			string query = "constant c Select c"; // missing semicolon
			shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
			list<string> result = driver->execute();
			shared_ptr<SyntaxErrorException> ex = make_shared<SyntaxErrorException>("");
			Assert::IsTrue(ex->getType() == result.back());
		}

		TEST_METHOD(TestQPSDriverInvalidQueryThrownSemanticError)
		{
			string query = "constant c; Select z"; // synonym not declared
			shared_ptr<QueryDriver> driver = make_shared<QueryDriver>(query);
			list<string> result = driver->execute();
			shared_ptr<SemanticErrorException> ex = make_shared<SemanticErrorException>("");
			Assert::IsTrue(ex->getType() == result.back());
		}


	};
}