//--------------------------------------------------
// Atta Math
// vector.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VECTOR_H
#define ATTA_VECTOR_H
#include <math.h>
#include "glm.h"
//#include "quaternion.h"


namespace atta
{
	class vec3
	{
		public:
			float x;
			float y;
			float z;
		private:
			float pad;// 4 word alignment

		public:
        	vec3(): x(0), y(0), z(0) {};
        	vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {};
			vec3(glm::vec3 vec): x(vec.x), y(vec.y), z(vec.z) {};

			float operator[](unsigned i) const
			{
				if(i == 0) return x;
				if(i == 1) return y;
				return z;
			}

			// Add vector
			void operator+=(const vec3& v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
			}

			vec3 operator+(const vec3& v) const
			{
				return vec3(x+v.x, y+v.y, z+v.z);
			}

			// Subtract vector
			void operator-=(const vec3& v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
			}

			vec3 operator-(const vec3& v) const
			{
				return vec3(x-v.x, y-v.y, z-v.z);
			}

			// Multiply scalar
			void operator*=(const float value)
			{
				x *= value;
				y *= value;
				z *= value;
			}

			vec3 operator*(const float value) const
			{
				return vec3(x*value, y*value, z*value);
			}

			// Divide scalar
			void operator/=(const float value)
			{
				x /= value;
				y /= value;
				z /= value;
			}

			vec3 operator/(const float value) const
			{
				return vec3(x/value, y/value, z/value);
			}

			// Multiply vector
			void operator*=(const vec3 vector)
			{
				x *= vector.x;
				y *= vector.y;
				z *= vector.z;
			}

			vec3 operator*(const vec3 vector) const
			{
				return vec3(x*vector.x, y*vector.y, z*vector.z);
			}

			// Vector products
			vec3 cross(const vec3 &vector) const
			{
				return vec3(y*vector.z-z*vector.y,
							   z*vector.x-x*vector.z,
							   x*vector.y-y*vector.x);
			}

			float dot(const vec3 &vector) const
			{
				return x*vector.x + y*vector.y + z*vector.x;
			}

			// Length
			float length() const
			{
				return std::sqrt(x*x+y*y+z*z);
			}

			float squareLength() const
			{
				return x*x+y*y+z*z;
			}
			
			// Normalize
			void normalize()
			{
				float l = length();
				if(l > 0)
				{
					(*this) *= 1.0f/l;
				}
			}

			// Unit
			vec3 unit() const
			{
				vec3 result = *this;
				result.normalize();
				return result;
			}

			// Logical compare
			bool operator==(const vec3& other) const
			{
				return x == other.x &&
					y == other.y &&
					z == other.z;
			}

			bool operator!=(const vec3& other) const
			{
				return !(*this == other);
			}

			bool operator<(const vec3& other) const
			{
				return x < other.x && y < other.y && z < other.z;
			}

			bool operator>(const vec3& other) const
			{
				return x > other.x && y > other.y && z > other.z;
			}

			bool operator<=(const vec3& other) const
			{
				return x <= other.x && y <= other.y && z <= other.z;
			}

			bool operator>=(const vec3& other) const
			{
				return x >= other.x && y >= other.y && z >= other.z;
			}

			// Helpers
			void clear()
			{
				x = y = z = 0;
			}

			void invert()
			{
				x = -x;
				y = -y;
				z = -z;
			}

			// Glm conversion
			operator glm::vec3() const { return glm::vec3(x,y,z); }
	};
}

#endif// ATTA_VECTOR_H
