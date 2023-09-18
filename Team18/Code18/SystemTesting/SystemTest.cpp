#include "stdafx.h"
#include "CppUnitTest.h"

#include "../source/ApplicationWrapper.h"
#include "SystemTestWrapper.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SystemTesting
{		
	TEST_CLASS(UnitTest1) {
	private:
		static inline const std::string defaultPath = "../../Tests18/";
	public:
		
		TEST_METHOD(noExtraClauses_success)
		{
			std::string fileName = defaultPath + "";
			std::string srcFilePath = fileName + "source";
			std::string queryFilePath = fileName + "query";
			assert(SystemTestWrapper::run(srcFilePath, queryFilePath));
		}

	};
}