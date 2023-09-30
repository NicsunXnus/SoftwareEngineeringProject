#include "stdafx.h"
#include "CppUnitTest.h"
#include <PKB.h>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

StringMap entityStatementData = { {"call", {"1", "2"}}, {"print", {"4"}} };
StringMap variableData = { {"x", {"1", "2"}}, {"y", {"2", "5", "7"}} };
StringMap procedureData = { {"proc1", {"1", "2"}}, {"proc2", {"5", "6", "7"}} };
StringMap constantData = { {"10", {"1"}}, {"HERE", {"4", "7", "10"}} };

shared_ptr<StringMap> statementMap = make_shared<StringMap>(entityStatementData);
shared_ptr<StringMap> variableMap = make_shared<StringMap>(variableData);
shared_ptr<StringMap> procedureMap = make_shared<StringMap>(procedureData);
shared_ptr<StringMap> constantMap = make_shared<StringMap>(constantData);

StringMap usesData = { {"x", {"main", "3", "6"}} };
StringMap modifiesData = { {"y", {"main", "8", "proc1", "2"}} };
StringMap followsStarData = { {"1", {"2", "3", "6", "7"}}, {"2", {"3", "6", "7"}} };
StringMap followsData = { {"1", {"2"}}, {"2", {"3"}} };
StringMap parentStarData = { {"10", {"7", "3", "1"}}, {"6", {"5", "1"}} };
StringMap parentData = { {"10", {"7"}}, {"6", {"5"}} };
StringMap callsStarData = { {"main", {"proc1", "proc2", "proc3"}}, {"proc1", {"proc2", "proc3"}}, {"proc2", {"proc3"}} };
StringMap callsData = { {"main", {"proc1"}}, {"proc1", {"proc2"}}, {"proc2", {"proc3"}} };
StringMap nextDataValid = { {"1", {"2"}}, {"2", {"3"}}, {"3", {"7"}}, {"7", {"1"}}, {"5", {"6"}} };
StringMap nextDataInvalid = { {"1", {"2", "4"}}, {"2", {"3", "5"}}, {"3", {"7"}}, {"7", {"1"}}, {"5", {"6"}}};

shared_ptr<StringMap> usesMap = make_shared<StringMap>(usesData);
shared_ptr<StringMap> modifiesMap = make_shared<StringMap>(modifiesData);
shared_ptr<StringMap> followsMap = make_shared<StringMap>(followsStarData);
shared_ptr<StringMap> parentsMap = make_shared<StringMap>(parentStarData);
shared_ptr<StringMap> callsMap = make_shared<StringMap>(callsStarData);
shared_ptr<StringMap> nextMapValid = make_shared<StringMap>(nextDataValid);
shared_ptr<StringMap> nextMapInvalid = make_shared<StringMap>(nextDataInvalid);


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
				// Assert::IsTrue(compare_vectors(PKB::responder.getProcedure("proc1"), { "1", "2" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getProcedure("proc2"), { "5", "6", "7" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getProcedure("noSuchProc"), {}));
			}

			TEST_METHOD(TestGetVariables) {
				PKB::insertor.addEntity(variableMap, VARIABLE);

				Assert::IsTrue(compare_vectors(PKB::responder.getAllVariables(), { "x", "y" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getVariable("x"), { "1", "2" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getVariable("y"), { "2", "5", "7" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getVariable("noSuchVar"), {}));
				Assert::IsTrue(compare_maps(PKB::responder.getVariableMap(), variableData));
			}

			TEST_METHOD(TestGetConstants) {
				PKB::insertor.addEntity(constantMap, CONSTANT);

				Assert::IsTrue(compare_vectors(PKB::responder.getAllConstants(), { "10", "HERE" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getConstant("10"), { "1" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getConstant("HERE"), { "4", "7", "10" }));
				// Assert::IsTrue(compare_vectors(PKB::responder.getConstant("noSuchConst"), {}));
				Assert::IsTrue(compare_maps(PKB::responder.getConstantMap(), constantData));
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
				StringMap news = PKB::responder.getAbstraction(NEXT); 
				
				Assert::IsTrue(compare_maps(news, nextDataValid));
			}
		};
}