#include "CppUnitTest.h"
#include "../source/TokenizerClasses/PQLTokenizer.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> static inline std::wstring ToString<Token>(const class Token& t) {
				std::wstring result = std::wstring(L"Token name: ");

				//result.append(t.getTokenName());

				return result;
			}
		}
	}
}