#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/TokenizerClasses/SimpleTokenizer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestSP)
	{
	public:

		TEST_METHOD(TestSimpleTokenizer)
		{
			std::vector < std::vector<std::shared_ptr<Token>>> test = SimpleTokenizer::tokenize(
				"read num1;"
				"read num2;"
				"read num3;"

				"sum = num1 + num2 + num3;"
				"ave = sum / 3;"

				"print ave;"
			);
			for (int i = 0; i < test.size(); i++) {
				std::vector<std::shared_ptr<Token>> curr = test[i];
				for (int j = 0; j < curr.size(); j++) {
					Token token = *curr[j];
				}
			}
			
		}

	};
}