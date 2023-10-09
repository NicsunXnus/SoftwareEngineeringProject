#include "stdafx.h"
#include "CppUnitTest.h"
#include <string>
#include "../source/DesignExtractor/DesignExtractor.h"
#include "../source/SimpleTokenizer/SIMPLETokenizer.h"
#include "../source/AST/ASTBuilder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
      TEST_CLASS(TestDesignExtractor){
            public :
                TEST_METHOD(TestWithExtraction){
                    std::string sourceCode =
                        "procedure p {"
                        "if (x == 3) then {"
                        "read num1;"
                        "print num3;"
                        "sum = num1 + num2 + num3;}"
                        "else{read num2;}"
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
                    std::shared_ptr<TokenizedProgram> tokenizedProgram = SimpleTokenizer::tokenizeProgram(sourceCode);
                    std::shared_ptr<ProgramNode> prg = ASTBuilder::parseProgram(tokenizedProgram);
                    DesignExtractor designExtractor = DesignExtractor();
                    designExtractor.extractAllWith(prg);
                    std::shared_ptr<map<string, unordered_set<string>>> callProcNameMap = designExtractor.getWithExtractor()->getCallProcNameMap();
                    std::shared_ptr<map<string, unordered_set<string>>> readVarNameMap = designExtractor.getWithExtractor()->getReadVarNameMap();
                    std::shared_ptr<map<string, unordered_set<string>>> printVarNameMap = designExtractor.getWithExtractor()->getPrintVarNameMap();

                    Assert::AreEqual(2, (int)callProcNameMap->size());
                    Assert::AreEqual(5, (int)readVarNameMap->size());
                    Assert::AreEqual(4, (int)printVarNameMap->size());
            }

            TEST_METHOD(TestPatternExtraction){
                    std::string sourceCode =
                        "procedure p {"
                        "if (x == 3) then {"
                        "read num1;"
                        "print num3;"
                        "sum = num1 + num2 + num3;}"
                        "else{read num2;}"
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
                    std::shared_ptr<TokenizedProgram> tokenizedProgram = SimpleTokenizer::tokenizeProgram(sourceCode);
                    std::shared_ptr<ProgramNode> prg = ASTBuilder::parseProgram(tokenizedProgram);
                    DesignExtractor designExtractor = DesignExtractor();
                    designExtractor.extractPattern(prg);
                    
                    std::shared_ptr<map<string, std::shared_ptr<ASTNode>>> patternMap = designExtractor.getPatternExtractor()->getPatternMap();

                    Assert::AreEqual(4, (int)patternMap->size());

                    // Check if the nodes are either if, while or assign
                    for (auto const& pattern : *patternMap) {
                        std::shared_ptr<ASTNode> node = pattern.second;
                        string nodeName = node->getName();
                        Assert::IsTrue(nodeName == "assign" || nodeName == "while" || nodeName == "if");
                    }
            }
            
            TEST_METHOD(TestCallsUsesModifiesAbstractionExtraction){
                    std::string sourceCode =
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
                    std::shared_ptr<TokenizedProgram> tokenizedProgram = SimpleTokenizer::tokenizeProgram(sourceCode);
                    std::shared_ptr<ProgramNode> prg = ASTBuilder::parseProgram(tokenizedProgram);
                    DesignExtractor designExtractor = DesignExtractor();
                    designExtractor.extractAllAbstractions(prg);

                    std::shared_ptr<map<string, vector<string>>> callsAbstractionMap = designExtractor.getCallsAbstractionMap();
                    std::shared_ptr<map<string, vector<string>>> usesAbstractionMap = designExtractor.getUsesAbstractionMap();
                    std::shared_ptr<map<string, vector<string>>> modifiesAbstractionMap = designExtractor.getModifiesAbstractionMap();

                    // Check the map for the key "procedure1" and that it has procedures 2,3,4,5 individually
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure1").begin(), callsAbstractionMap->at("procedure1").end(), "procedure2") != callsAbstractionMap->at("procedure1").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure1").begin(), callsAbstractionMap->at("procedure1").end(), "procedure3") != callsAbstractionMap->at("procedure1").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure1").begin(), callsAbstractionMap->at("procedure1").end(), "procedure4") != callsAbstractionMap->at("procedure1").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure1").begin(), callsAbstractionMap->at("procedure1").end(), "procedure5") != callsAbstractionMap->at("procedure1").end());

                    // Check the map for the key "procedure3" and that it has procedures 2, 4 and 5 individually
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure3").begin(), callsAbstractionMap->at("procedure3").end(), "procedure2") != callsAbstractionMap->at("procedure3").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure3").begin(), callsAbstractionMap->at("procedure3").end(), "procedure4") != callsAbstractionMap->at("procedure3").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure3").begin(), callsAbstractionMap->at("procedure3").end(), "procedure5") != callsAbstractionMap->at("procedure3").end());

                    // Check the map for the key "procedure4" and that it has procedure2 
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure4").begin(), callsAbstractionMap->at("procedure4").end(), "procedure2") != callsAbstractionMap->at("procedure4").end());

                    // Check the map for the key "procedure5" and that it has procedure2 and procedure4 individually
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure5").begin(), callsAbstractionMap->at("procedure5").end(), "procedure2") != callsAbstractionMap->at("procedure5").end());
                    Assert::IsTrue(std::find(callsAbstractionMap->at("procedure5").begin(), callsAbstractionMap->at("procedure5").end(), "procedure4") != callsAbstractionMap->at("procedure5").end());

                    // Check useAbstractionMap for the key "x" and that it has values "4", "5", "6", "7", "8", "12", "21", "22", "24", "26" "29", "32", "33", "procedure1", "procedure3", "procedure4" "procedure5"
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "4") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "5") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "6") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "7") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "8") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "12") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "21") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "22") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "24") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "26") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "29") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "32") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "33") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "procedure1") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "procedure3") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "procedure4") != usesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(usesAbstractionMap->at("x").begin(), usesAbstractionMap->at("x").end(), "procedure5") != usesAbstractionMap->at("x").end());
                    Assert::AreEqual(17, (int)usesAbstractionMap->at("x").size());

                    // Check modifiesAbstractionMap for the key "x" and that it has values "2", "5", "6", "8", "12", "21", "22", "23", "29", "30", "32", "33", "procedure1", "procedure3", "procedure5"
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "2") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "5") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "6") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "8") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "12") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "21") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "22") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "23") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "29") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "30") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "32") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "33") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "procedure1") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "procedure3") != modifiesAbstractionMap->at("x").end());
                    Assert::IsTrue(std::find(modifiesAbstractionMap->at("x").begin(), modifiesAbstractionMap->at("x").end(), "procedure5") != modifiesAbstractionMap->at("x").end());
                    Assert::AreEqual(15, (int)modifiesAbstractionMap->at("x").size());
            }

            
    };
      
}