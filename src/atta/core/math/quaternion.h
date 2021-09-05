//--------------------------------------------------
// Atta Math
// quaternion.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_MATH_QUATERNION_H
#define ATTA_CORE_MATH_QUATERNION_H

#include <atta/core/math/vector.h>
//#include <cmath>
//#include <cfloat>

namespace atta
{
    class quat
    {
    public:
		float r;// Real component
		float i;// First complex
		float j;// Second complex
		float k;// Third complex

        quat() : r(1), i(0), j(0), k(0) {}
        quat(const float r, const float i, const float j, const float k)
            : r(r), i(i), j(j), k(k)
        {
        }

        void normalize()
        {
            float d = r*r+i*i+j*j+k*k;

            // Check for zero length quaternion, and use the no-rotation
            // quaternion in that case
            if(d < std::numeric_limits<float>::epsilon()) {
                r = 1;
                return;
            }

            d = 1.0f/std::sqrt(d);
            r *= d;
            i *= d;
            j *= d;
            k *= d;
        }

		// quat multiplication
        void operator *=(const quat &multiplier)
        {
            quat q = *this;
            r = q.r*multiplier.r - q.i*multiplier.i - q.j*multiplier.j - q.k*multiplier.k;
            i = q.r*multiplier.i + q.i*multiplier.r + q.j*multiplier.k - q.k*multiplier.j;
            j = q.r*multiplier.j + q.j*multiplier.r + q.k*multiplier.i - q.i*multiplier.k;
            k = q.r*multiplier.k + q.k*multiplier.r + q.i*multiplier.j - q.j*multiplier.i;
        }

        quat operator *(const quat &multiplier)
        {
			quat result = (*this);
			result*=multiplier;
			return result;
        }

		// Multiply scalar
		template <typename U>
		void operator*=(const U value)
		{
            r *= value;
            i *= value;
            j *= value;
            k *= value;
		}

		template <typename U>
		quat operator*(const U value) const
		{
			quat q;
            q.r = r*value;
            q.i = i*value;
            q.j = j*value;
            q.k = k*value;
			return q;
		}

        void operator +=(const vec3 &vec)
        {
            quat q(0, vec.x, vec.y, vec.z);
            q *= *this;

            r += q.r * 0.5f;
            i += q.i * 0.5f;
            j += q.j * 0.5f;
            k += q.k * 0.5f;
        }

        void addScaledVector(const vec3& vec, float scale)
        {
            quat q(0,
                vec.x * scale,
                vec.y * scale,
                vec.z * scale);
            q *= *this;
            r += q.r * 0.5f;
            i += q.i * 0.5f;
            j += q.j * 0.5f;
            k += q.k * 0.5f;
        }

        void rotateByVector(const vec3& vector)
        {
            quat q(0, vector.x, vector.y, vector.z);
            (*this) *= q;
        }

		void transformVector(const vec3& before, const vec3& after);

		void fromEuler(const vec3 &e)
		{
			r = cos(e.x/2)*cos(e.y/2)*cos(e.z/2)+sin(e.x/2)*sin(e.y/2)*sin(e.z/2);
			i = sin(e.x/2)*cos(e.y/2)*cos(e.z/2)-cos(e.x/2)*sin(e.y/2)*sin(e.z/2);
			j = cos(e.x/2)*sin(e.y/2)*cos(e.z/2)+sin(e.x/2)*cos(e.y/2)*sin(e.z/2);
			k = cos(e.x/2)*cos(e.y/2)*sin(e.z/2)-sin(e.x/2)*sin(e.y/2)*cos(e.z/2);
		}

		vec3 toEuler() const
		{
			vec3 e;
			e.x = atan2(2*(r*i+j*k), 1-2*(i*i+j*j));
			e.y = asin(2*(r*j-k*i));
			e.z = atan2(2*(r*k+i*j), 1-2*(j*j+k*k));
			return e;
		}

		std::string toString() const
		{
			return "quat{r="+std::to_string(r)+", i="+std::to_string(i)+", j="+std::to_string(j)+", k="+std::to_string(k)+"}";
		}
    };

	inline quat eulerToQuat(const vec3 &e)
	{
		quat q;
		q.fromEuler(e);
		return q;
	}

	inline vec3 quatToEuler(const quat &q)
	{
		vec3 e = q.toEuler();
		return e;
	}
}
#endif// ATTA_CORE_MATH_QUATERNION_H
