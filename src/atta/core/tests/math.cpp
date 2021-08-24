//--------------------------------------------------
// Atta Core Tests
// stringId.cpp
// Date: 2021-08-18
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <gtest/gtest.h>
#include <atta/core/math/math.h>

using namespace atta;
namespace
{
	TEST(Core_Math_Vector, Create)
	{
		atta::vec4 v4(0.0f, 1.0f, 2.0f, 3.0f);
		EXPECT_EQ(v4.x, 0.0f);
		EXPECT_EQ(v4.y, 1.0f);
		EXPECT_EQ(v4.z, 2.0f);
		EXPECT_EQ(v4.w, 3.0f);
	}
}
