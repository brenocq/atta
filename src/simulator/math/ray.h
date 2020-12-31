//--------------------------------------------------
// Atta Math
// ray.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RAY_H
#define ATTA_RAY_H
#include <math.h>
#include <limits>
#include "simulator/math/math.h"
#include "point.h"
#include "vector.h"

namespace atta
{
	//---------- Ray ----------//
	class ray
	{
		public:
			pnt3 o;
			vec3 d;
			mutable float tMax;
			float time;
			// const Medium medium;

        	ray(): tMax(infinity), time(0.0f) {};
        	ray(pnt3 &o, vec3 &d, float tMax=infinity, float time=0.0f): 
				o(o), d(d), tMax(tMax), time(time) {};

			pnt3 operator()(float t) const { return o + t*d; }
	};

	//---------- Ray Differential ----------//
	class rayDifferential : public ray
	{
		public:
			bool hasDifferentials;
			pnt3 rxOrigin, ryOrigin;
			vec3 rxDirection, ryDirection;

        	rayDifferential() { hasDifferentials = false; };
        	rayDifferential(pnt3 &o, vec3 &d, float tMax=infinity, float time=0.0f): 
				ray(o, d, tMax, time)
			{
				hasDifferentials = false;
			};
        	rayDifferential(ray &r): ray(r) { hasDifferentials = false; };

			void scaleDifferentials(float s)
			{
				rxOrigin = o + (rxOrigin-o)*s;
				ryOrigin = o + (ryOrigin-o)*s;
				rxDirection = d + (rxDirection-d)*s;
				ryDirection = d + (ryDirection-d)*s;
			}
	};
}

#endif// ATTA_RAY_H
