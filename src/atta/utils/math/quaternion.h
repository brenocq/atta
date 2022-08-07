//--------------------------------------------------
// Atta Math
// quaternion.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_MATH_QUATERNION_H
#define ATTA_CORE_MATH_QUATERNION_H

#include <atta/core/math/vector.h>

namespace atta
{
    class mat3;
    class quat
    {
    public:
        float r;// Real component
        float i;// First complex
        float j;// Second complex
        float k;// Third complex

        quat(): r(1), i(0), j(0), k(0) {}
        quat(const vec3& v) { fromEuler(v); }
        quat(const float r, const float i, const float j, const float k): 
            r(r), i(i), j(j), k(k)
        {}

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

        quat operator -()
        {
            quat q = (*this);
            q.r *= -1;
            return q;
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

        bool operator !=(const quat &other)
        {
            return std::abs(other.i - i) > 0.00001f || 
                std::abs(other.j - j) > 0.00001f || 
                std::abs(other.k - k) > 0.00001f || 
                std::abs(other.r - r) > 0.00001f;
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

        void rotateAroundAxis(const vec3& axis, float angle)
        {
            // Axis must be an unit vector
            // The angle must be in radians
            // Counter clockwise rotation around the axis
            float halfAngle = angle/2.0f;
            float c = cos(halfAngle);
            float s = sin(halfAngle);
            quat q(c, s*axis.x, s*axis.y, s*axis.z);
            q.normalize();
            normalize();
            (*this) *= q;
        }

        void rotationFromVectors(const vec3& before, const vec3& after)
        {
            // Given two vectors, return the quaternion representing the shortest rotation between the two vectors
            vec3 rotAxis = cross(before, after);
            float cosTheta = dot(before, after);
            float halfAngle = acos(cosTheta)*0.5f;
            // Degenerated cases
            if(cosTheta==1)// Equal directions
                return;
            else if(cosTheta==-1)// Opposite directions
            {
                // Choose one of the tangent vectors
                if(before.x>before.y)
                    rotAxis = cross(before, vec3(0,1,0));
                else
                    rotAxis = cross(before, vec3(1,0,0));
            }
            rotAxis.normalize();
            float cosHalfAngle = cos(halfAngle);
            float sinHalfAngle = sin(halfAngle);

            // Create rotation quaternion
            r = cosHalfAngle;
            i = sinHalfAngle*rotAxis.x;
            j = sinHalfAngle*rotAxis.y;
            k = sinHalfAngle*rotAxis.z;
        }

        void fromEuler(const vec3 &e)
        {
            // ZYX euler
            r = cos(e.x/2.0)*cos(e.y/2.0)*cos(e.z/2.0)+sin(e.x/2.0)*sin(e.y/2.0)*sin(e.z/2.0);
            i = sin(e.x/2.0)*cos(e.y/2.0)*cos(e.z/2.0)-cos(e.x/2.0)*sin(e.y/2.0)*sin(e.z/2.0);
            j = cos(e.x/2.0)*sin(e.y/2.0)*cos(e.z/2.0)+sin(e.x/2.0)*cos(e.y/2.0)*sin(e.z/2.0);
            k = cos(e.x/2.0)*cos(e.y/2.0)*sin(e.z/2.0)-sin(e.x/2.0)*sin(e.y/2.0)*cos(e.z/2.0);
            normalize();
        }

        vec3 toEuler() const
        {
            // ZYX euler
            vec3 e;
            e.x = atan2(2.0*(r*i+j*k), 1-2.0*(i*i+j*j));
            e.y = asin(2.0*(r*j-k*i));
            e.z = atan2(2.0*(r*k+i*j), 1-2.0*(j*j+k*k));
            return e;
        }

        void fromAxisAngle(const vec3 &v)
        {
            // TODO validate
            float angle = v.length();
            vec3 axis = atta::normalize(v);
            float halfAngle = angle/2.0f;
            float c = cos(halfAngle);
            float s = sin(halfAngle);
            r = c;
            i = s*axis.x;
            j = s*axis.y;
            k = s*axis.z;
        }

        // TODO
        vec3 toAxisAngle() const;
        // TODO
        mat3 toRotationMatrix() const;

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

    // <<
    inline std::ostream& operator<<(std::ostream& os, const quat& q)
    {
        return os << q.toString();
    }
}
#endif// ATTA_CORE_MATH_QUATERNION_H
