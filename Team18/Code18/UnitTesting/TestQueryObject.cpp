#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryObjects/ClauseArg.h"
#include "../source/QPS/QueryObjects/SynonymObjects.h"
#include "../source/QPS/QueryObjects/BooleanQueryObject.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/QPS/QueryObjects/ComparisonQueryObject.h"
#include "../source/QPS/QueryObjects/DesignObjects.h"
#include "../source/QPS/QueryObjects/NotQueryObject.h"
#include "../source/QPS/QueryObjects/PatternClauseObject.h"
#include "../source/QPS/QueryObjects/WithClauseObject.h"
#include <cassert>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQueryObject)
	{
	public:


		TEST_METHOD(TestBooleanQueryObject)
		{
			BooleanQueryObject qo = BooleanQueryObject("BOOLEAN");
			Assert::IsTrue(0 == static_cast<int>(qo.getSynonyms()->size()));
		}

		TEST_METHOD(TestClauseObject)
		{	
			shared_ptr<SynonymObject> synA = make_shared<SynonymObject>(SynonymObject("a", ASSIGN));
			shared_ptr<SynonymObject> synB = make_shared<SynonymObject>(SynonymObject("b", ASSIGN));
			shared_ptr<ClauseArg> intArg = make_shared<ClauseArg>(ClauseArg("1"));
			shared_ptr<ClauseArg> synArgA = make_shared<ClauseArg>(ClauseArg("a", synA));
			shared_ptr<ClauseArg> synArgB = make_shared<ClauseArg>(ClauseArg("b", synB));


			FollowsObject qo0 = FollowsObject("Follows"sv, intArg, intArg);
			Assert::IsTrue(0 == static_cast<int>(qo0.getSynonyms()->size()));

			FollowsObject qo1 = FollowsObject("Follows"sv, synArgA, intArg);
			Assert::IsTrue(1 == static_cast<int>(qo1.getSynonyms()->size()));

			FollowsObject qo11 = FollowsObject("Follows"sv, synArgA, synArgA);
			Assert::IsTrue(1 == static_cast<int>(qo11.getSynonyms()->size()));

			FollowsObject qo2 = FollowsObject("Follows"sv, synArgA, synArgB);
			Assert::IsTrue(2 == static_cast<int>(qo2.getSynonyms()->size()));
		}

		TEST_METHOD(TestPatternObject)
		{
			shared_ptr<SynonymObject> synA = make_shared<SynonymObject>(SynonymObject("a", ASSIGN));
			shared_ptr<SynonymObject> synV = make_shared<SynonymObject>(SynonymObject("v", VARIABLE));
			shared_ptr<ClauseArg> wcArg = make_shared<ClauseArg>(ClauseArg("_"));
			shared_ptr<ClauseArg> synArgA = make_shared<ClauseArg>(ClauseArg("a", synA));
			shared_ptr<ClauseArg> synArgV = make_shared<ClauseArg>(ClauseArg("v", synV));

			vector<shared_ptr<ClauseArg>> vec0 = { synArgA, wcArg, wcArg };
			AssignPatternObject qo0 = AssignPatternObject("pattern"sv, vec0);
			Assert::IsTrue(1 == static_cast<int>(qo0.getSynonyms()->size()));

			vector<shared_ptr<ClauseArg>> vec1 = { synArgA, synArgV, wcArg };
			AssignPatternObject qo1 = AssignPatternObject("pattern"sv, vec1);
			Assert::IsTrue(2 == static_cast<int>(qo1.getSynonyms()->size()));
		}

		TEST_METHOD(TestDesignObject)
		{
			StmtObject stmtS = StmtObject("s"sv);

			Assert::IsTrue(1 == static_cast<int>(stmtS.getSynonyms()->size()));
		}

		TEST_METHOD(TestAttrRefObject)
		{	
			shared_ptr<SynonymObject> synA = make_shared<SynonymObject>(SynonymObject("a", ASSIGN));
			shared_ptr<ClauseArg> synArgA = make_shared<ClauseArg>(ClauseArg("a", synA));

			StmtNoObject stmtS = StmtNoObject("a"sv, synArgA);

			Assert::IsTrue(1 == static_cast<int>(stmtS.getSynonyms()->size()));
		}
	};
}