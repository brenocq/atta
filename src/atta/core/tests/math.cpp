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
		//---------- vec2 ----------//
		{
			vec2 v0(0.0f, 1.0f);
			EXPECT_EQ(v0.x, 0.0f);
			EXPECT_EQ(v0.y, 1.0f);

			vec2 v1(1.0f);
			EXPECT_EQ(v1.x, 1.0f);
			EXPECT_EQ(v1.y, 1.0f);

			vec2 v2;
			EXPECT_EQ(v2.x, 0.0f);
			EXPECT_EQ(v2.y, 0.0f);

			vec2 v3 = { 1.0f, 2.0f };
			EXPECT_EQ(v3.x, 1.0f);
			EXPECT_EQ(v3.y, 2.0f);
		}

		//---------- vec3 ----------//
		{
			vec3 v0(0.0f, 1.0f, 2.0f);
			EXPECT_EQ(v0.x, 0.0f);
			EXPECT_EQ(v0.y, 1.0f);
			EXPECT_EQ(v0.z, 2.0f);

			vec3 v1(vec2(0.0f, 1.0f), 2.0f);
			EXPECT_EQ(v1.x, 0.0f);
			EXPECT_EQ(v1.y, 1.0f);
			EXPECT_EQ(v1.z, 2.0f);

			vec3 v2(0.0f, vec2(1.0f, 2.0f));
			EXPECT_EQ(v2.x, 0.0f);
			EXPECT_EQ(v2.y, 1.0f);
			EXPECT_EQ(v2.z, 2.0f);

			vec3 v3(1.0f);
			EXPECT_EQ(v3.x, 1.0f);
			EXPECT_EQ(v3.y, 1.0f);
			EXPECT_EQ(v3.z, 1.0f);

			vec3 v4;
			EXPECT_EQ(v4.x, 0.0f);
			EXPECT_EQ(v4.y, 0.0f);
			EXPECT_EQ(v4.z, 0.0f);
			
			vec3 v5 = { 0.0f, 1.0f, 2.0f };
			EXPECT_EQ(v5.x, 0.0f);
			EXPECT_EQ(v5.y, 1.0f);
			EXPECT_EQ(v5.z, 2.0f);
		}

		//---------- vec4 ----------//
		{
			vec4 v0(0.0f, 1.0f, 2.0f, 3.0f);
			EXPECT_EQ(v0.x, 0.0f);
			EXPECT_EQ(v0.y, 1.0f);
			EXPECT_EQ(v0.z, 2.0f);
			EXPECT_EQ(v0.w, 3.0f);

			vec4 v1(1.0f);
			EXPECT_EQ(v1.x, 1.0f);
			EXPECT_EQ(v1.y, 1.0f);
			EXPECT_EQ(v1.z, 1.0f);
			EXPECT_EQ(v1.w, 1.0f);

			vec4 v2;
			EXPECT_EQ(v2.x, 0.0f);
			EXPECT_EQ(v2.y, 0.0f);
			EXPECT_EQ(v2.z, 0.0f);
			EXPECT_EQ(v2.w, 0.0f);

			vec4 v3(vec2(0.0f, 1.0f), vec2(2.0f, 3.0f));
			EXPECT_EQ(v3.x, 0.0f);
			EXPECT_EQ(v3.y, 1.0f);
			EXPECT_EQ(v3.z, 2.0f);
			EXPECT_EQ(v3.w, 3.0f);

			vec4 v4(vec3(0.0f, 1.0f, 2.0f), 3.0f);
			EXPECT_EQ(v4.x, 0.0f);
			EXPECT_EQ(v4.y, 1.0f);
			EXPECT_EQ(v4.z, 2.0f);
			EXPECT_EQ(v4.w, 3.0f);

			vec4 v5(0.0f, vec3(1.0f, 2.0f, 3.0f));
			EXPECT_EQ(v5.x, 0.0f);
			EXPECT_EQ(v5.y, 1.0f);
			EXPECT_EQ(v5.z, 2.0f);
			EXPECT_EQ(v5.w, 3.0f);

			vec4 v6(0.0f, 1.0f, vec2(2.0f, 3.0f));
			EXPECT_EQ(v6.x, 0.0f);
			EXPECT_EQ(v6.y, 1.0f);
			EXPECT_EQ(v6.z, 2.0f);
			EXPECT_EQ(v6.w, 3.0f);

			vec4 v7(0.0f, vec2(1.0f, 2.0f), 3.0f);
			EXPECT_EQ(v7.x, 0.0f);
			EXPECT_EQ(v7.y, 1.0f);
			EXPECT_EQ(v7.z, 2.0f);
			EXPECT_EQ(v7.w, 3.0f);

			vec4 v8(vec2(0.0f, 1.0f), 2.0f, 3.0f);
			EXPECT_EQ(v8.x, 0.0f);
			EXPECT_EQ(v8.y, 1.0f);
			EXPECT_EQ(v8.z, 2.0f);
			EXPECT_EQ(v8.w, 3.0f);

			vec4 v9 = { 0.0f, 1.0f, 2.0f, 3.0f };
			EXPECT_EQ(v9.x, 0.0f);
			EXPECT_EQ(v9.y, 1.0f);
			EXPECT_EQ(v9.z, 2.0f);
			EXPECT_EQ(v9.w, 3.0f);
		}
	}
}
