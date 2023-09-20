#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/QPS/QueryObjects/ClauseObject.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"
#include "../source/QPS/QueryBuilder.h"
#include "../source/QPS/DataAccessLayerStub.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;

namespace UnitTesting

{
	TEST_CLASS(TestQPSQueryEval)
	{
	public:

		TEST_METHOD(TestValidAssign)
		{
			vector<string> testS = tokenize("assign s; Select s such that Follows(3, 2)");
			vector<string_view> test{ sToSvVector(testS) };
			shared_ptr<QueryParser> p = make_shared<QueryParser>();
			tuple<vector<string_view>, vector<string_view>> testObj = p->splitDeclarationQuery(test);
			vector<shared_ptr<QueryObject>> curr = p->validateDeclaration(get<0>(testObj));
			vector<shared_ptr<QueryObject>> qo = p->validateQuery(std::get<1>(testObj));

			
			// Create mock data to insert
			StringMap entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
			StringMap variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
			StringMap procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}} };
			StringMap constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}} };

			shared_ptr<StringMap> toInsertStatement = make_shared<StringMap>(entityStatementData);
			shared_ptr<StringMap> toInsertVariable = make_shared<StringMap>(variableData);
			shared_ptr<StringMap> toInsertProcedure = make_shared<StringMap>(procedureData);
			shared_ptr<StringMap> toInsertConstant = make_shared<StringMap>(constantData);

			// Insertion
			PKB::insertor.addEntity(toInsertStatement);
			PKB::insertor.addEntity(toInsertConstant, CONSTANT);
			PKB::insertor.addEntity(toInsertProcedure, PROCEDURE);
			PKB::insertor.addEntity(toInsertVariable, VARIABLE);
			StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
			StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
			shared_ptr<StringMap> toInsert = make_shared<StringMap>(followsStarData);

			// Insertion
			PKB::insertor.addAbstraction(toInsert, FOLLOWS);
			
			
			unordered_map<string_view, shared_ptr<QueryObject>> synonyms = p->getSynonyms();
			shared_ptr<DataAccessLayer> dataAccessLayer = make_shared<DataAccessLayerStub>();
			shared_ptr<DataAccessLayerStub> dataAccessLayerStub = std::dynamic_pointer_cast<DataAccessLayerStub>(dataAccessLayer);
			dataAccessLayerStub->insertEntity({"1", "2"});
			dataAccessLayerStub->insertClauses({ {"1", {"2"}}, {"2", {"3"}} });
			shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>(qo, synonyms, dataAccessLayerStub);
			vector<shared_ptr<QueryResultsTable>> tables = queryBuilder->buildQuery();
			int testfasf = 1;
	


		}

	};

}
