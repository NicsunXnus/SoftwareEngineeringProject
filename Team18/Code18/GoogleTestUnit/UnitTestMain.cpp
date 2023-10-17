#include "pch.h"
#include "../source/HelperFunctions.h"
#include <vector>
#include <string>
//start of TestHelperFunctions

namespace SmallTest {
	TEST(isNumber_Test, numbers_success)
	{
		bool result = isNumber("01234");
		ASSERT_TRUE(result);
	}

	TEST(isNumber_Test, nonNumbers_failure)
	{
		std::vector<std::string> tests = { "0x1", "0b1", "0abc", "abc0", "-1", "--1", " ", "%", "|"};
		bool result = any_of(tests.begin(), tests.end(), isNumber);
		ASSERT_FALSE(result);
	}
};
namespace SmallTest2 {
	TEST(isAlphanumeric_Test, alphanum_success)
	{
		std::vector<std::string> tests = { "abc", "123", "x4y5z6", "abc0", "0x1", "0b1" };
		bool result = std::all_of(tests.begin(), tests.end(), isAlphanumeric);
		EXPECT_TRUE(result);
	}

	TEST(isAlphanumeric_Test, nonAlphanum_failure)
	{
		std::vector<std::string> tests = { " ", "|" };
		bool result = std::any_of(tests.begin(), tests.end(), isNumber);
		EXPECT_FALSE(result);
	}
};
int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}