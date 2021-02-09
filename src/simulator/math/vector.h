//--------------------------------------------------
// Atta Math
// vector.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_VECTOR_H
#define ATTA_VECTOR_H

#include <math.h>
#include <string>
//#include "glm.h"

namespace atta
{
	template <typename T>
	class vector4;
	template <typename T>
	class vector3;
	template <typename T>
	class vector2;

	//----------------------------------------//
	//--------------- Vector 4 ---------------//
	//----------------------------------------//
	template <typename T>
	class vector4
	{
		public:
			T x, y, z, w;

		public:
        	vector4(): x(0), y(0), z(0), w(0) {};
        	vector4(T _x, T _y, T _z, T _w): x(_x), y(_y), z(_z), w(_w) {};
			vector4(vector3<T> vector, T _w): x(vector.x), y(vector.y), z(vector.z), w(_w) {};
			vector4(T _x, vector3<T> vector): x(_x), y(vector.x), z(vector.y), w(vector.z) {};

			T operator[](unsigned i) const
			{
				if(i == 0) return x;
				if(i == 1) return y;
				if(i == 2) return z;
				return w;
			}

			// Add vector
			template <typename U>
			void operator+=(const vector4<U>& v)
			{
				x += v.x;
				y += v.y;
				z += v.z;
				w += v.w;
			}

			template <typename U>
			vector4<T> operator+(const vector4<U>& v) const
			{
				return vector4<T>(x+v.x, y+v.y, z+v.z, w+v.w);
			}

			// Subtract vector
			template <typename U>
			void operator-=(const vector4<U>& v)
			{
				x -= v.x;
				y -= v.y;
				z -= v.z;
				w -= v.w;
			}

			template <typename U>
			vector4<T> operator-(const vector4<U>& v) const
			{
				return vector4<T>(x-v.x, y-v.y, z-v.z, w-v.w);
			}

			vector4<T> operator-() const
			{
				return vector4<T>(-x, -y, -z, -w);
			}

			// Multiply scalar
			template <typename U>
			void operator*=(const U value)
			{
				x *= value;
				y *= value;
				z *= value;
				w *= value;
			}

			template <typename U>
			vector4<T> operator*(const U value) const
			{
				return vector4<T>(x*value, y*value, z*value, w*value);
			}

			// Divide scalar
			template <typename U>
			void operator/=(const U value)
			{
				x /= value;
				y /= value;
				z /= value;
				w /= value;
			}

			template <typename U>
			vector4<T> operator/(const U value) const
			{
				return vector4<T>(x/value, y/value, z/value, w/value);
			}

			// Multiply vector
			template <typename U>
			void operator*=(const vector4<U> vector)
			{
				x *= vector.x;
				y *= vector.y;
				z *= vector.z;
				w *= vector.w;
			}

			template <typename U>
			vector4<T> operator*(const vector4<U> vector) const
			{
				return vector4<T>(x*vector.x, y*vector.y, z*vector.z, w*vector.w);
			}

			template <typename U>
			float dot(const vector4<U> &vec) const
			{
				return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
			}

			// Length
			float length() const
			{
				return std::sqrt(squareLength());
			}

			float squareLength() const
			{
				return x*x+y*y+z*z+w*w;
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
			vector4<T> unit() const
			{
				vector4<T> result = *this;
				result.normalize();
				return result;
			}

			// Logical compare
			template <typename U>
			bool operator==(const vector4<U>& other) const
			{
				return x == other.x &&
					y == other.y &&
					z == other.z &&
					w == other.w;
			}

			template <typename U>
			bool operator!=(const vector4<U>& other) const
			{
				return !(*this == other);
			}

			template <typename U>
			bool operator<(const vector4<U>& other) const
			{
				return x < other.x && y < other.y && z < other.z && w < other.w;
			}

			template <typename U>
			bool operator>(const vector4<U>& other) const
			{
				return x > other.x && y > other.y && z > other.z && w > other.w;
			}

			template <typename U>
			bool operator<=(const vector4<U>& other) const
			{
				return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
			}

			template <typename U>
			bool operator>=(const vector4<U>& other) const
			{
				return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
			}

			// Helpers
			void clear()
			{
				x = y = z = w = 0;
			}

			void invert()
			{
				x = -x;
				y = -y;
				z = -z;
				w = -w;
			}

			T maxComponent()
			{
				return std::max(x, std::max(y, std::max(z, w)));
			}

			T minComponent()
			{
				return std::min(x, std::min(y, std::min(z, w)));
			}

			// Glm conversion
			//operator glm::vec4() const { return glm::vec4(x,y,z,w); }

			std::string toString() const
			{
				return "vec4{"+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+", "+std::to_string(w)+"}";
			}
	};

	template <typename T>
	vector4<T> operator*(const T value, vector4<T> const &vec)
	{
		return vec*value;
	}

	// Normalize
	template <typename T>
	inline vector4<T> normalize(const vector4<T> &v)
	{
		float l = v.length();
		if(l > 0)
		{
			return v * 1.0f/l;
		}
		return v;
	}

	//----------------------------------------//
	//--------------- Vector 3 ---------------//
	//----------------------------------------//
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
        	vector3(vector4<T> vector): x(vector.x), y(vector.y), z(vector.z) {};
			//vector3(glm::vec3 vector): x(vector.x), y(vector.y), z(vector.z) {};

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
			void operator-=(const vector3<U>& v)
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

			vector3<T> operator-() const
			{
				return vector3<T>(-x, -y, -z);
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
			void normalize()
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
			//operator glm::vec3() const { return glm::vec3(x,y,z); }

			std::string toString() const
			{
				return "vec3{"+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+"}";
			}
	};

	template <typename T>
	vector3<T> operator*(const T value, vector3<T> const &vec)
	{
		return vec*value;
	}


	// Cross
	template <typename T>
	inline vector3<T> cross(const vector3<T> &v1, const vector3<T> &v2)
	{
		// Using always double to avoid float-pointing error
		double xd = v1.x, yd = v1.y, zd = v1.z;
		double vxd = v2.x, vyd = v2.y, vzd = v2.z;
		return vector3<T>((yd*vzd) - (zd*vyd),
						  (zd*vxd) - (xd*vzd),
						  (xd*vyd) - (yd*vxd));
	}

	// Normalize
	template <typename T>
	inline vector3<T> normalize(const vector3<T> &v)
	{
		float l = v.length();
		if(l > 0)
		{
			return v * 1.0f/l;
		}
		return v;
	}

	// Dot
	template <typename T>
	float dot(const vector3<T> &v1, const vector3<T> &v2)
	{
		return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	}

	//----------------------------------------//
	//--------------- Vector 2 ---------------//
	//----------------------------------------//
	template <typename T>
	class vector2
	{
		public:
			T x, y;

        	vector2(): x(0), y(0) {};
        	vector2(T _x, T _y): x(_x), y(_y) {};
			//vector2(glm::vec2 vector): x(vector.x), y(vector.y) {};

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
			void operator-=(const vector2<U>& v)
			{
				x -= v.x;
				y -= v.y;
			}

			template <typename U>
			vector2<T> operator-(const vector2<U>& v) const
			{
				return vector2<T>(x-v.x, y-v.y);
			}

			vector2<T> operator-() const
			{
				return vector2<T>(-x, -y);
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
			void normalize()
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
			//operator glm::vec2() const { return glm::vec2(x,y); }

			std::string toString() const
			{
				return "vec2{"+std::to_string(x)+", "+std::to_string(y)+"}";
			}
	};

	typedef vector4<float> vec4;
	typedef vector4<float> vec4f;
	typedef vector4<double> vec4d;
	typedef vector4<int> vec4i;

	typedef vector3<float> vec3;
	typedef vector3<float> vec3f;
	typedef vector3<double> vec3d;
	typedef vector3<int> vec3i;

	typedef vector2<float> vec2;
	typedef vector2<float> vec2f;
	typedef vector2<double> vec2d;
	typedef vector2<int> vec2i;

}

namespace std {
	template<> struct hash<atta::vec3> {
		size_t operator()(atta::vec3 const& vec) const {
			return ((hash<float>()(vec.x) ^
				   (hash<float>()(vec.y) << 1)) >> 1) ^
				   (hash<float>()(vec.z) << 1);
		}
	};

	template<> struct hash<atta::vec2> {
		size_t operator()(atta::vec2 const& vec) const {
			return ((hash<float>()(vec.x) ^
				   (hash<float>()(vec.y) << 1)) >> 1);
		}
	};
}


#endif// ATTA_VECTOR_H
