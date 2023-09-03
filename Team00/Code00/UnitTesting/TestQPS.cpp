#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/QPS/QueryParser.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQPS)
	{
	public:

		TEST_METHOD(TestSplitter)
		{
			std::vector<std::shared_ptr<Token>> test = tokenize("stmt v; Select v");
			std::shared_ptr<QueryParser> p = std::make_shared<QueryParser>();
			//std::tuple<std::vector<std::shared_ptr<Token>>, std::vector<std::shared_ptr<Token>>> testObj = p->splitTokens(test);
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