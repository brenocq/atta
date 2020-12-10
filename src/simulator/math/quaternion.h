//--------------------------------------------------
// Atta Math
// quaternion.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_QUATERNION_H
#define ATTA_QUATERNION_H
#include "vector.h"
#include <cmath>
#include <cfloat>

namespace atta
{
    class quat
    {
    public:
        union {
            struct {
                float r;// Real component
                float i;// First complex
                float j;// Second complex
                float k;// Third complex
            };

			// quat data as array
            float data[4];
        };

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
            if(d < FLT_EPSILON) {
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
            r = q.r*multiplier.r - q.i*multiplier.i -
                q.j*multiplier.j - q.k*multiplier.k;
            i = q.r*multiplier.i + q.i*multiplier.r +
                q.j*multiplier.k - q.k*multiplier.j;
            j = q.r*multiplier.j + q.j*multiplier.r +
                q.k*multiplier.i - q.i*multiplier.k;
            k = q.r*multiplier.k + q.k*multiplier.r +
                q.i*multiplier.j - q.j*multiplier.i;
        }

        void addScaledVector(const vec3& vector, float scale)
        {
            quat q(0,
                vector.x * scale,
                vector.y * scale,
                vector.z * scale);
            q *= *this;
            r += q.r * ((float)0.5);
            i += q.i * ((float)0.5);
            j += q.j * ((float)0.5);
            k += q.k * ((float)0.5);
        }

        void rotateByVector(const vec3& vector)
        {
            quat q(0, vector.x, vector.y, vector.z);
            (*this) *= q;
        }
    };
}
#endif// ATTA_QUATERNION_H
