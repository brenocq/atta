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

	//#include <immintrin.h>
	//TEST(Core_Math_Vector, SIMD128)
	//{
	//	class Vec128
	//	{
	//		public:
	//		union {
	//			struct {
	//				float x;
	//				float y;
	//				float z;
	//				float w;
	//			};
	//			__m128 v;
	//		};

	//		Vec128(float x_, float y_, float z_, float w_):
	//			x(x_), y(y_), z(z_), w(w_)
	//		{

	//		}

	//		Vec128() = default;

	//		Vec128 operator+(const Vec128& o)
	//		{
	//			Vec128 res;
	//			//res.x = x + o.x;
	//			//res.y = y + o.y;
	//			//res.z = z + o.z;
	//			//res.w = w + o.w;
	//			res.v = _mm_add_ps(v, o.v);
	//			return res;
	//		}
	//	};

	//	Vec128 a(0,1,2,3);
	//	Vec128 b(3,2,1,0);
	//	Vec128 res;
	//	for(int i = 0; i < 10000*1000; i++)
	//		res = a+b;

	//	EXPECT_EQ(res.x, 3.0f);
	//	EXPECT_EQ(res.y, 3.0f);
	//	EXPECT_EQ(res.z, 3.0f);
	//	EXPECT_EQ(res.w, 3.0f);

	//	class Vec256
	//	{
	//		public:
	//			union {
	//				float d[8];
	//				__m256 v;
	//			};

	//			Vec256() = default;

	//			Vec256 operator+(const Vec256& o)
	//			{
	//				Vec256 res;
	//				//res.x = x + o.x;
	//				//res.y = y + o.y;
	//				//res.z = z + o.z;
	//				//res.w = w + o.w;
	//				// Faster 2x
	//				res.v = _mm256_add_ps(v, o.v);
	//				return res;
	//			}
	//	};
	//}

	//TEST(Core_Math_Vector, SIMD256)
	//{
	//	class Vec256
	//	{
	//		public:
	//			union {
	//				float d[8];
	//				__m256 v;
	//			};

	//			Vec256() = default;

	//			Vec256 operator+(const Vec256& o)
	//			{
	//				Vec256 res;
	//				//res.d[0] = d[0] + o.d[0];
	//				//res.d[1] = d[1] + o.d[1];
	//				//res.d[2] = d[2] + o.d[2];
	//				//res.d[3] = d[3] + o.d[3];
	//				//res.d[4] = d[4] + o.d[4];
	//				//res.d[5] = d[5] + o.d[5];
	//				//res.d[6] = d[6] + o.d[6];
	//				//res.d[7] = d[7] + o.d[7];

	//				// Faster x2
	//				res.v = _mm256_add_ps(v, o.v);
	//				return res;
	//			}
	//	};

	//	Vec256 a;
	//	a.d[0] = 0.0f;
	//	a.d[1] = 1.0f;
	//	a.d[2] = 2.0f;
	//	a.d[3] = 3.0f;
	//	a.d[4] = 4.0f;
	//	a.d[5] = 5.0f;
	//	a.d[6] = 6.0f;
	//	a.d[7] = 7.0f;

	//	Vec256 b;
	//	b.d[7] = 0.0f;
	//	b.d[6] = 1.0f;
	//	b.d[5] = 2.0f;
	//	b.d[4] = 3.0f;
	//	b.d[3] = 4.0f;
	//	b.d[2] = 5.0f;
	//	b.d[1] = 6.0f;
	//	b.d[0] = 7.0f;

	//	Vec256 res;
	//	for(int i = 0; i < 10000*1000; i++)
	//		res = a+b;

	//	EXPECT_EQ(res.d[0], 7.0f);
	//	EXPECT_EQ(res.d[1], 7.0f);
	//	EXPECT_EQ(res.d[2], 7.0f);
	//	EXPECT_EQ(res.d[3], 7.0f);
	//	EXPECT_EQ(res.d[4], 7.0f);
	//	EXPECT_EQ(res.d[5], 7.0f);
	//	EXPECT_EQ(res.d[6], 7.0f);
	//	EXPECT_EQ(res.d[7], 7.0f);
	//}
}
