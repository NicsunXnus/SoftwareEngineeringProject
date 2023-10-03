#include "stdafx.h"
#include "CppUnitTest.h"
#include <PKB.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

map<string, vector<string>> entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
map<string, vector<string>> variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
map<string, vector<string>> procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}} };
map<string, vector<string>> constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}} };
map<string, vector<string>> callProcNameData = { {"2", {"proc1"}}, {"4", {"proc2"}}, {"11", {"proc3"}} };
map<string, vector<string>> readVarNameData = { {"4", {"x"}}, {"13", {"y"}}, {"21", {"z"}} };
map<string, vector<string>> printVarNameData = { {"3", {"x"}}, {"7", {"y"}}, {"19", {"z"}} };

shared_ptr<map<string, vector<string>>> statementMap = make_shared<map<string, vector<string>>>(entityStatementData);
shared_ptr<map<string, vector<string>>> variableMap = make_shared<map<string, vector<string>>>(variableData);
shared_ptr<map<string, vector<string>>> procedureMap = make_shared<map<string, vector<string>>>(procedureData);
shared_ptr<map<string, vector<string>>> constantMap = make_shared<map<string, vector<string>>>(constantData);
shared_ptr<map<string, vector<string>>> callProcNameMap = make_shared<map<string, vector<string>>>(callProcNameData);
shared_ptr<map<string, vector<string>>> readVarNameMap = make_shared<map<string, vector<string>>>(readVarNameData);
shared_ptr<map<string, vector<string>>> printVarNameMap = make_shared<map<string, vector<string>>>(printVarNameData);

map<string, vector<string>> usesData = { {"x", {"main", "3", "6"}} };
map<string, vector<string>> modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
map<string, vector<string>> followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
map<string, vector<string>> followsData = { {"1", {"2"}}, {"2", {"3"}} };
map<string, vector<string>> parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
map<string, vector<string>> parentData = { {"10", {"7"}}, {"6", {"5"}} };
map<string, vector<string>> callsStarData = { {"main", {"proc1", "proc2", "proc3"}}, {"proc1", {"proc2", "proc3"}}, {"proc2", {"proc3"}} };
map<string, vector<string>> callsData = { {"main", {"proc1"}}, {"proc1", {"proc2"}}, {"proc2", {"proc3"}} };
map<string, vector<string>> nextDataValid = { {"1", {"2"}}, {"2", {"3"}}, {"3", {"7"}}, {"7", {"1"}}, {"5", {"6"}} };
map<string, vector<string>> nextDataInvalid = { {"1", {"2", "4"}}, {"2", {"3", "5"}}, {"3", {"7"}}, {"7", {"1"}}, {"5", {"6"}}};

shared_ptr<map<string, vector<string>>> usesMap = make_shared<map<string, vector<string>>>(usesData);
shared_ptr<map<string, vector<string>>> modifiesMap = make_shared<map<string, vector<string>>>(modifiesData);
shared_ptr<map<string, vector<string>>> followsMap = make_shared<map<string, vector<string>>>(followsStarData);
shared_ptr<map<string, vector<string>>> parentsMap = make_shared<map<string, vector<string>>>(parentStarData);
shared_ptr<map<string, vector<string>>> callsMap = make_shared<map<string, vector<string>>>(callsStarData);
shared_ptr<map<string, vector<string>>> nextMapValid = make_shared<map<string, vector<string>>>(nextDataValid);
shared_ptr<map<string, vector<string>>> nextMapInvalid = make_shared<map<string, vector<string>>>(nextDataInvalid);


namespace UnitTesting {
	TEST_CLASS(TestPKBRespondStorageManager) {
		public:
			TEST_METHOD(TestGetEntityStatements) {
				PKB::insertor.addEntity(statementMap);

				Assert::IsTrue(compare_vectors(PKB::responder.getEntityStatement(CALL), { "1", "2" }));
				Assert::IsTrue(compare_vectors(PKB::responder.getEntityStatement(PRINT), { "4" }));
				Assert::IsTrue(compare_vectors(PKB::responder.getEntityStatement(WHILE), {}));
			}

			TEST_METHOD(TestGetProcedures) {
				PKB::insertor.addEntity(procedureMap, PROCEDURE);

				Assert::IsTrue(compare_vectors(PKB::responder.getAllProcedures(), { "proc1", "proc2" }));
			}

			TEST_METHOD(TestGetVariables) {
				PKB::insertor.addEntity(variableMap, VARIABLE);

				Assert::IsTrue(compare_vectors(PKB::responder.getAllVariables(), { "x", "y" }));
				Assert::IsTrue(compare_maps(PKB::responder.getVariableMap(), variableData));
			}

			TEST_METHOD(TestGetConstants) {
				PKB::insertor.addEntity(constantMap, CONSTANT);

				Assert::IsTrue(compare_vectors(PKB::responder.getAllConstants(), { "10", "HERE" }));
				Assert::IsTrue(compare_maps(PKB::responder.getConstantMap(), constantData));
			}

			TEST_METHOD(TestGetCallProcNames) {
				PKB::insertor.addEntityNames(callProcNameMap, CALL);
				Assert::IsTrue(compare_maps(PKB::responder.getCallProcNameMap(), callProcNameData));
			}

			TEST_METHOD(TestGetReadVarNames) {
				PKB::insertor.addEntityNames(readVarNameMap, READ);
				Assert::IsTrue(compare_maps(PKB::responder.getReadVarNameMap(), readVarNameData));
			}

			TEST_METHOD(TestGetPrintVarNames) {
				PKB::insertor.addEntityNames(printVarNameMap, PRINT);
				Assert::IsTrue(compare_maps(PKB::responder.getPrintVarNameMap(), printVarNameData));
			}

			TEST_METHOD(TestGetAbstractions) {
				PKB::insertor.addAbstraction(modifiesMap, MODIFIES);
				PKB::insertor.addAbstraction(usesMap, USES);
				PKB::insertor.addAbstraction(parentsMap, PARENT);
				PKB::insertor.addAbstraction(followsMap, FOLLOWS);
				PKB::insertor.addAbstraction(callsMap, CALLS);
				PKB::insertor.addAbstraction(nextMapValid, NEXT);

				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(USES), usesData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(MODIFIES), modifiesData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(PARENT), parentData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(FOLLOWS), followsData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(PARENTSTAR), parentStarData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(FOLLOWSSTAR), followsStarData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(CALLS), callsData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(CALLSSTAR), callsStarData));
				Assert::IsTrue(compare_maps(PKB::responder.getAbstraction(NEXT), nextDataValid));
			}

			TEST_METHOD(TestGetNextAbstraction) {
				PKB::insertor.addAbstraction(nextMapInvalid, NEXT);
				// assert database contains the correct data (truncates to the valid data)
				map<string, vector<string>> news = PKB::responder.getAbstraction(NEXT); 
				
				Assert::IsTrue(compare_maps(news, nextDataValid));
			}
		};
}