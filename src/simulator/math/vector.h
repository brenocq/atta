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
	//---------- Vector 3 ----------//
	template <typename T>
	class vector3
	{
		public:
			T x, y, z;
		private:
			T pad;// 4 word alignment

		public:
        	vector3(): x(0), y(0), z(0) {};
        	vector3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {};
			vector3(glm::vec3 vector): x(vector.x), y(vector.y), z(vector.z) {};

			T operator[](unsigned i) const
			{
				if(i == 0) return x;
				if(i == 1) return y;
				return z;
			}

			// Add vector
			template <typename U>
			void operator+=(const vector3<U>& v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
			}

			template <typename U>
			vector3<T> operator+(const vector3<U>& v) const
			{
				return vector3<T>(x+v.x, y+v.y, z+v.z);
			}

			// Subtract vector
			template <typename U>
			T operator-=(const vector3<U>& v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
			}

			template <typename U>
			vector3<T> operator-(const vector3<U>& v) const
			{
				return vector3<T>(x-v.x, y-v.y, z-v.z);
			}

			// Multiply scalar
			template <typename U>
			void operator*=(const U value)
			{
				x *= value;
				y *= value;
				z *= value;
			}

			template <typename U>
			vector3<T> operator*(const U value) const
			{
				return vector3<T>(x*value, y*value, z*value);
			}

			// Divide scalar
			template <typename U>
			void operator/=(const U value)
			{
				x /= value;
				y /= value;
				z /= value;
			}

			template <typename U>
			vector3<T> operator/(const U value) const
			{
				return vector3<T>(x/value, y/value, z/value);
			}

			// Multiply vector
			template <typename U>
			void operator*=(const vector3<U> vector)
			{
				x *= vector.x;
				y *= vector.y;
				z *= vector.z;
			}

			template <typename U>
			vector3<T> operator*(const vector3<U> vector) const
			{
				return vector3<T>(x*vector.x, y*vector.y, z*vector.z);
			}

			// Vector products
			template <typename U>
			vector3<T> cross(const vector3<U> &vector) const
			{
				// Using always double to avoid float-pointing error
				double xd = x, yd = y, zd = z;
				double vxd = vector.x, vyd = vector.y, vzd = vector.z;
				return vector3<T>((yd*vzd) - (zd*vyd),
								  (zd*vxd) - (xd*vzd),
								  (xd*vyd) - (yd*vxd));
			}

			template <typename U>
			float dot(const vector3<U> &vector) const
			{
				return x*vector.x + y*vector.y + z*vector.z;
			}

			// Length
			float length() const
			{
				return std::sqrt(squareLength());
			}

			float squareLength() const
			{
				return x*x+y*y+z*z;
			}
			
			// Normalize
			float normalize()
			{
				float l = length();
				if(l > 0)
				{
					(*this) *= 1.0f/l;
				}
			}

			// Unit
			vector3<T> unit() const
			{
				vector3<T> result = *this;
				result.normalize();
				return result;
			}

			// Logical compare
			template <typename U>
			bool operator==(const vector3<U>& other) const
			{
				return x == other.x &&
					y == other.y &&
					z == other.z;
			}

			template <typename U>
			bool operator!=(const vector3<U>& other) const
			{
				return !(*this == other);
			}

			template <typename U>
			bool operator<(const vector3<U>& other) const
			{
				return x < other.x && y < other.y && z < other.z;
			}

			template <typename U>
			bool operator>(const vector3<U>& other) const
			{
				return x > other.x && y > other.y && z > other.z;
			}

			template <typename U>
			bool operator<=(const vector3<U>& other) const
			{
				return x <= other.x && y <= other.y && z <= other.z;
			}

			template <typename U>
			bool operator>=(const vector3<U>& other) const
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

			T maxComponent()
			{
				return std::max(x, std::max(y, z));
			}

			T minComponent()
			{
				return std::min(x, std::min(y, z));
			}

			// Glm conversion
			operator glm::vec3() const { return glm::vec3(x,y,z); }
	};

	template <typename T>
	vector3<T> operator*(const T value, vector3<T> const &vec)
	{
		return vec*value;
	}


	//---------- Vector 2 ----------//
	template <typename T>
	class vector2
	{
		public:
			T x, y;

        	vector2(): x(0), y(0) {};
        	vector2(T _x, T _y): x(_x), y(_y) {};
			vector2(glm::vec2 vector): x(vector.x), y(vector.y) {};

			T operator[](unsigned i) const
			{
				if(i == 0) return x;
				return y;
			}

			// Add vector
			template <typename U>
			void operator+=(const vector2<U>& v)
			{
				x += v.x;
				y += v.y;
			}

			template <typename U>
			vector2<T> operator+(const vector2<U>& v) const
			{
				return vector2<T>(x+v.x, y+v.y);
			}

			// Subtract vector
			template <typename U>
			T operator-=(const vector2<U>& v)
			{
				x -= v.x;
				y -= v.y;
			}

			template <typename U>
			vector2<T> operator-(const vector2<U>& v) const
			{
				return vector2<T>(x-v.x, y-v.y);
			}

			// Multiply scalar
			template <typename U>
			void operator*=(const U value)
			{
				x *= value;
				y *= value;
			}

			template <typename U>
			vector2<T> operator*(const U value) const
			{
				return vector2<T>(x*value, y*value);
			}

			// Divide scalar
			template <typename U>
			void operator/=(const U value)
			{
				x /= value;
				y /= value;
			}

			template <typename U>
			vector2<T> operator/(const U value) const
			{
				return vector2<T>(x/value, y/value);
			}

			// Multiply vector
			template <typename U>
			void operator*=(const vector2<U> vector)
			{
				x *= vector.x;
				y *= vector.y;
			}

			template <typename U>
			vector2<T> operator*(const vector2<U> vector) const
			{
				return vector2<T>(x*vector.x, y*vector.y);
			}

			template <typename U>
			float dot(const vector2<U> &vector) const
			{
				return x*vector.x + y*vector.y;
			}

			// Length
			float length() const
			{
				return std::sqrt(squareLength());
			}

			float squareLength() const
			{
				return x*x+y*y;
			}
			
			// Normalize
			float normalize()
			{
				float l = length();
				if(l > 0)
				{
					(*this) *= 1.0f/l;
				}
			}

			// Unit
			vector2<T> unit() const
			{
				vector2<T> result = *this;
				result.normalize();
				return result;
			}

			// Logical compare
			template <typename U>
			bool operator==(const vector2<U>& other) const
			{
				return x == other.x &&
					y == other.y;
			}

			template <typename U>
			bool operator!=(const vector2<U>& other) const
			{
				return !(*this == other);
			}

			template <typename U>
			bool operator<(const vector2<U>& other) const
			{
				return x < other.x && y < other.y;
			}

			template <typename U>
			bool operator>(const vector2<U>& other) const
			{
				return x > other.x && y > other.y;
			}

			template <typename U>
			bool operator<=(const vector2<U>& other) const
			{
				return x <= other.x && y <= other.yz;
			}

			template <typename U>
			bool operator>=(const vector2<U>& other) const
			{
				return x >= other.x && y >= other.yz;
			}

			// Helpers
			void clear()
			{
				x = y = 0;
			}

			void invert()
			{
				x = -x;
				y = -y;
			}

			T maxComponent()
			{
				return std::max(x, y);
			}

			T minComponent()
			{
				return std::min(x, y);
			}

			// Glm conversion
			operator glm::vec2() const { return glm::vec2(x,y); }
	};

	typedef vector3<float> vec3;
	typedef vector3<float> vec3f;
	typedef vector3<double> vec3d;
	typedef vector3<int> vec3i;

	typedef vector2<float> vec2;
	typedef vector2<float> vec2f;
	typedef vector2<double> vec2d;
	typedef vector2<int> vec2i;

}

#endif// ATTA_VECTOR_H
