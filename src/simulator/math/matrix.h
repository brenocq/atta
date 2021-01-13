//--------------------------------------------------
// Atta Math
// matrix.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MATRIX_H
#define ATTA_MATRIX_H

#include "vector.h"
#include "point.h"
#include "quaternion.h"
#include "glm.h"

namespace atta
{
	class mat3;
	class mat4;
	//------------------------------------------------------------//
	//--------------------------- mat4 ---------------------------//
	//------------------------------------------------------------//
    class mat4
    {
    public:
        float data[16];

        mat4();
        mat4(float diag);
        mat4(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &v3);
        mat4(const mat3 &mat);

        float getDeterminant() const;
        void setDiagonal(float a, float b, float c);
        void setInverse(const mat4 &m);
		vec4 getCol(unsigned i) const;
		vec4 getRow(unsigned i) const;

        mat4 operator*(const mat4 &o) const;
        mat4 operator()(const mat4 &o) const;
        mat4 operator+(const mat4 &o) const;
        vec3 operator*(const vec3 &vector) const;
		template<typename T>
        point3<T> operator*(const point3<T> &p) const
		{
			T x=p.x, y=p.y, z=p.z;
			T xp = data[0]*x + data[1]*y + data[2]*z + data[3];
			T yp = data[4]*x + data[5]*y + data[6]*z + data[7];
			T zp = data[8]*x + data[9]*y + data[10]*z + data[11];
			T wp = data[12]*x + data[13]*y + data[14]*z + data[15];
			if(wp == 1) return point3<T>(xp, yp, zp);
			else return point3<T>(xp, yp, zp)/wp;
		}

        vec3 transform(const vec3 &vector) const;
        mat4 translate(const vec3 &vector) const;
		mat4 rotate(float angle, const vec3 &vector) const;
		mat4 operator*(const float v) const;

        mat4 inverse() const;
        void invert();
        void transpose();

        vec3 transformDirection(const vec3 &vector) const;
        vec3 transformInverseDirection(const vec3 &vector) const;

        vec3 transformInverse(const vec3 &vector) const;
        vec3 getAxisVector(int i) const;
        void setOrientationAndPos(const quat &q, const vec3 &pos);
        void setPosOriScale(const vec3 &pos, const quat &q, const vec3 &scale);

		// Glm conversion
		operator glm::mat4() const
		{
			return glm::make_mat4(data);
		}
    };

	// Return the inverse matrix
	inline mat4 inverse(mat4 mat)
	{
		mat4 result;
		result.setInverse(mat);
		return result;
	}
	//------------------------------------------------------------//
	//--------------------------- mat3 ---------------------------//
	//------------------------------------------------------------//
	
    class mat3
    {
    public:
        float data[9];

        mat3();
        mat3(float diag);
        mat3(const vec3 &compOne, const vec3 &compTwo, const vec3 &compThree);
        mat3(float c0, float c1, float c2, 
				float c3, float c4, float c5,
            	float c6, float c7, float c8);
		mat3(const mat4 &mat);

        void setDiagonal(float a, float b, float c);


		// Set skew symmetric matrix from vector
        void setSkewSymmetric(const vec3 vector);

		// Set matrix from three column vectors
        void setComponents(const vec3 &compOne, const vec3 &compTwo,
            const vec3 &compThree);

		// Transform the vector
        vec3 operator*(const vec3 &vector) const;

		// Transform the vector
        vec3 transform(const vec3 &vector) const;

		// Transform the vector by the transpose of this matrix
        vec3 transformTranspose(const vec3 &vector) const;

		// Get row i
        vec3 getRowVector(int i) const;

		// Get axis/column i
        vec3 getAxisVector(int i) const;

		// Sets matrix to be the inverse of the another
        void setInverse(const mat3 &m);

		// Get the inverse
        mat3 inverse() const;

		// Inverts this matrix
        void invert();

		// Set matrix to be transpose of the given matrix
        void setTranspose(const mat3 &m);

		// Returns the transpose
        mat3 transpose() const;

		// Multiply matrices
        mat3 operator*(const mat3 &o) const;

        void operator*=(const mat3 &o);

		// Multiply matrix with scalar
        void operator*=(const float scalar);

		// Component-wise addition
        void operator+=(const mat3 &o);

		// Set the matrix to be the rotation matrix to the given quaternion
        void setOrientation(const quat &q);

		// Interpolate two matrices
        static mat3 linearInterpolate(const mat3& a, const mat3& b, float prop);

		//---------- Physics tensor calculations ----------//
		// Set matrix from inertia tensor values
        void setInertiaTensorCoeffs(
				float ix, float iy, float iz,
            	float ixy=0, float ixz=0, float iyz=0);

		// Set matrix as an inertia tensor of a rectangular block
		// TODO calculate for each shape
        void setBlockInertiaTensor(const vec3 &halfSizes, float mass);

    };

}
#endif// ATTA_MATRIX_H
