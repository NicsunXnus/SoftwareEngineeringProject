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
                        designExtractor.extractWith(prg);
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

            
    };
      
}