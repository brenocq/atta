//--------------------------------------------------
// Atta Math
// point.h
// Date: 2020-12-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_POINT_H
#define ATTA_POINT_H
#include <math.h>
#include "vector.h"

namespace atta
{
	//---------- Point3 ----------//
	template <typename T>
	class point3
	{
		public:
			T x,y,z;
		private:
			T pad;// 4 word alignment

		public:
        	point3(): x(0), y(0), z(0) {};
        	point3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {};
			template <typename U>
			explicit point3(point3<U> &p): x((T)p.x), y((T)p.y), z((T)p.z) {}

			point3<T> operator+(const vector3<T> &v) const
			{
				return point3<T>(x+v.x, y+v.y, z+v.z);
			} 

			vector3<T> operator+(const point3<T> &p) const
			{
				return vector3<T>(x+p.x, y+p.y, z+p.z);
			} 

			point3<T> &operator+=(const vector3<T> &v) const
			{
				x+=v.x; y+=v.y; z+=v.z;
				return *this;
			} 

			point3<T> operator-(const vector3<T> &v) const
			{
				return point3<T>(x-v.x, y-v.y, z-v.z);
			} 

			vector3<T> operator-(const point3<T> &p) const
			{
				return vector3<T>(x-p.x, y-p.y, z-p.z);
			} 

			point3<T> &operator-=(const vector3<T> &v) const
			{
				x-=v.x; y-=v.y; z-=v.z;
				return *this;
			} 

			point3<T> operator[](int i) const
			{
				if(i==0) return x;
				if(i==1) return y;
				return z;
			} 
	};

	//---------- Point2 ----------//
	template <typename T>
	class point2
	{
		public:
			T x,y;

        	point2(): x(0), y(0) {};
        	point2(T _x, T _y): x(_x), y(_y) {};
			point2(point3<T> &p): x(p.x), y(p.y) {};
			template <typename U>
			explicit point2(point2<U> &p): x((T)p.x), y((T)p.y) {}
	};

	//---------- Inline functions ----------//
	// Distance
	template <typename T>
	inline float distance(const point3<T> &p1, const point3<T> &p2)
	{
		return (p1-p2).length();
	}
	template <typename T>
	inline float distance(const point2<T> &p1, const point2<T> &p2)
	{
		return (p1-p2).length();
	}

	// Distance squared
	template <typename T>
	inline float distanceSquared(const point3<T> &p1, const point3<T> &p2)
	{
		return (p1-p2).lengthSquared();
	}
	template <typename T>
	inline float distanceSquared(const point2<T> &p1, const point2<T> &p2)
	{
		return (p1-p2).lengthSquared();
	}

	// Lerp
	template <typename T>
	inline point3<T> lerp(const point3<T> &p1, const point3<T> &p2, float t)
	{
		return (1-t)*p1 + t*p2;
	}
	template <typename T>
	inline point2<T> lerp(const point2<T> &p1, const point2<T> &p2, float t)
	{
		return (1-t)*p1 + t*p2;
	}

	// Min
	template <typename T>
	inline point3<T> min(const point3<T> &p1, const point3<T> &p2)
	{
		return point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
	}
	template <typename T>
	inline point2<T> min(const point2<T> &p1, const point2<T> &p2)
	{
		return point3<T>(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
	}

	// Max
	template <typename T>
	inline point3<T> max(const point3<T> &p1, const point3<T> &p2)
	{
		return point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
	}
	template <typename T>
	inline point2<T> max(const point2<T> &p1, const point2<T> &p2)
	{
		return point3<T>(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
	}

	typedef point3<float> pnt3;
	typedef point3<float> pnt3f;
	typedef point3<double> pnt3d;
	typedef point3<int> pnt3i;

	typedef point2<float> pnt2;
	typedef point2<float> pnt2f;
	typedef point2<double> pnt2d;
	typedef point2<int> pnt2i;
}

#endif// ATTA_POINT_H
