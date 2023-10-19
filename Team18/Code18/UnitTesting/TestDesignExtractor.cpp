#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/SP/DesignExtractor/DesignExtractor.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/SimpleProcessor/ProcessedProgram.h"
#include "../source/SP/AST/Node.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
     TEST_CLASS(TestDesignExtractor){
           private:
           std::string sourceCodeDemo3 =
           "procedure procedure1 {"
                "    call procedure2;"
                "    read x;"
                "    print y;"
                "    z = x + y;"
                ""
                "    while (z == 0) {"
                "        x = x + 1;"
                "        print x;"
                "        if (x != y + 20) then {"
                "            z = y % 2;"
                "            print z;"
                "            call procedure2;"
                "        } else {"
                "            call procedure3;" 
                "        }"
                "    }"
                "}"
                ""
                "procedure procedure2 {"
                "    a = b + c * d / e - f;" 
                "    print a;"
                "    read f;"
                ""
                "    if (f == 1) then {"
                "        f = a;"
                "    } else {"
                "        read f;"
                "    }"
                ""
                "    while (f == 0) {"
                "        read f;" 
                "    }"
                "}"
                ""
                "procedure procedure3 {"
                "    if (x == 0) then {" 
                "        call procedure5;"
                "        x = y;"
                "    } else {"
                "        call procedure4;"
                "        y = y + 1;" 
                "    }"
                "}"
                ""
                "procedure procedure4 {"
                "    while (x - 3 >= y) {" 
                "        call procedure2;"
                "        y = 100 + 50 + 100;" 
                "    }"
                "}"
                ""
                "procedure procedure5 {"
                "    while (x <= 100) {" 
                "        read x;"
                "        call procedure4;"
                "        if (x != 25) then {"
                "            x = x + 1;"
                "            call procedure4;"
                "        } else {"
                "            call procedure2;" 
                "        }"
                "    }"
                "}";
           public :
           TEST_METHOD(TestEntityExtraction) {
                std::string sourceCode =
                    "procedure procedure1 {"
                    "    read x;"
                    "    print y;"
                    "    z = 10 + 20;"
                    "    print x;"
                    "    read x;"
                    "    if (z == 10) then {"
                    "        z = 20 + 20;"
                    "        read y;"
                    "    } else {"
                    "        z = 30 + 40;"
                    "        print x;"
                    "    }"
                    "    a = 10;"
                    "    while (a == 10) {"
                    "        print a;"
                    "        call procedure2;"
                    "    }"
                    "}"
                    "procedure procedure2 {"
                    "    read x;"
                    "}";
                
                std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);

                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractEntities(processedProgram);

                // Get the maps from the design extractor
                std::map<std::string, unordered_set<std::string>> procedureMap = *(designExtractor.getProcedureSetMap());
                std::map<std::string, unordered_set<std::string>> statementMap = *(designExtractor.getStatementSetMap());
                std::map<std::string, unordered_set<std::string>> variableMap = *(designExtractor.getVariableSetMap());
                std::map<std::string, unordered_set<std::string>> constantMap = *(designExtractor.getConstantSetMap());

                std::map<std::string, shared_ptr<Node>> patternMap = *(designExtractor.getPatternMap());

                Assert::AreEqual(procedureMap["procedure1"].empty(), true);

                Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(1)) != statementMap["read"].end());
                Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(5)) != statementMap["read"].end());
                Assert::IsTrue(std::find(statementMap["read"].begin(), statementMap["read"].end(), std::to_string(8)) != statementMap["read"].end());

                Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(2)) != statementMap["print"].end());
                Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(4)) != statementMap["print"].end());
                Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(10)) != statementMap["print"].end());
                Assert::IsTrue(std::find(statementMap["print"].begin(), statementMap["print"].end(), std::to_string(13)) != statementMap["print"].end());

                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(1)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(2)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(3)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(4)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(5)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(6)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(7)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(8)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(9)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(10)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(11)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(12)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(13)) != statementMap["stmt"].end());
                Assert::IsTrue(std::find(statementMap["stmt"].begin(), statementMap["stmt"].end(), std::to_string(14)) != statementMap["stmt"].end());

                Assert::IsTrue(std::find(statementMap["if"].begin(), statementMap["if"].end(), std::to_string(6)) != statementMap["if"].end());

                Assert::IsTrue(std::find(statementMap["while"].begin(), statementMap["while"].end(), std::to_string(12)) != statementMap["while"].end());

                Assert::IsTrue(std::find(statementMap["call"].begin(), statementMap["call"].end(), std::to_string(14)) != statementMap["call"].end());

                // Check the existence of values in variableMap
                Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(1)) != variableMap["x"].end());
                Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(4)) != variableMap["x"].end());
                Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(5)) != variableMap["x"].end());
                Assert::IsTrue(std::find(variableMap["x"].begin(), variableMap["x"].end(), std::to_string(10)) != variableMap["x"].end());

                Assert::IsTrue(std::find(variableMap["y"].begin(), variableMap["y"].end(), std::to_string(2)) != variableMap["y"].end());
                Assert::IsTrue(std::find(variableMap["y"].begin(), variableMap["y"].end(), std::to_string(8)) != variableMap["y"].end());

                Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(3)) != variableMap["z"].end());
                Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(6)) != variableMap["z"].end());
                Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(7)) != variableMap["z"].end());
                Assert::IsTrue(std::find(variableMap["z"].begin(), variableMap["z"].end(), std::to_string(9)) != variableMap["z"].end());

                Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(11)) != variableMap["a"].end());
                Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(12)) != variableMap["a"].end());
                Assert::IsTrue(std::find(variableMap["a"].begin(), variableMap["a"].end(), std::to_string(13)) != variableMap["a"].end());

                // Check the existence of values in constantMap
                Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(3)) != constantMap["10"].end());
                Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(6)) != constantMap["10"].end());
                Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(11)) != constantMap["10"].end());
                Assert::IsTrue(std::find(constantMap["10"].begin(), constantMap["10"].end(), std::to_string(12)) != constantMap["10"].end());

                Assert::IsTrue(std::find(constantMap["20"].begin(), constantMap["20"].end(), std::to_string(3)) != constantMap["20"].end());
                Assert::IsTrue(std::find(constantMap["20"].begin(), constantMap["20"].end(), std::to_string(7)) != constantMap["20"].end());

                Assert::IsTrue(std::find(constantMap["30"].begin(), constantMap["30"].end(), std::to_string(9)) != constantMap["30"].end());

                Assert::IsTrue(std::find(constantMap["40"].begin(), constantMap["40"].end(), std::to_string(9)) != constantMap["40"].end());

                // Check the existence of keys for 3, 6, 7, 9, 11, 12 in patternMap
                Assert::AreEqual(static_cast<int>(patternMap.size()), 6);
                Assert::IsTrue(patternMap.find("3") != patternMap.end());
                Assert::IsTrue(patternMap.find("6") != patternMap.end());
                Assert::IsTrue(patternMap.find("7") != patternMap.end());
                Assert::IsTrue(patternMap.find("9") != patternMap.end());
                Assert::IsTrue(patternMap.find("11") != patternMap.end());
                Assert::IsTrue(patternMap.find("12") != patternMap.end());
            }
            TEST_METHOD(TestWithExtraction){
                std::string sourceCode =
                    "procedure p {"
                    "if (x == 3) then {"
                    "read num1;"
                    "print num3;"
                    "sum = num1 + num2 + num3;}"
                    "else{"
                    "read num2;"
                    "}"
                    "while (y == 4){"
                    "print num3;"
                    "read num1;"
                    "sum = num1 + num2 + num3;"
                    "call q;"
                    "}}"
                    "procedure q {"
                    "read num1;"
                    "print num3;"
                    "call r;"
                    "}"
                    "procedure r {"
                    "read num2;"
                    "print num1;"
                    "}";
                std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractEntities(processedProgram);
                std::map<std::string, unordered_set<std::string>> callProcNameMap = *(designExtractor.getCallProcNameMap());
                std::map<std::string, unordered_set<std::string>> readVarNameMap = *(designExtractor.getReadVarNameMap());
                std::map<std::string, unordered_set<std::string>> printVarNameMap = *(designExtractor.getPrintVarNameMap());

                // Check the existence of values in callProcNameMap
                Assert::AreEqual(static_cast<int>(callProcNameMap.size()), 2);
                Assert::IsTrue(std::find(callProcNameMap["10"].begin(), callProcNameMap["10"].end(), "q") != callProcNameMap["10"].end());
                Assert::IsTrue(std::find(callProcNameMap["13"].begin(), callProcNameMap["13"].end(), "r") != callProcNameMap["13"].end());
                
                // Check the existence of values in readVarNameMap
                Assert::AreEqual(static_cast<int>(readVarNameMap.size()), 5);
                Assert::IsTrue(std::find(readVarNameMap["2"].begin(), readVarNameMap["2"].end(), "num1") != readVarNameMap["2"].end());  
                Assert::IsTrue(std::find(readVarNameMap["5"].begin(), readVarNameMap["5"].end(), "num2") != readVarNameMap["5"].end());  
                Assert::IsTrue(std::find(readVarNameMap["8"].begin(), readVarNameMap["8"].end(), "num1") != readVarNameMap["8"].end());  
                Assert::IsTrue(std::find(readVarNameMap["11"].begin(), readVarNameMap["11"].end(), "num1") != readVarNameMap["11"].end());  
                Assert::IsTrue(std::find(readVarNameMap["14"].begin(), readVarNameMap["14"].end(), "num2") != readVarNameMap["14"].end());  

                // Check the existence of values in printVarNameMap
                Assert::AreEqual(static_cast<int>(printVarNameMap.size()), 4);
                Assert::IsTrue(std::find(printVarNameMap["3"].begin(), printVarNameMap["3"].end(), "num3") != printVarNameMap["3"].end());
                Assert::IsTrue(std::find(printVarNameMap["7"].begin(), printVarNameMap["7"].end(), "num3") != printVarNameMap["7"].end());  
                Assert::IsTrue(std::find(printVarNameMap["12"].begin(), printVarNameMap["12"].end(), "num3") != printVarNameMap["12"].end());  
                Assert::IsTrue(std::find(printVarNameMap["15"].begin(), printVarNameMap["15"].end(), "num1") != printVarNameMap["15"].end()); 
            }
           TEST_METHOD(TestSimpleParentsFollowsAbstraction)
		    {
                std::string sourceCode =
                    "procedure procedure1 {"
                    "    read x;"
                    "    print y;"
                    "    z = 10 + 20;"
                    "    print x;"
                    "    read x;"
                    "    if (z == 10) then {"
                    "        z = 20 + 20;"
                    "        read y;"
                    "    } else {"
                    "        z = 30 + 40;"
                    "        print x;"
                    "    }"
                    "    a = 10;"
                    "    while (a == 10) {"
                    "        print a;"
                    "        call procedure2;"
                    "    }"
                    "}"
                    "procedure procedure2 {"
                    "    read x;"
                    "}";
                
                std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
                
                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractParents(processedProgram);
                designExtractor.extractFollows(processedProgram);

                std::map<std::string, unordered_set<std::string>> parentsMap = *(designExtractor.getParentMap());
                std::map<std::string, unordered_set<std::string>> parentsStarMap = *(designExtractor.getParentStarMap());
                std::map<std::string, unordered_set<std::string>> followsMap = *(designExtractor.getFollowsMap());
                std::map<std::string, unordered_set<std::string>> followsStarMap = *(designExtractor.getFollowsStarMap());
 
                 // Check if the parentsMap is extracted correctly
                 Assert::AreEqual(parentsMap["1"].empty(), true);
                 Assert::AreEqual(parentsMap["2"].empty(), true);
                 Assert::AreEqual(parentsMap["3"].empty(), true);
                 Assert::AreEqual(parentsMap["4"].empty(), true);
                 Assert::AreEqual(parentsMap["5"].empty(), true);
                 Assert::AreEqual(parentsMap["6"].empty(), true);
                 Assert::AreEqual(parentsMap["11"].empty(), true);
                 Assert::AreEqual(parentsMap["12"].empty(), true);

                // Check the values of parentsMap
                Assert::IsTrue(std::find(parentsMap["7"].begin(), parentsMap["7"].end(), "6") != parentsMap["7"].end());
                Assert::IsTrue(std::find(parentsMap["8"].begin(), parentsMap["8"].end(), "6") != parentsMap["8"].end());
                Assert::IsTrue(std::find(parentsMap["9"].begin(), parentsMap["9"].end(), "6") != parentsMap["9"].end());
                Assert::IsTrue(std::find(parentsMap["10"].begin(), parentsMap["10"].end(), "6") != parentsMap["10"].end());
                Assert::IsTrue(std::find(parentsMap["13"].begin(), parentsMap["13"].end(), "12") != parentsMap["13"].end());
                Assert::IsTrue(std::find(parentsMap["14"].begin(), parentsMap["14"].end(), "12") != parentsMap["14"].end());

                // Check the values of parentsStarMap
                Assert::IsTrue(std::find(parentsStarMap["7"].begin(), parentsStarMap["7"].end(), "6") != parentsStarMap["7"].end());
                Assert::IsTrue(std::find(parentsStarMap["8"].begin(), parentsStarMap["8"].end(), "6") != parentsStarMap["8"].end());
                Assert::IsTrue(std::find(parentsStarMap["9"].begin(), parentsStarMap["9"].end(), "6") != parentsStarMap["9"].end());
                Assert::IsTrue(std::find(parentsStarMap["10"].begin(), parentsStarMap["10"].end(), "6") != parentsStarMap["10"].end());
                Assert::IsTrue(std::find(parentsStarMap["13"].begin(), parentsStarMap["13"].end(), "12") != parentsStarMap["13"].end());
                Assert::IsTrue(std::find(parentsStarMap["14"].begin(), parentsStarMap["14"].end(), "12") != parentsStarMap["14"].end());
                
                // Check if the followsMap is extracted correctly
                Assert::IsTrue(std::find(followsMap["1"].begin(), followsMap["1"].end(), "2") != followsMap["1"].end());
                Assert::IsTrue(std::find(followsMap["2"].begin(), followsMap["2"].end(), "3") != followsMap["2"].end());
                Assert::IsTrue(std::find(followsMap["3"].begin(), followsMap["3"].end(), "4") != followsMap["3"].end());
                Assert::IsTrue(std::find(followsMap["4"].begin(), followsMap["4"].end(), "5") != followsMap["4"].end());
                Assert::IsTrue(std::find(followsMap["5"].begin(), followsMap["5"].end(), "6") != followsMap["5"].end());
                Assert::IsTrue(std::find(followsMap["6"].begin(), followsMap["6"].end(), "11") != followsMap["6"].end());
                Assert::IsTrue(std::find(followsMap["11"].begin(), followsMap["11"].end(), "12") != followsMap["11"].end());
                Assert::IsTrue(std::find(followsMap["7"].begin(), followsMap["7"].end(), "8") != followsMap["7"].end());
                Assert::IsTrue(std::find(followsMap["9"].begin(), followsMap["9"].end(), "10") != followsMap["9"].end());
                Assert::IsTrue(std::find(followsMap["13"].begin(), followsMap["13"].end(), "14") != followsMap["13"].end());

                // Check if the followsStarMap is extracted correctly
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "2") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "3") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "4") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "5") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "6") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "11") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "12") != followsStarMap["1"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "3") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "4") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "5") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "6") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "11") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "12") != followsStarMap["2"].end());
                Assert::IsTrue(std::find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "4") != followsStarMap["3"].end());
                Assert::IsTrue(std::find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "5") != followsStarMap["3"].end());
                Assert::IsTrue(std::find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "6") != followsStarMap["3"].end());
                Assert::IsTrue(std::find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "11") != followsStarMap["3"].end());
                Assert::IsTrue(std::find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "12") != followsStarMap["3"].end());
                Assert::IsTrue(std::find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "5") != followsStarMap["4"].end());
                Assert::IsTrue(std::find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "6") != followsStarMap["4"].end());
                Assert::IsTrue(std::find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "11") != followsStarMap["4"].end());
                Assert::IsTrue(std::find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "12") != followsStarMap["4"].end());
                Assert::IsTrue(std::find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "6") != followsStarMap["5"].end());
                Assert::IsTrue(std::find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "11") != followsStarMap["5"].end());
                Assert::IsTrue(std::find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "12") != followsStarMap["5"].end());
                Assert::IsTrue(std::find(followsStarMap["6"].begin(), followsStarMap["6"].end(), "11") != followsStarMap["6"].end());
                Assert::IsTrue(std::find(followsStarMap["6"].begin(), followsStarMap["6"].end(), "12") != followsStarMap["6"].end());
                Assert::IsTrue(std::find(followsStarMap["11"].begin(), followsStarMap["11"].end(), "12") != followsStarMap["11"].end());
                Assert::IsTrue(std::find(followsStarMap["7"].begin(), followsStarMap["7"].end(), "8") != followsStarMap["7"].end());
                Assert::IsTrue(std::find(followsStarMap["9"].begin(), followsStarMap["9"].end(), "10") != followsStarMap["9"].end());
                Assert::IsTrue(std::find(followsStarMap["13"].begin(), followsStarMap["13"].end(), "14") != followsStarMap["13"].end());
                Assert::AreEqual(followsStarMap["8"].empty(), true);
                Assert::AreEqual(followsStarMap["10"].empty(), true);
                Assert::AreEqual(followsStarMap["12"].empty(), true);
                Assert::AreEqual(followsStarMap["14"].empty(), true);
                Assert::AreEqual(followsStarMap["15"].empty(), true);
		}

        TEST_METHOD(TestProcedureLineNumberExtractor)
        {
            std::string sourceCode = sourceCodeDemo3;
            std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractEntities(processedProgram);

            std::map<std::string, std::pair<std::string, std::string>> procedureStatementStorageMap = *(designExtractor.getProcedureStatementStorageMap());

            // Check the existence of values in procedureStatementStorageMap
            // Check that the first value for procedure1 is 1 and the second value is 12
            Assert::AreEqual(procedureStatementStorageMap["procedure1"].first, std::to_string(1));
            Assert::AreEqual(procedureStatementStorageMap["procedure1"].second, std::to_string(12));

            // Check that the first value for procedure2 is 13 and the second value is 20
            Assert::AreEqual(procedureStatementStorageMap["procedure2"].first, std::to_string(13));
            Assert::AreEqual(procedureStatementStorageMap["procedure2"].second, std::to_string(20));

            // Check that the first value for procedure3 is 21 and the second value is 25
            Assert::AreEqual(procedureStatementStorageMap["procedure3"].first, std::to_string(21));
            Assert::AreEqual(procedureStatementStorageMap["procedure3"].second, std::to_string(25));

            // Check that the first value for procedure4 is 26 and the second value is 28
            Assert::AreEqual(procedureStatementStorageMap["procedure4"].first, std::to_string(26));
            Assert::AreEqual(procedureStatementStorageMap["procedure4"].second, std::to_string(28));

            // Check that the first value for procedure5 is 29 and the second value is 35
            Assert::AreEqual(procedureStatementStorageMap["procedure5"].first, std::to_string(29));
            Assert::AreEqual(procedureStatementStorageMap["procedure5"].second, std::to_string(35));
		}

        // Note that CallsExtractor relies on ProcedureLineNumberExtractor
        TEST_METHOD(TestCallsExtractor)
        {
            std::string sourceCode = sourceCodeDemo3;

            std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractCalls(processedProgram);

            std::map<std::string, unordered_set<std::string>> callsMap = *(designExtractor.getCallsMap());
            std::map<std::string, unordered_set<std::string>> callsStarMap = *(designExtractor.getCallsStarMap());

            // Check the existence of values in callsMap
            Assert::AreEqual(static_cast<int>(callsMap.size()), 4);
            Assert::AreEqual(static_cast<int>(callsMap["procedure1"].size()), 2);
            Assert::IsTrue(std::find(callsMap["procedure1"].begin(), callsMap["procedure1"].end(), "procedure2") != callsMap["procedure1"].end());
            Assert::IsTrue(std::find(callsMap["procedure1"].begin(), callsMap["procedure1"].end(), "procedure3") != callsMap["procedure1"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure3"].size()), 2);
            Assert::IsTrue(std::find(callsMap["procedure3"].begin(), callsMap["procedure3"].end(), "procedure4") != callsMap["procedure3"].end());
            Assert::IsTrue(std::find(callsMap["procedure3"].begin(), callsMap["procedure3"].end(), "procedure5") != callsMap["procedure3"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure4"].size()), 1);
            Assert::IsTrue(std::find(callsMap["procedure4"].begin(), callsMap["procedure4"].end(), "procedure2") != callsMap["procedure4"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure5"].size()), 2);
            Assert::IsTrue(std::find(callsMap["procedure5"].begin(), callsMap["procedure5"].end(), "procedure4") != callsMap["procedure5"].end());
            Assert::IsTrue(std::find(callsMap["procedure5"].begin(), callsMap["procedure5"].end(), "procedure2") != callsMap["procedure5"].end());

            // Check the existence of values in callsStarMap
            Assert::AreEqual(static_cast<int>(callsStarMap.size()), 4);
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure1"].size()), 4);
            Assert::IsTrue(std::find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure2") != callsStarMap["procedure1"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure3") != callsStarMap["procedure1"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure4") != callsStarMap["procedure1"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure5") != callsStarMap["procedure1"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure3"].size()), 3);
            Assert::IsTrue(std::find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure4") != callsStarMap["procedure3"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure5") != callsStarMap["procedure3"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure2") != callsStarMap["procedure3"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure4"].size()), 1);
            Assert::IsTrue(std::find(callsStarMap["procedure4"].begin(), callsStarMap["procedure4"].end(), "procedure2") != callsStarMap["procedure4"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure5"].size()), 2);
            Assert::IsTrue(std::find(callsStarMap["procedure5"].begin(), callsStarMap["procedure5"].end(), "procedure4") != callsStarMap["procedure5"].end());
            Assert::IsTrue(std::find(callsStarMap["procedure5"].begin(), callsStarMap["procedure5"].end(), "procedure2") != callsStarMap["procedure5"].end());

        }

        // Note that UsesExtractor relies on CallsExtractor
        TEST_METHOD(TestUsesExtractor)
        {
            std::string sourceCode = sourceCodeDemo3;

            std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractUses(processedProgram);

            std::map<std::string, unordered_set<std::string>> usesAbstractionMap = *(designExtractor.getUsesMap());

            // Check useAbstractionMap for the key "x" and that it has values "4", "5", "6", "7", "8", "12", "21", "22", "24", "26" "29", "31", "32", "33", "34" "procedure1", "procedure3", "procedure4" "procedure5"
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "4") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "5") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "6") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "7") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "8") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "12") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "21") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "22") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "24") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "26") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "29") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "31") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "32") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "33") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "34") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure1") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure3") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure4") != usesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure5") != usesAbstractionMap["x"].end());
            Assert::AreEqual(19, (int)usesAbstractionMap["x"].size());

            // Check the number of variables (x, y, z, a, b, c, d, e, f)
            Assert::AreEqual(9, (int)usesAbstractionMap.size());

        }

        // Note that ModifiesExtractor relies on CallsExtractor
        TEST_METHOD(TestModifiesExtractor)
        {
            std::string sourceCode = sourceCodeDemo3;

            std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractModifies(processedProgram);

            std::map<std::string, unordered_set<std::string>> modifiesAbstractionMap = *(designExtractor.getModifiesMap());

            // Check modifiesAbstractionMap for the key "x" and that it has values "2", "5", "6", "8", "12", "21", "22", "23", "29", "30", "32", "33", "procedure1", "procedure3", "procedure5"
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "2") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "5") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "6") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "8") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "12") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "21") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "22") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "23") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "29") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "30") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "32") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "33") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure1") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure3") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(std::find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure5") != modifiesAbstractionMap["x"].end());
            Assert::AreEqual(15, (int)modifiesAbstractionMap["x"].size());

            // Check the number of variables (x, y, z, a, f)
            Assert::AreEqual(5, (int)modifiesAbstractionMap.size());
        }

        TEST_METHOD(TestNextExtractor)
        {
            std::string sourceCode = sourceCodeDemo3;

            std::shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractNext(processedProgram);

            std::map<std::string, unordered_set<std::string>> nextMap = *(designExtractor.getNextMap());

            // Check the values of nextMap
            Assert::IsTrue(std::find(nextMap["1"].begin(), nextMap["1"].end(), "2") != nextMap["1"].end());
            Assert::IsTrue(std::find(nextMap["2"].begin(), nextMap["2"].end(), "3") != nextMap["2"].end());
            Assert::IsTrue(std::find(nextMap["3"].begin(), nextMap["3"].end(), "4") != nextMap["3"].end());
            Assert::IsTrue(std::find(nextMap["4"].begin(), nextMap["4"].end(), "5") != nextMap["4"].end());
            Assert::IsTrue(std::find(nextMap["5"].begin(), nextMap["5"].end(), "6") != nextMap["5"].end());
            Assert::IsTrue(std::find(nextMap["6"].begin(), nextMap["6"].end(), "7") != nextMap["6"].end());
            Assert::IsTrue(std::find(nextMap["7"].begin(), nextMap["7"].end(), "8") != nextMap["7"].end());
            Assert::IsTrue(std::find(nextMap["8"].begin(), nextMap["8"].end(), "9") != nextMap["8"].end());
            Assert::IsTrue(std::find(nextMap["8"].begin(), nextMap["8"].end(), "12") != nextMap["8"].end());
            Assert::IsTrue(std::find(nextMap["9"].begin(), nextMap["9"].end(), "10") != nextMap["9"].end());
            Assert::IsTrue(std::find(nextMap["10"].begin(), nextMap["10"].end(), "11") != nextMap["10"].end());
            Assert::IsTrue(std::find(nextMap["11"].begin(), nextMap["11"].end(), "5") != nextMap["11"].end());
            Assert::IsTrue(std::find(nextMap["12"].begin(), nextMap["12"].end(), "5") != nextMap["12"].end());

            Assert::IsTrue(std::find(nextMap["13"].begin(), nextMap["13"].end(), "14") != nextMap["13"].end());
            Assert::IsTrue(std::find(nextMap["14"].begin(), nextMap["14"].end(), "15") != nextMap["14"].end());
            Assert::IsTrue(std::find(nextMap["15"].begin(), nextMap["15"].end(), "16") != nextMap["15"].end());
            Assert::IsTrue(std::find(nextMap["16"].begin(), nextMap["16"].end(), "17") != nextMap["16"].end());
            Assert::IsTrue(std::find(nextMap["16"].begin(), nextMap["16"].end(), "18") != nextMap["16"].end());
            Assert::IsTrue(std::find(nextMap["17"].begin(), nextMap["17"].end(), "19") != nextMap["17"].end());
            Assert::IsTrue(std::find(nextMap["18"].begin(), nextMap["18"].end(), "19") != nextMap["18"].end());
            Assert::IsTrue(std::find(nextMap["19"].begin(), nextMap["19"].end(), "20") != nextMap["19"].end());
            Assert::IsTrue(std::find(nextMap["20"].begin(), nextMap["20"].end(), "19") != nextMap["20"].end());

            Assert::IsTrue(std::find(nextMap["21"].begin(), nextMap["21"].end(), "22") != nextMap["21"].end());
            Assert::IsTrue(std::find(nextMap["21"].begin(), nextMap["21"].end(), "24") != nextMap["21"].end());
            Assert::IsTrue(std::find(nextMap["22"].begin(), nextMap["22"].end(), "23") != nextMap["22"].end());
            Assert::IsTrue(std::find(nextMap["24"].begin(), nextMap["24"].end(), "25") != nextMap["24"].end());

            Assert::IsTrue(std::find(nextMap["26"].begin(), nextMap["26"].end(), "27") != nextMap["26"].end());
            Assert::IsTrue(std::find(nextMap["27"].begin(), nextMap["27"].end(), "28") != nextMap["27"].end());
            Assert::IsTrue(std::find(nextMap["28"].begin(), nextMap["28"].end(), "26") != nextMap["28"].end());

            Assert::IsTrue(std::find(nextMap["29"].begin(), nextMap["29"].end(), "30") != nextMap["29"].end());
            Assert::IsTrue(std::find(nextMap["34"].begin(), nextMap["34"].end(), "29") != nextMap["34"].end());
            Assert::IsTrue(std::find(nextMap["35"].begin(), nextMap["35"].end(), "29") != nextMap["35"].end());
            Assert::IsTrue(std::find(nextMap["30"].begin(), nextMap["30"].end(), "31") != nextMap["30"].end());
            Assert::IsTrue(std::find(nextMap["31"].begin(), nextMap["31"].end(), "32") != nextMap["31"].end());
            Assert::IsTrue(std::find(nextMap["32"].begin(), nextMap["32"].end(), "33") != nextMap["32"].end());
            Assert::IsTrue(std::find(nextMap["32"].begin(), nextMap["32"].end(), "35") != nextMap["32"].end());
            Assert::IsTrue(std::find(nextMap["33"].begin(), nextMap["33"].end(), "34") != nextMap["33"].end());
            
        }
   };
     
}
