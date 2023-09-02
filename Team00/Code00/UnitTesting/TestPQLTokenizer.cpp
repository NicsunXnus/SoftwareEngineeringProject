#include "stdafx.h"
#include "CppUnitTest.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQPS)
	{
	public:

		TEST_METHOD(TestBasic)
		{
			std::vector<std::shared_ptr<Token>> test = tokenize("variable v; Select v");
			for (int i = 0; i < test.size(); i++) {
				std::shared_ptr<Token> curr = test[i];
				Token token = *curr;

			}
			
		}

	};
}