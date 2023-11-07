#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/SP/DesignExtractor/DesignExtractor.h"
#include "../source/SP/SimpleProcessor/SimpleProcessor.h"
#include "../source/SP/SimpleProcessor/ProcessedProgram.h"
#include "../source/SP/AST/Node.h"
#include "../source/ApplicationWrapper.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
     TEST_CLASS(TestDesignExtractor){
           private:
           string sourceCodeDemo3 =
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
            string sourceCodeDemo4 =
               "procedure testprocedure {"
               "    x = 10;"
               "    if (x == 10) then {"
               "        x = 20;"
               "        while(y == 10) {"
               "            y = 20;"
               "            if (z == 20) then {"
               "                z = 30;"
               "            } else {"
               "                z = 40;"
               "            }"
               "        }"
               "    } else {"
               "        x = 50;"
               "    }"
               "}";
            string sourceCodeDemo5 =
                "procedure double {"
                "    x = a + b;"
                "    z = x + y;"
                "    while ((p > 1) && (q > 2) ) {"
                "        x = p + q;"
                "        if (x > 1) then {"
                "            z = x * 3 - 2;"
                "            if (y <= 10) then {"
                "                z = x + y + 1 * 2;"
                "            } else {"
                "                read p;"
                "                call single;"
                "            }"
                "            print x;"
                "            a = 12 + x + y + c / 8;"
                "            while ((p / 10 * 9 + 11) < 11) {"
                "                a = b + 1 + x + r + e;"
                "                p = q;" 
                "                while (!(x == a + b)) {"
                "                    if (17 % y != x) then {"
                "                        a = b + 3;"
                "                    } else {"
                "                        b = a + b + p + q - 9;"
                "                    }"
                "                    read c;"
                "                    a = c * (1 * 2 * 3);"
                "                }"
                "                read a;"
                "                print b;"
                "            }"
                "            call single;"
                "            read x;"
                "            a = b - c % 26;"
                "        } else {"
                "            while (x < 8) {"
                "                x = (1 * 2 * 3) + a + b;"
                "                x = x + y + (a + b * c) * 29 * 29;"
                "                if (x >= 55555) then {"
                "                    print x;"
                "                    x = 1234567; }"
                "                else {"
                "                    print x;"
                "                    read y;"
                "                    call child2;"
                "                    a = (1 * 2 * 3) + (12 / 13 % 14) % a - 36;"
                "                }"
                "            }"
                "        }"
                "        read y;"
                "        print x;"
                "        call child;"
                "    }"
                "    if (p == q) then {"
                "        a = a + 41;"
                "        var = var + (x + y) + a - (z / 2 - (z * 2));"
                "        print var;"
                "    } else {"
                "        b = 54321 * 99 % c;"
                "        read b;"
                "        if (a == b) then {"
                "            b = b + 47 - (z * 2);"
                "        } else {"
                "            read = print;"
                "            print = read + if - while * assign / call;"
                "        }"
                "        x = 11 + ((((12)))) % 13;"
                "        y = 51*8-6%17;"
                "    }"
                "    call single;"
                "    a = x * y + b;"
                "}"

                "procedure single {"
                "    read x;"
                "    x = y * x + p + q - 55;"
                "    if (y > z) then {"
                "        call child;"
                "        call child2;"
                "    } else {"
                "        print c;"
                "    }"
                "}"

                "procedure child {"
                "    read x;"
                "}"

                "procedure child2 {"
                "    print y;"
                "}";

       public:
           TEST_METHOD(TestEntityExtraction) {
                string sourceCode =
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
                
                shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);

                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractEntities(processedProgram);

                // Get the maps from the design extractor
                map<string, unordered_set<string>> procedureMap = *(designExtractor.getProcedureSetMap());
                map<string, unordered_set<string>> statementMap = *(designExtractor.getStatementSetMap());
                map<string, unordered_set<string>> variableMap = *(designExtractor.getVariableSetMap());
                map<string, unordered_set<string>> constantMap = *(designExtractor.getConstantSetMap());

                map<string, shared_ptr<Node>> patternMap = *(designExtractor.getPatternMap());

                Assert::AreEqual(procedureMap["procedure1"].empty(), true);

                Assert::IsTrue(find(statementMap["read"].begin(), statementMap["read"].end(), to_string(1)) != statementMap["read"].end());
                Assert::IsTrue(find(statementMap["read"].begin(), statementMap["read"].end(), to_string(5)) != statementMap["read"].end());
                Assert::IsTrue(find(statementMap["read"].begin(), statementMap["read"].end(), to_string(8)) != statementMap["read"].end());

                Assert::IsTrue(find(statementMap["print"].begin(), statementMap["print"].end(), to_string(2)) != statementMap["print"].end());
                Assert::IsTrue(find(statementMap["print"].begin(), statementMap["print"].end(), to_string(4)) != statementMap["print"].end());
                Assert::IsTrue(find(statementMap["print"].begin(), statementMap["print"].end(), to_string(10)) != statementMap["print"].end());
                Assert::IsTrue(find(statementMap["print"].begin(), statementMap["print"].end(), to_string(13)) != statementMap["print"].end());

                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(1)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(2)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(3)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(4)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(5)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(6)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(7)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(8)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(9)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(10)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(11)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(12)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(13)) != statementMap["stmt"].end());
                Assert::IsTrue(find(statementMap["stmt"].begin(), statementMap["stmt"].end(), to_string(14)) != statementMap["stmt"].end());

                Assert::IsTrue(find(statementMap["if"].begin(), statementMap["if"].end(), to_string(6)) != statementMap["if"].end());

                Assert::IsTrue(find(statementMap["while"].begin(), statementMap["while"].end(), to_string(12)) != statementMap["while"].end());

                Assert::IsTrue(find(statementMap["call"].begin(), statementMap["call"].end(), to_string(14)) != statementMap["call"].end());

                // Check the existence of values in variableMap
                Assert::IsTrue(find(variableMap["x"].begin(), variableMap["x"].end(), to_string(1)) != variableMap["x"].end());
                Assert::IsTrue(find(variableMap["x"].begin(), variableMap["x"].end(), to_string(4)) != variableMap["x"].end());
                Assert::IsTrue(find(variableMap["x"].begin(), variableMap["x"].end(), to_string(5)) != variableMap["x"].end());
                Assert::IsTrue(find(variableMap["x"].begin(), variableMap["x"].end(), to_string(10)) != variableMap["x"].end());

                Assert::IsTrue(find(variableMap["y"].begin(), variableMap["y"].end(), to_string(2)) != variableMap["y"].end());
                Assert::IsTrue(find(variableMap["y"].begin(), variableMap["y"].end(), to_string(8)) != variableMap["y"].end());

                Assert::IsTrue(find(variableMap["z"].begin(), variableMap["z"].end(), to_string(3)) != variableMap["z"].end());
                Assert::IsTrue(find(variableMap["z"].begin(), variableMap["z"].end(), to_string(6)) != variableMap["z"].end());
                Assert::IsTrue(find(variableMap["z"].begin(), variableMap["z"].end(), to_string(7)) != variableMap["z"].end());
                Assert::IsTrue(find(variableMap["z"].begin(), variableMap["z"].end(), to_string(9)) != variableMap["z"].end());

                Assert::IsTrue(find(variableMap["a"].begin(), variableMap["a"].end(), to_string(11)) != variableMap["a"].end());
                Assert::IsTrue(find(variableMap["a"].begin(), variableMap["a"].end(), to_string(12)) != variableMap["a"].end());
                Assert::IsTrue(find(variableMap["a"].begin(), variableMap["a"].end(), to_string(13)) != variableMap["a"].end());

                // Check the existence of values in constantMap
                Assert::IsTrue(find(constantMap["10"].begin(), constantMap["10"].end(), to_string(3)) != constantMap["10"].end());
                Assert::IsTrue(find(constantMap["10"].begin(), constantMap["10"].end(), to_string(6)) != constantMap["10"].end());
                Assert::IsTrue(find(constantMap["10"].begin(), constantMap["10"].end(), to_string(11)) != constantMap["10"].end());
                Assert::IsTrue(find(constantMap["10"].begin(), constantMap["10"].end(), to_string(12)) != constantMap["10"].end());

                Assert::IsTrue(find(constantMap["20"].begin(), constantMap["20"].end(), to_string(3)) != constantMap["20"].end());
                Assert::IsTrue(find(constantMap["20"].begin(), constantMap["20"].end(), to_string(7)) != constantMap["20"].end());

                Assert::IsTrue(find(constantMap["30"].begin(), constantMap["30"].end(), to_string(9)) != constantMap["30"].end());

                Assert::IsTrue(find(constantMap["40"].begin(), constantMap["40"].end(), to_string(9)) != constantMap["40"].end());

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
                string sourceCode =
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
                shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractEntities(processedProgram);
                map<string, unordered_set<string>> callProcNameMap = *(designExtractor.getCallProcNameMap());
                map<string, unordered_set<string>> readVarNameMap = *(designExtractor.getReadVarNameMap());
                map<string, unordered_set<string>> printVarNameMap = *(designExtractor.getPrintVarNameMap());

                // Check the existence of values in callProcNameMap
                Assert::AreEqual(static_cast<int>(callProcNameMap.size()), 2);
                Assert::IsTrue(find(callProcNameMap["10"].begin(), callProcNameMap["10"].end(), "q") != callProcNameMap["10"].end());
                Assert::IsTrue(find(callProcNameMap["13"].begin(), callProcNameMap["13"].end(), "r") != callProcNameMap["13"].end());
                
                // Check the existence of values in readVarNameMap
                Assert::AreEqual(static_cast<int>(readVarNameMap.size()), 5);
                Assert::IsTrue(find(readVarNameMap["2"].begin(), readVarNameMap["2"].end(), "num1") != readVarNameMap["2"].end());  
                Assert::IsTrue(find(readVarNameMap["5"].begin(), readVarNameMap["5"].end(), "num2") != readVarNameMap["5"].end());  
                Assert::IsTrue(find(readVarNameMap["8"].begin(), readVarNameMap["8"].end(), "num1") != readVarNameMap["8"].end());  
                Assert::IsTrue(find(readVarNameMap["11"].begin(), readVarNameMap["11"].end(), "num1") != readVarNameMap["11"].end());  
                Assert::IsTrue(find(readVarNameMap["14"].begin(), readVarNameMap["14"].end(), "num2") != readVarNameMap["14"].end());  

                // Check the existence of values in printVarNameMap
                Assert::AreEqual(static_cast<int>(printVarNameMap.size()), 4);
                Assert::IsTrue(find(printVarNameMap["3"].begin(), printVarNameMap["3"].end(), "num3") != printVarNameMap["3"].end());
                Assert::IsTrue(find(printVarNameMap["7"].begin(), printVarNameMap["7"].end(), "num3") != printVarNameMap["7"].end());  
                Assert::IsTrue(find(printVarNameMap["12"].begin(), printVarNameMap["12"].end(), "num3") != printVarNameMap["12"].end());  
                Assert::IsTrue(find(printVarNameMap["15"].begin(), printVarNameMap["15"].end(), "num1") != printVarNameMap["15"].end()); 
            }
           TEST_METHOD(TestSimpleParentsFollowsAbstraction)
		    {
                string sourceCode =
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
                
                shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
                
                DesignExtractor designExtractor = DesignExtractor();
                designExtractor.extractParents(processedProgram);
                designExtractor.extractFollows(processedProgram);

                map<string, unordered_set<string>> parentsMap = *(designExtractor.getParentMap());
                map<string, unordered_set<string>> parentsStarMap = *(designExtractor.getParentStarMap());
                map<string, unordered_set<string>> followsMap = *(designExtractor.getFollowsMap());
                map<string, unordered_set<string>> followsStarMap = *(designExtractor.getFollowsStarMap());
 
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
                Assert::IsTrue(find(parentsMap["7"].begin(), parentsMap["7"].end(), "6") != parentsMap["7"].end());
                Assert::IsTrue(find(parentsMap["8"].begin(), parentsMap["8"].end(), "6") != parentsMap["8"].end());
                Assert::IsTrue(find(parentsMap["9"].begin(), parentsMap["9"].end(), "6") != parentsMap["9"].end());
                Assert::IsTrue(find(parentsMap["10"].begin(), parentsMap["10"].end(), "6") != parentsMap["10"].end());
                Assert::IsTrue(find(parentsMap["13"].begin(), parentsMap["13"].end(), "12") != parentsMap["13"].end());
                Assert::IsTrue(find(parentsMap["14"].begin(), parentsMap["14"].end(), "12") != parentsMap["14"].end());

                // Check the values of parentsStarMap
                Assert::IsTrue(find(parentsStarMap["7"].begin(), parentsStarMap["7"].end(), "6") != parentsStarMap["7"].end());
                Assert::IsTrue(find(parentsStarMap["8"].begin(), parentsStarMap["8"].end(), "6") != parentsStarMap["8"].end());
                Assert::IsTrue(find(parentsStarMap["9"].begin(), parentsStarMap["9"].end(), "6") != parentsStarMap["9"].end());
                Assert::IsTrue(find(parentsStarMap["10"].begin(), parentsStarMap["10"].end(), "6") != parentsStarMap["10"].end());
                Assert::IsTrue(find(parentsStarMap["13"].begin(), parentsStarMap["13"].end(), "12") != parentsStarMap["13"].end());
                Assert::IsTrue(find(parentsStarMap["14"].begin(), parentsStarMap["14"].end(), "12") != parentsStarMap["14"].end());
                
                // Check if the followsMap is extracted correctly
                Assert::IsTrue(find(followsMap["1"].begin(), followsMap["1"].end(), "2") != followsMap["1"].end());
                Assert::IsTrue(find(followsMap["2"].begin(), followsMap["2"].end(), "3") != followsMap["2"].end());
                Assert::IsTrue(find(followsMap["3"].begin(), followsMap["3"].end(), "4") != followsMap["3"].end());
                Assert::IsTrue(find(followsMap["4"].begin(), followsMap["4"].end(), "5") != followsMap["4"].end());
                Assert::IsTrue(find(followsMap["5"].begin(), followsMap["5"].end(), "6") != followsMap["5"].end());
                Assert::IsTrue(find(followsMap["6"].begin(), followsMap["6"].end(), "11") != followsMap["6"].end());
                Assert::IsTrue(find(followsMap["11"].begin(), followsMap["11"].end(), "12") != followsMap["11"].end());
                Assert::IsTrue(find(followsMap["7"].begin(), followsMap["7"].end(), "8") != followsMap["7"].end());
                Assert::IsTrue(find(followsMap["9"].begin(), followsMap["9"].end(), "10") != followsMap["9"].end());
                Assert::IsTrue(find(followsMap["13"].begin(), followsMap["13"].end(), "14") != followsMap["13"].end());

                // Check if the followsStarMap is extracted correctly
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "2") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "3") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "4") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "5") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "6") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "11") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["1"].begin(), followsStarMap["1"].end(), "12") != followsStarMap["1"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "3") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "4") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "5") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "6") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "11") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["2"].begin(), followsStarMap["2"].end(), "12") != followsStarMap["2"].end());
                Assert::IsTrue(find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "4") != followsStarMap["3"].end());
                Assert::IsTrue(find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "5") != followsStarMap["3"].end());
                Assert::IsTrue(find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "6") != followsStarMap["3"].end());
                Assert::IsTrue(find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "11") != followsStarMap["3"].end());
                Assert::IsTrue(find(followsStarMap["3"].begin(), followsStarMap["3"].end(), "12") != followsStarMap["3"].end());
                Assert::IsTrue(find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "5") != followsStarMap["4"].end());
                Assert::IsTrue(find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "6") != followsStarMap["4"].end());
                Assert::IsTrue(find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "11") != followsStarMap["4"].end());
                Assert::IsTrue(find(followsStarMap["4"].begin(), followsStarMap["4"].end(), "12") != followsStarMap["4"].end());
                Assert::IsTrue(find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "6") != followsStarMap["5"].end());
                Assert::IsTrue(find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "11") != followsStarMap["5"].end());
                Assert::IsTrue(find(followsStarMap["5"].begin(), followsStarMap["5"].end(), "12") != followsStarMap["5"].end());
                Assert::IsTrue(find(followsStarMap["6"].begin(), followsStarMap["6"].end(), "11") != followsStarMap["6"].end());
                Assert::IsTrue(find(followsStarMap["6"].begin(), followsStarMap["6"].end(), "12") != followsStarMap["6"].end());
                Assert::IsTrue(find(followsStarMap["11"].begin(), followsStarMap["11"].end(), "12") != followsStarMap["11"].end());
                Assert::IsTrue(find(followsStarMap["7"].begin(), followsStarMap["7"].end(), "8") != followsStarMap["7"].end());
                Assert::IsTrue(find(followsStarMap["9"].begin(), followsStarMap["9"].end(), "10") != followsStarMap["9"].end());
                Assert::IsTrue(find(followsStarMap["13"].begin(), followsStarMap["13"].end(), "14") != followsStarMap["13"].end());
                Assert::AreEqual(followsStarMap["8"].empty(), true);
                Assert::AreEqual(followsStarMap["10"].empty(), true);
                Assert::AreEqual(followsStarMap["12"].empty(), true);
                Assert::AreEqual(followsStarMap["14"].empty(), true);
                Assert::AreEqual(followsStarMap["15"].empty(), true);
		}

        TEST_METHOD(TestProcedureLineNumberExtractor)
        {
            string sourceCode = sourceCodeDemo3;
            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractEntities(processedProgram);

            map<string, pair<string, string>> procedureStatementStorageMap = *(designExtractor.getProcedureStatementStorageMap());

            // Check the existence of values in procedureStatementStorageMap
            // Check that the first value for procedure1 is 1 and the second value is 12
            Assert::AreEqual(procedureStatementStorageMap["procedure1"].first, to_string(1));
            Assert::AreEqual(procedureStatementStorageMap["procedure1"].second, to_string(12));

            // Check that the first value for procedure2 is 13 and the second value is 20
            Assert::AreEqual(procedureStatementStorageMap["procedure2"].first, to_string(13));
            Assert::AreEqual(procedureStatementStorageMap["procedure2"].second, to_string(20));

            // Check that the first value for procedure3 is 21 and the second value is 25
            Assert::AreEqual(procedureStatementStorageMap["procedure3"].first, to_string(21));
            Assert::AreEqual(procedureStatementStorageMap["procedure3"].second, to_string(25));

            // Check that the first value for procedure4 is 26 and the second value is 28
            Assert::AreEqual(procedureStatementStorageMap["procedure4"].first, to_string(26));
            Assert::AreEqual(procedureStatementStorageMap["procedure4"].second, to_string(28));

            // Check that the first value for procedure5 is 29 and the second value is 35
            Assert::AreEqual(procedureStatementStorageMap["procedure5"].first, to_string(29));
            Assert::AreEqual(procedureStatementStorageMap["procedure5"].second, to_string(35));
		}

        // Note that CallsExtractor relies on ProcedureLineNumberExtractor
        TEST_METHOD(TestCallsExtractor)
        {
            string sourceCode = sourceCodeDemo3;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractCalls(processedProgram);

            map<string, unordered_set<string>> callsMap = *(designExtractor.getCallsMap());
            map<string, unordered_set<string>> callsStarMap = *(designExtractor.getCallsStarMap());

            // Check the existence of values in callsMap
            Assert::AreEqual(static_cast<int>(callsMap.size()), 4);
            Assert::AreEqual(static_cast<int>(callsMap["procedure1"].size()), 2);
            Assert::IsTrue(find(callsMap["procedure1"].begin(), callsMap["procedure1"].end(), "procedure2") != callsMap["procedure1"].end());
            Assert::IsTrue(find(callsMap["procedure1"].begin(), callsMap["procedure1"].end(), "procedure3") != callsMap["procedure1"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure3"].size()), 2);
            Assert::IsTrue(find(callsMap["procedure3"].begin(), callsMap["procedure3"].end(), "procedure4") != callsMap["procedure3"].end());
            Assert::IsTrue(find(callsMap["procedure3"].begin(), callsMap["procedure3"].end(), "procedure5") != callsMap["procedure3"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure4"].size()), 1);
            Assert::IsTrue(find(callsMap["procedure4"].begin(), callsMap["procedure4"].end(), "procedure2") != callsMap["procedure4"].end());
            Assert::AreEqual(static_cast<int>(callsMap["procedure5"].size()), 2);
            Assert::IsTrue(find(callsMap["procedure5"].begin(), callsMap["procedure5"].end(), "procedure4") != callsMap["procedure5"].end());
            Assert::IsTrue(find(callsMap["procedure5"].begin(), callsMap["procedure5"].end(), "procedure2") != callsMap["procedure5"].end());

            // Check the existence of values in callsStarMap
            Assert::AreEqual(static_cast<int>(callsStarMap.size()), 4);
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure1"].size()), 4);
            Assert::IsTrue(find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure2") != callsStarMap["procedure1"].end());
            Assert::IsTrue(find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure3") != callsStarMap["procedure1"].end());
            Assert::IsTrue(find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure4") != callsStarMap["procedure1"].end());
            Assert::IsTrue(find(callsStarMap["procedure1"].begin(), callsStarMap["procedure1"].end(), "procedure5") != callsStarMap["procedure1"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure3"].size()), 3);
            Assert::IsTrue(find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure4") != callsStarMap["procedure3"].end());
            Assert::IsTrue(find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure5") != callsStarMap["procedure3"].end());
            Assert::IsTrue(find(callsStarMap["procedure3"].begin(), callsStarMap["procedure3"].end(), "procedure2") != callsStarMap["procedure3"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure4"].size()), 1);
            Assert::IsTrue(find(callsStarMap["procedure4"].begin(), callsStarMap["procedure4"].end(), "procedure2") != callsStarMap["procedure4"].end());
            Assert::AreEqual(static_cast<int>(callsStarMap["procedure5"].size()), 2);
            Assert::IsTrue(find(callsStarMap["procedure5"].begin(), callsStarMap["procedure5"].end(), "procedure4") != callsStarMap["procedure5"].end());
            Assert::IsTrue(find(callsStarMap["procedure5"].begin(), callsStarMap["procedure5"].end(), "procedure2") != callsStarMap["procedure5"].end());

        }

        // Note that UsesExtractor relies on CallsExtractor
        TEST_METHOD(TestUsesExtractor)
        {
            string sourceCode = sourceCodeDemo3;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractUses(processedProgram);

            map<string, unordered_set<string>> usesAbstractionMap = *(designExtractor.getUsesMap());

            // Check useAbstractionMap for the key "x" and that it has values "4", "5", "6", "7", "8", "12", "21", "22", "24", "26" "29", "31", "32", "33", "34" "procedure1", "procedure3", "procedure4" "procedure5"
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "4") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "5") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "6") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "7") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "8") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "12") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "21") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "22") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "24") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "26") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "29") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "31") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "32") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "33") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "34") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure1") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure3") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure4") != usesAbstractionMap["x"].end());
            Assert::IsTrue(find(usesAbstractionMap["x"].begin(), usesAbstractionMap["x"].end(), "procedure5") != usesAbstractionMap["x"].end());
            Assert::AreEqual(19, (int)usesAbstractionMap["x"].size());

            // Check the number of variables (x, y, z, a, b, c, d, e, f)
            Assert::AreEqual(9, (int)usesAbstractionMap.size());

        }

        // Note that ModifiesExtractor relies on CallsExtractor
        TEST_METHOD(TestModifiesExtractor)
        {
            string sourceCode = sourceCodeDemo3;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractModifies(processedProgram);

            map<string, unordered_set<string>> modifiesAbstractionMap = *(designExtractor.getModifiesMap());

            // Check modifiesAbstractionMap for the key "x" and that it has values "2", "5", "6", "8", "12", "21", "22", "23", "29", "30", "32", "33", "procedure1", "procedure3", "procedure5"
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "2") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "5") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "6") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "8") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "12") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "21") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "22") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "23") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "29") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "30") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "32") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "33") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure1") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure3") != modifiesAbstractionMap["x"].end());
            Assert::IsTrue(find(modifiesAbstractionMap["x"].begin(), modifiesAbstractionMap["x"].end(), "procedure5") != modifiesAbstractionMap["x"].end());
            Assert::AreEqual(15, (int)modifiesAbstractionMap["x"].size());

            // Check the number of variables (x, y, z, a, f)
            Assert::AreEqual(5, (int)modifiesAbstractionMap.size());
        }

        TEST_METHOD(TestNextExtractor)
        {
            string sourceCode = sourceCodeDemo3;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractNext(processedProgram);

            map<string, unordered_set<string>> nextMap = *(designExtractor.getNextMap());

            // Check the values of nextMap
            Assert::AreEqual(static_cast<int>(nextMap["1"].size()), 1);
            Assert::IsTrue(find(nextMap["1"].begin(), nextMap["1"].end(), "2") != nextMap["1"].end());
            Assert::AreEqual(static_cast<int>(nextMap["2"].size()), 1);
            Assert::IsTrue(find(nextMap["2"].begin(), nextMap["2"].end(), "3") != nextMap["2"].end());
            Assert::AreEqual(static_cast<int>(nextMap["3"].size()), 1);
            Assert::IsTrue(find(nextMap["3"].begin(), nextMap["3"].end(), "4") != nextMap["3"].end());
            Assert::AreEqual(static_cast<int>(nextMap["4"].size()), 1);
            Assert::IsTrue(find(nextMap["4"].begin(), nextMap["4"].end(), "5") != nextMap["4"].end());
            Assert::AreEqual(static_cast<int>(nextMap["5"].size()), 1);
            Assert::IsTrue(find(nextMap["5"].begin(), nextMap["5"].end(), "6") != nextMap["5"].end());
            Assert::AreEqual(static_cast<int>(nextMap["6"].size()), 1);
            Assert::IsTrue(find(nextMap["6"].begin(), nextMap["6"].end(), "7") != nextMap["6"].end());
            Assert::AreEqual(static_cast<int>(nextMap["7"].size()), 1);
            Assert::IsTrue(find(nextMap["7"].begin(), nextMap["7"].end(), "8") != nextMap["7"].end());
            Assert::AreEqual(static_cast<int>(nextMap["8"].size()), 2);
            Assert::IsTrue(find(nextMap["8"].begin(), nextMap["8"].end(), "9") != nextMap["8"].end());
            Assert::IsTrue(find(nextMap["8"].begin(), nextMap["8"].end(), "12") != nextMap["8"].end());
            Assert::AreEqual(static_cast<int>(nextMap["9"].size()), 1);
            Assert::IsTrue(find(nextMap["9"].begin(), nextMap["9"].end(), "10") != nextMap["9"].end());
            Assert::AreEqual(static_cast<int>(nextMap["10"].size()), 1);
            Assert::IsTrue(find(nextMap["10"].begin(), nextMap["10"].end(), "11") != nextMap["10"].end());
            Assert::AreEqual(static_cast<int>(nextMap["11"].size()), 1);
            Assert::IsTrue(find(nextMap["11"].begin(), nextMap["11"].end(), "5") != nextMap["11"].end());
            Assert::AreEqual(static_cast<int>(nextMap["12"].size()), 1);
            Assert::IsTrue(find(nextMap["12"].begin(), nextMap["12"].end(), "5") != nextMap["12"].end());

            Assert::AreEqual(static_cast<int>(nextMap["13"].size()), 1);
            Assert::IsTrue(find(nextMap["13"].begin(), nextMap["13"].end(), "14") != nextMap["13"].end());
            Assert::AreEqual(static_cast<int>(nextMap["14"].size()), 1);
            Assert::IsTrue(find(nextMap["14"].begin(), nextMap["14"].end(), "15") != nextMap["14"].end());
            Assert::AreEqual(static_cast<int>(nextMap["15"].size()), 1);
            Assert::IsTrue(find(nextMap["15"].begin(), nextMap["15"].end(), "16") != nextMap["15"].end());
            Assert::AreEqual(static_cast<int>(nextMap["16"].size()), 2);
            Assert::IsTrue(find(nextMap["16"].begin(), nextMap["16"].end(), "17") != nextMap["16"].end());
            Assert::IsTrue(find(nextMap["16"].begin(), nextMap["16"].end(), "18") != nextMap["16"].end());
            Assert::AreEqual(static_cast<int>(nextMap["17"].size()), 1);
            Assert::IsTrue(find(nextMap["17"].begin(), nextMap["17"].end(), "19") != nextMap["17"].end());
            Assert::AreEqual(static_cast<int>(nextMap["18"].size()), 1);
            Assert::IsTrue(find(nextMap["18"].begin(), nextMap["18"].end(), "19") != nextMap["18"].end());
            Assert::AreEqual(static_cast<int>(nextMap["19"].size()), 1);
            Assert::IsTrue(find(nextMap["19"].begin(), nextMap["19"].end(), "20") != nextMap["19"].end());
            Assert::AreEqual(static_cast<int>(nextMap["20"].size()), 1);
            Assert::IsTrue(find(nextMap["20"].begin(), nextMap["20"].end(), "19") != nextMap["20"].end());

            Assert::AreEqual(static_cast<int>(nextMap["21"].size()), 2);
            Assert::IsTrue(find(nextMap["21"].begin(), nextMap["21"].end(), "22") != nextMap["21"].end());
            Assert::IsTrue(find(nextMap["21"].begin(), nextMap["21"].end(), "24") != nextMap["21"].end());
            Assert::AreEqual(static_cast<int>(nextMap["22"].size()), 1);
            Assert::IsTrue(find(nextMap["22"].begin(), nextMap["22"].end(), "23") != nextMap["22"].end());
            Assert::AreEqual(static_cast<int>(nextMap["24"].size()), 1);
            Assert::IsTrue(find(nextMap["24"].begin(), nextMap["24"].end(), "25") != nextMap["24"].end());

            Assert::AreEqual(static_cast<int>(nextMap["26"].size()), 1);
            Assert::IsTrue(find(nextMap["26"].begin(), nextMap["26"].end(), "27") != nextMap["26"].end());
            Assert::AreEqual(static_cast<int>(nextMap["27"].size()), 1);
            Assert::IsTrue(find(nextMap["27"].begin(), nextMap["27"].end(), "28") != nextMap["27"].end());
            Assert::AreEqual(static_cast<int>(nextMap["28"].size()), 1);
            Assert::IsTrue(find(nextMap["28"].begin(), nextMap["28"].end(), "26") != nextMap["28"].end());

            Assert::AreEqual(static_cast<int>(nextMap["29"].size()), 1);
            Assert::IsTrue(find(nextMap["29"].begin(), nextMap["29"].end(), "30") != nextMap["29"].end());
            Assert::AreEqual(static_cast<int>(nextMap["30"].size()), 1);
            Assert::IsTrue(find(nextMap["30"].begin(), nextMap["30"].end(), "31") != nextMap["30"].end());
            Assert::AreEqual(static_cast<int>(nextMap["31"].size()), 1);
            Assert::IsTrue(find(nextMap["31"].begin(), nextMap["31"].end(), "32") != nextMap["31"].end());
            Assert::AreEqual(static_cast<int>(nextMap["32"].size()), 2);
            Assert::IsTrue(find(nextMap["32"].begin(), nextMap["32"].end(), "33") != nextMap["32"].end());
            Assert::IsTrue(find(nextMap["32"].begin(), nextMap["32"].end(), "35") != nextMap["32"].end());
            Assert::AreEqual(static_cast<int>(nextMap["33"].size()), 1);
            Assert::IsTrue(find(nextMap["33"].begin(), nextMap["33"].end(), "34") != nextMap["33"].end());
            Assert::AreEqual(static_cast<int>(nextMap["34"].size()), 1);
            Assert::IsTrue(find(nextMap["34"].begin(), nextMap["34"].end(), "29") != nextMap["34"].end());
            Assert::AreEqual(static_cast<int>(nextMap["35"].size()), 1);
            Assert::IsTrue(find(nextMap["35"].begin(), nextMap["35"].end(), "29") != nextMap["35"].end());
        }

        TEST_METHOD(TestComplexNextExtractor)
        {
            string sourceCode = sourceCodeDemo5;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractNext(processedProgram);

            map<string, unordered_set<string>> nextMap = *(designExtractor.getNextMap());

            // Check the values of nextMap
            Assert::AreEqual(static_cast<int>(nextMap["1"].size()), 1);
            Assert::IsTrue(find(nextMap["1"].begin(), nextMap["1"].end(), "2") != nextMap["1"].end());
            Assert::AreEqual(static_cast<int>(nextMap["2"].size()), 1);
            Assert::IsTrue(find(nextMap["2"].begin(), nextMap["2"].end(), "3") != nextMap["2"].end());
            Assert::AreEqual(static_cast<int>(nextMap["3"].size()), 1);
            Assert::IsTrue(find(nextMap["3"].begin(), nextMap["3"].end(), "4") != nextMap["3"].end());
            Assert::AreEqual(static_cast<int>(nextMap["4"].size()), 1);
            Assert::IsTrue(find(nextMap["4"].begin(), nextMap["4"].end(), "5") != nextMap["4"].end());
            Assert::AreEqual(static_cast<int>(nextMap["5"].size()), 2);
            Assert::IsTrue(find(nextMap["5"].begin(), nextMap["5"].end(), "6") != nextMap["5"].end());
            Assert::IsTrue(find(nextMap["5"].begin(), nextMap["5"].end(), "28") != nextMap["5"].end());
            Assert::AreEqual(static_cast<int>(nextMap["6"].size()), 1);
            Assert::IsTrue(find(nextMap["6"].begin(), nextMap["6"].end(), "7") != nextMap["6"].end());
            Assert::AreEqual(static_cast<int>(nextMap["7"].size()), 2);
            Assert::IsTrue(find(nextMap["7"].begin(), nextMap["7"].end(), "8") != nextMap["7"].end());
            Assert::IsTrue(find(nextMap["7"].begin(), nextMap["7"].end(), "9") != nextMap["7"].end());
            Assert::AreEqual(static_cast<int>(nextMap["8"].size()), 1);
            Assert::IsTrue(find(nextMap["8"].begin(), nextMap["8"].end(), "11") != nextMap["8"].end());
            Assert::AreEqual(static_cast<int>(nextMap["9"].size()), 1);
            Assert::IsTrue(find(nextMap["9"].begin(), nextMap["9"].end(), "10") != nextMap["9"].end());
            Assert::AreEqual(static_cast<int>(nextMap["10"].size()), 1);
            Assert::IsTrue(find(nextMap["10"].begin(), nextMap["10"].end(), "11") != nextMap["10"].end());
            Assert::AreEqual(static_cast<int>(nextMap["11"].size()), 1);
            Assert::IsTrue(find(nextMap["11"].begin(), nextMap["11"].end(), "12") != nextMap["11"].end());
            Assert::AreEqual(static_cast<int>(nextMap["12"].size()), 1);
            Assert::IsTrue(find(nextMap["12"].begin(), nextMap["12"].end(), "13") != nextMap["12"].end());
            Assert::AreEqual(static_cast<int>(nextMap["13"].size()), 2);
            Assert::IsTrue(find(nextMap["13"].begin(), nextMap["13"].end(), "14") != nextMap["13"].end());
            Assert::IsTrue(find(nextMap["13"].begin(), nextMap["13"].end(), "24") != nextMap["13"].end());
            Assert::AreEqual(static_cast<int>(nextMap["14"].size()), 1);
            Assert::IsTrue(find(nextMap["14"].begin(), nextMap["14"].end(), "15") != nextMap["14"].end());
            Assert::AreEqual(static_cast<int>(nextMap["15"].size()), 1);
            Assert::IsTrue(find(nextMap["15"].begin(), nextMap["15"].end(), "16") != nextMap["15"].end());
            Assert::AreEqual(static_cast<int>(nextMap["16"].size()), 2);
            Assert::IsTrue(find(nextMap["16"].begin(), nextMap["16"].end(), "17") != nextMap["16"].end());
            Assert::IsTrue(find(nextMap["16"].begin(), nextMap["16"].end(), "22") != nextMap["16"].end());
            Assert::AreEqual(static_cast<int>(nextMap["17"].size()), 1);
            Assert::IsTrue(find(nextMap["17"].begin(), nextMap["17"].end(), "18") != nextMap["17"].end());
            Assert::AreEqual(static_cast<int>(nextMap["18"].size()), 1);
            Assert::IsTrue(find(nextMap["18"].begin(), nextMap["18"].end(), "20") != nextMap["18"].end());
            Assert::AreEqual(static_cast<int>(nextMap["19"].size()), 1);
            Assert::IsTrue(find(nextMap["19"].begin(), nextMap["19"].end(), "20") != nextMap["19"].end());
            Assert::AreEqual(static_cast<int>(nextMap["20"].size()), 1);
            Assert::IsTrue(find(nextMap["20"].begin(), nextMap["20"].end(), "21") != nextMap["20"].end());
            Assert::AreEqual(static_cast<int>(nextMap["21"].size()), 2);
            Assert::IsTrue(find(nextMap["21"].begin(), nextMap["21"].end(), "16") != nextMap["21"].end());
            Assert::IsTrue(find(nextMap["21"].begin(), nextMap["21"].end(), "22") != nextMap["21"].end());
            Assert::AreEqual(static_cast<int>(nextMap["22"].size()), 1);
            Assert::IsTrue(find(nextMap["22"].begin(), nextMap["22"].end(), "23") != nextMap["22"].end());
            Assert::AreEqual(static_cast<int>(nextMap["23"].size()), 1);
            Assert::IsTrue(find(nextMap["23"].begin(), nextMap["23"].end(), "13") != nextMap["23"].end());
            Assert::AreEqual(static_cast<int>(nextMap["24"].size()), 1);
            Assert::IsTrue(find(nextMap["24"].begin(), nextMap["24"].end(), "25") != nextMap["24"].end());
            Assert::AreEqual(static_cast<int>(nextMap["25"].size()), 1);
            Assert::IsTrue(find(nextMap["25"].begin(), nextMap["25"].end(), "26") != nextMap["25"].end());
            Assert::AreEqual(static_cast<int>(nextMap["26"].size()), 1);
            Assert::IsTrue(find(nextMap["26"].begin(), nextMap["26"].end(), "37") != nextMap["26"].end());
            Assert::AreEqual(static_cast<int>(nextMap["27"].size()), 2);
            Assert::IsTrue(find(nextMap["27"].begin(), nextMap["27"].end(), "28") != nextMap["27"].end());
            Assert::IsTrue(find(nextMap["27"].begin(), nextMap["27"].end(), "37") != nextMap["27"].end());
            Assert::AreEqual(static_cast<int>(nextMap["28"].size()), 1);
            Assert::IsTrue(find(nextMap["28"].begin(), nextMap["28"].end(), "29") != nextMap["28"].end());
            Assert::AreEqual(static_cast<int>(nextMap["29"].size()), 1);
            Assert::IsTrue(find(nextMap["29"].begin(), nextMap["29"].end(), "30") != nextMap["29"].end());
            Assert::AreEqual(static_cast<int>(nextMap["30"].size()), 2);
            Assert::IsTrue(find(nextMap["30"].begin(), nextMap["30"].end(), "31") != nextMap["30"].end());
            Assert::IsTrue(find(nextMap["30"].begin(), nextMap["30"].end(), "33") != nextMap["30"].end());
            Assert::AreEqual(static_cast<int>(nextMap["31"].size()), 1);
            Assert::IsTrue(find(nextMap["31"].begin(), nextMap["31"].end(), "32") != nextMap["31"].end());
            Assert::AreEqual(static_cast<int>(nextMap["32"].size()), 1);
            Assert::IsTrue(find(nextMap["32"].begin(), nextMap["32"].end(), "27") != nextMap["32"].end());
            Assert::AreEqual(static_cast<int>(nextMap["33"].size()), 1);
            Assert::IsTrue(find(nextMap["33"].begin(), nextMap["33"].end(), "34") != nextMap["33"].end());
            Assert::AreEqual(static_cast<int>(nextMap["34"].size()), 1);
            Assert::IsTrue(find(nextMap["34"].begin(), nextMap["34"].end(), "35") != nextMap["34"].end());
            Assert::AreEqual(static_cast<int>(nextMap["35"].size()), 1);
            Assert::IsTrue(find(nextMap["35"].begin(), nextMap["35"].end(), "36") != nextMap["35"].end());
            Assert::AreEqual(static_cast<int>(nextMap["36"].size()), 1);
            Assert::IsTrue(find(nextMap["36"].begin(), nextMap["36"].end(), "27") != nextMap["36"].end());
            Assert::AreEqual(static_cast<int>(nextMap["37"].size()), 1);
            Assert::IsTrue(find(nextMap["37"].begin(), nextMap["37"].end(), "38") != nextMap["37"].end());
            Assert::AreEqual(static_cast<int>(nextMap["38"].size()), 1);
            Assert::IsTrue(find(nextMap["38"].begin(), nextMap["38"].end(), "39") != nextMap["38"].end());
            Assert::AreEqual(static_cast<int>(nextMap["39"].size()), 1);
            Assert::IsTrue(find(nextMap["39"].begin(), nextMap["39"].end(), "3") != nextMap["39"].end());
            Assert::AreEqual(static_cast<int>(nextMap["40"].size()), 2);
            Assert::IsTrue(find(nextMap["40"].begin(), nextMap["40"].end(), "41") != nextMap["40"].end());
            Assert::IsTrue(find(nextMap["40"].begin(), nextMap["40"].end(), "44") != nextMap["40"].end());
            Assert::AreEqual(static_cast<int>(nextMap["41"].size()), 1);
            Assert::IsTrue(find(nextMap["41"].begin(), nextMap["41"].end(), "42") != nextMap["41"].end());
            Assert::AreEqual(static_cast<int>(nextMap["42"].size()), 1);
            Assert::IsTrue(find(nextMap["42"].begin(), nextMap["42"].end(), "43") != nextMap["42"].end());
            Assert::AreEqual(static_cast<int>(nextMap["43"].size()), 1);
            Assert::IsTrue(find(nextMap["43"].begin(), nextMap["43"].end(), "52") != nextMap["43"].end());
            Assert::AreEqual(static_cast<int>(nextMap["44"].size()), 1);
            Assert::IsTrue(find(nextMap["44"].begin(), nextMap["44"].end(), "45") != nextMap["44"].end());
            Assert::AreEqual(static_cast<int>(nextMap["45"].size()), 1);
            Assert::IsTrue(find(nextMap["45"].begin(), nextMap["45"].end(), "46") != nextMap["45"].end());
            Assert::AreEqual(static_cast<int>(nextMap["46"].size()), 2);
            Assert::IsTrue(find(nextMap["46"].begin(), nextMap["46"].end(), "47") != nextMap["46"].end());
            Assert::IsTrue(find(nextMap["46"].begin(), nextMap["46"].end(), "48") != nextMap["46"].end());
            Assert::AreEqual(static_cast<int>(nextMap["47"].size()), 1);
            Assert::IsTrue(find(nextMap["47"].begin(), nextMap["47"].end(), "50") != nextMap["47"].end());
            Assert::AreEqual(static_cast<int>(nextMap["48"].size()), 1);
            Assert::IsTrue(find(nextMap["48"].begin(), nextMap["48"].end(), "49") != nextMap["48"].end());
            Assert::AreEqual(static_cast<int>(nextMap["49"].size()), 1);
            Assert::IsTrue(find(nextMap["49"].begin(), nextMap["49"].end(), "50") != nextMap["49"].end());
            Assert::AreEqual(static_cast<int>(nextMap["50"].size()), 1);
            Assert::IsTrue(find(nextMap["50"].begin(), nextMap["50"].end(), "51") != nextMap["50"].end());
            Assert::AreEqual(static_cast<int>(nextMap["51"].size()), 1);
            Assert::IsTrue(find(nextMap["51"].begin(), nextMap["51"].end(), "52") != nextMap["51"].end());
            Assert::AreEqual(static_cast<int>(nextMap["52"].size()), 1);
            Assert::IsTrue(find(nextMap["52"].begin(), nextMap["52"].end(), "53") != nextMap["52"].end());

               
        }

        TEST_METHOD(TestNestedAbstractionExtractopm)
        {
            string sourceCode = sourceCodeDemo4;

            shared_ptr<ProcessedProgram> processedProgram = SimpleProcessor::processProgram(sourceCode);
            DesignExtractor designExtractor = DesignExtractor();
            designExtractor.extractAbstractions(processedProgram);

            // Get the abstraction map
            map<string, unordered_set<string>> modifiesMapRef = *(designExtractor.getModifiesMap());
            map<string, unordered_set<string>> usesMapRef = *(designExtractor.getUsesMap());
            map<string, unordered_set<string>> followsMapRef = *(designExtractor.getFollowsMap());
            map<string, unordered_set<string>> followsStarMapRef = *(designExtractor.getFollowsStarMap());
            map<string, unordered_set<string>> parentMapRef = *(designExtractor.getParentMap());
            map<string, unordered_set<string>> parentStarMapRef = *(designExtractor.getParentStarMap());
            map<string, unordered_set<string>> nextMapRef = *(designExtractor.getNextMap());

            // Check the existence of values in usesMap
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(1)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(2)) != usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(3)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(4)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(5)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(6)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(7)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(8)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), to_string(9)) == usesMapRef["x"].end());
            Assert::IsTrue(find(usesMapRef["x"].begin(), usesMapRef["x"].end(), "testprocedure") != usesMapRef["x"].end());

            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(1)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(2)) != usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(3)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(4)) != usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(5)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(6)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(7)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(8)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), to_string(9)) == usesMapRef["y"].end());
            Assert::IsTrue(find(usesMapRef["y"].begin(), usesMapRef["y"].end(), "testprocedure") != usesMapRef["y"].end());

            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(1)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(2)) != usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(3)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(4)) != usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(5)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(6)) != usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(7)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(8)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), to_string(9)) == usesMapRef["z"].end());
            Assert::IsTrue(find(usesMapRef["z"].begin(), usesMapRef["z"].end(), "testprocedure") != usesMapRef["z"].end());

            // Check the existence of values in modifiesMap
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(1)) != modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(2)) != modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(3)) != modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(4)) == modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(5)) == modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(6)) == modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(7)) == modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(8)) == modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), to_string(9)) != modifiesMapRef["x"].end());
            Assert::IsTrue(find(modifiesMapRef["x"].begin(), modifiesMapRef["x"].end(), "testprocedure") != modifiesMapRef["x"].end());

            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(1)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(2)) != modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(3)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(4)) != modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(5)) != modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(6)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(7)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(8)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), to_string(9)) == modifiesMapRef["y"].end());
            Assert::IsTrue(find(modifiesMapRef["y"].begin(), modifiesMapRef["y"].end(), "testprocedure") != modifiesMapRef["y"].end());

            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(1)) == modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(2)) != modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(3)) == modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(4)) != modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(5)) == modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(6)) != modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(7)) != modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(8)) != modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), to_string(9)) == modifiesMapRef["z"].end());
            Assert::IsTrue(find(modifiesMapRef["z"].begin(), modifiesMapRef["z"].end(), "testprocedure") != modifiesMapRef["z"].end());

            // Check the existence of values in parentsMap
            Assert::IsTrue(parentMapRef["1"].empty());
            Assert::IsTrue(parentStarMapRef["1"].empty());

            Assert::IsTrue(parentMapRef["2"].empty());
            Assert::IsTrue(parentStarMapRef["2"].empty());

            
            Assert::AreEqual((int)parentMapRef["3"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["3"].size(), 1);
            Assert::IsTrue(find(parentMapRef["3"].begin(), parentMapRef["3"].end(), string("2")) != parentMapRef["3"].end());
            Assert::IsTrue(find(parentStarMapRef["3"].begin(), parentStarMapRef["3"].end(), string("2")) != parentStarMapRef["3"].end());

            Assert::AreEqual((int)parentMapRef["4"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["4"].size(), 1);
            Assert::IsTrue(find(parentMapRef["4"].begin(), parentMapRef["4"].end(), string("2")) != parentMapRef["4"].end());
            Assert::IsTrue(find(parentStarMapRef["4"].begin(), parentStarMapRef["4"].end(), string("2")) != parentStarMapRef["4"].end());

            Assert::AreEqual((int)parentMapRef["5"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["5"].size(), 2);
            Assert::IsTrue(find(parentMapRef["5"].begin(), parentMapRef["5"].end(), string("4")) != parentMapRef["5"].end());
            Assert::IsTrue(find(parentStarMapRef["5"].begin(), parentStarMapRef["5"].end(), string("4")) != parentStarMapRef["5"].end());
            Assert::IsTrue(find(parentStarMapRef["5"].begin(), parentStarMapRef["5"].end(), string("2")) != parentStarMapRef["5"].end());

            Assert::AreEqual((int)parentMapRef["6"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["6"].size(), 2);
            Assert::IsTrue(find(parentMapRef["6"].begin(), parentMapRef["6"].end(), string("4")) != parentMapRef["6"].end());
            Assert::IsTrue(find(parentStarMapRef["6"].begin(), parentStarMapRef["6"].end(), string("4")) != parentStarMapRef["6"].end());
            Assert::IsTrue(find(parentStarMapRef["6"].begin(), parentStarMapRef["6"].end(), string("2")) != parentStarMapRef["6"].end());

            Assert::AreEqual((int)parentMapRef["7"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["7"].size(), 3);
            Assert::IsTrue(find(parentMapRef["7"].begin(), parentMapRef["7"].end(), string("6")) != parentMapRef["7"].end());
            Assert::IsTrue(find(parentStarMapRef["7"].begin(), parentStarMapRef["7"].end(), string("6")) != parentStarMapRef["7"].end());
            Assert::IsTrue(find(parentStarMapRef["7"].begin(), parentStarMapRef["7"].end(), string("4")) != parentStarMapRef["7"].end());
            Assert::IsTrue(find(parentStarMapRef["7"].begin(), parentStarMapRef["7"].end(), string("2")) != parentStarMapRef["7"].end());

            Assert::AreEqual((int)parentMapRef["8"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["8"].size(), 3);
            Assert::IsTrue(find(parentMapRef["8"].begin(), parentMapRef["8"].end(), string("6")) != parentMapRef["8"].end());
            Assert::IsTrue(find(parentStarMapRef["8"].begin(), parentStarMapRef["8"].end(), string("6")) != parentStarMapRef["8"].end());
            Assert::IsTrue(find(parentStarMapRef["8"].begin(), parentStarMapRef["8"].end(), string("4")) != parentStarMapRef["8"].end());
            Assert::IsTrue(find(parentStarMapRef["8"].begin(), parentStarMapRef["8"].end(), string("2")) != parentStarMapRef["8"].end());

            Assert::AreEqual((int)parentMapRef["9"].size(), 1);
            Assert::AreEqual((int)parentStarMapRef["9"].size(), 1);
            Assert::IsTrue(find(parentMapRef["9"].begin(), parentMapRef["9"].end(), string("2")) != parentMapRef["9"].end());
            Assert::IsTrue(find(parentStarMapRef["9"].begin(), parentStarMapRef["9"].end(), string("2")) != parentStarMapRef["9"].end());

            // Check the existence of values in followsMap
            Assert::AreEqual((int)followsMapRef["1"].size(), 1);
            Assert::IsTrue(find(followsMapRef["1"].begin(), followsMapRef["1"].end(), string("2")) != followsMapRef["1"].end());
            Assert::IsTrue(followsMapRef["2"].empty());

            Assert::AreEqual((int)followsMapRef["3"].size(), 1);
            Assert::IsTrue(find(followsMapRef["3"].begin(), followsMapRef["3"].end(), string("4")) != followsMapRef["3"].end());
            
            Assert::IsTrue(followsMapRef["4"].empty());

            Assert::AreEqual((int)followsMapRef["5"].size(), 1);
            Assert::IsTrue(find(followsMapRef["5"].begin(), followsMapRef["5"].end(), string("6")) != followsMapRef["5"].end());

            Assert::IsTrue(followsMapRef["6"].empty());

            Assert::IsTrue(followsMapRef["7"].empty());

            Assert::IsTrue(followsMapRef["8"].empty());

            Assert::IsTrue(followsMapRef["9"].empty());

            // Check the existence of values in nextMap
            //x = 10
            Assert::IsTrue(find(nextMapRef["1"].begin(), nextMapRef["1"].end(), "2") != nextMapRef["1"].end());

            // If else statement (x == 10)
            Assert::IsTrue(find(nextMapRef["2"].begin(), nextMapRef["2"].end(), "3") != nextMapRef["2"].end());
            Assert::IsTrue(find(nextMapRef["2"].begin(), nextMapRef["2"].end(), "9") != nextMapRef["2"].end());

            // If statement (x = 20)
            Assert::IsTrue(find(nextMapRef["3"].begin(), nextMapRef["3"].end(), "4") != nextMapRef["3"].end());
            
            // While statement (y == 10)
            Assert::IsTrue(find(nextMapRef["4"].begin(), nextMapRef["4"].end(), "5") != nextMapRef["4"].end());

            // y = 20
            Assert::IsTrue(find(nextMapRef["5"].begin(), nextMapRef["5"].end(), "6") != nextMapRef["5"].end());

            // If else statement (z == 20)
            Assert::IsTrue(find(nextMapRef["6"].begin(), nextMapRef["6"].end(), "7") != nextMapRef["6"].end());
            Assert::IsTrue(find(nextMapRef["6"].begin(), nextMapRef["6"].end(), "8") != nextMapRef["6"].end());

            // z = 30
            Assert::IsTrue(find(nextMapRef["7"].begin(), nextMapRef["7"].end(), "4") != nextMapRef["7"].end());
            // z = 40
            Assert::IsTrue(find(nextMapRef["8"].begin(), nextMapRef["8"].end(), "4") != nextMapRef["8"].end());
        }

        TEST_METHOD(TestStressMultiThreading)
        {
            ApplicationWrapper applicationWrapper;
            applicationWrapper.parse("../../Tests18/Stress_source.txt", true);
        }

        TEST_METHOD(TestStressControl)
        {
            ApplicationWrapper applicationWrapper;
            applicationWrapper.parse("../../Tests18/Stress_source.txt", false);
        }
   };
     
}
