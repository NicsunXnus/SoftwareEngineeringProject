#include "stdafx.h"
#include "CppUnitTest.h"
#include "CppUnitTestAssert.h"
#include "QPSTestingSpecializations.h"
#include "../source/QPS/QueryParser.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQPS)
	{
	public:
		TEST_METHOD(TestTokenizer) {
			std::vector<std::shared_ptr<Token>> test = tokenize("stmt v; Select v");
			std::vector<std::shared_ptr<Token>> expectedResult;
			expectedResult.push_back(std::make_shared<StmtKeywordToken>());
			expectedResult.push_back(std::make_shared<IdentifierToken>("v"));
			expectedResult.push_back(std::make_shared<SemicolonSepToken>());
			expectedResult.push_back(std::make_shared<SelectKeywordToken>());
			expectedResult.push_back(std::make_shared <IdentifierToken>("v"));

			for (int i = 0; i < test.size(); ++i) {
				Token testToken = *test[i];
				Token expectedToken = *expectedResult[i];
				Assert::IsTrue(*expectedResult[i] == *test[i]);
			}
		}

		TEST_METHOD(TestSplitter)
		{
			std::vector<std::shared_ptr<Token>> test = tokenize("stmt v; Select v");
			std::shared_ptr<QueryParser> p = std::make_shared<QueryParser>();
			std::tuple<std::vector<std::shared_ptr<Token>>, std::vector<std::shared_ptr<Token>>> testObj = p->splitTokens(test);
			int test1 = 1;
		}

		TEST_METHOD(TestSelect)
		{
			std::vector<std::shared_ptr<Token>> tokenizer = tokenize("variable v; Select v");
			for (int i = 0; i < tokenizer.size(); i++) {
				std::shared_ptr<Token> curr = tokenizer[i];
				Token token = *curr;

			}

		}


	};
}