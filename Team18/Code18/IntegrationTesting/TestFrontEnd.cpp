#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/ApplicationWrapper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(DESCRIBE_THIS_TEST)
		{
			std::string srcCode = "";
			ApplicationWrapper applicationWrapper;
			applicationWrapper.directParse(srcCode);
			std::string query = "";
			std::list<std::string> output;
			applicationWrapper.evaluate(query, output);
			std::list<std::string> expected;
			assert(output == expected);
		}

	};
}