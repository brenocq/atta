//--------------------------------------------------
// Atta - Robot Simulator
// matrix.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_MATRIX_H
#define ATTA_MATRIX_H
#include "vector.h"
#include "quaternion.h"

namespace atta
{
	//------------------------------------------------------------//
	//--------------------------- mat4 ---------------------------//
	//------------------------------------------------------------//
    class mat4
    {
    public:
        float data[12];

        mat4()
        {
            data[0] = data[1] = data[2] = data[3] =
				data[4] = data[5] = data[6] = data[7] =
				data[8] = data[9] = data[10] = data[11] = 0;
        }

        mat4(float diag)
        {
            data[1] = data[2] = data[3] = data[4] = data[6] =
                data[7] = data[8] = data[9] = data[11] = 0;
            data[0] = data[5] = data[10] = diag;
        }

		// Create from column vectors
        mat4(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &v3)
        {
            data[0] = v0.x;
            data[1] = v1.x;
            data[2] = v2.x;
            data[3] = v3.x;
            data[4] = v0.y;
            data[5] = v1.y;
            data[6] = v2.y;
            data[7] = v3.y;
            data[8] = v0.z;
            data[9] = v1.z;
            data[10] = v2.z;
            data[11] = v3.z;
        }

        void setDiagonal(float a, float b, float c)
        {
            data[0] = a;
            data[5] = b;
            data[10] = c;
        }

        mat4 operator*(const mat4 &o) const
        {
            mat4 result;
            result.data[0] = (o.data[0]*data[0]) + (o.data[4]*data[1]) + (o.data[8]*data[2]);
            result.data[4] = (o.data[0]*data[4]) + (o.data[4]*data[5]) + (o.data[8]*data[6]);
            result.data[8] = (o.data[0]*data[8]) + (o.data[4]*data[9]) + (o.data[8]*data[10]);

            result.data[1] = (o.data[1]*data[0]) + (o.data[5]*data[1]) + (o.data[9]*data[2]);
            result.data[5] = (o.data[1]*data[4]) + (o.data[5]*data[5]) + (o.data[9]*data[6]);
            result.data[9] = (o.data[1]*data[8]) + (o.data[5]*data[9]) + (o.data[9]*data[10]);

            result.data[2] = (o.data[2]*data[0]) + (o.data[6]*data[1]) + (o.data[10]*data[2]);
            result.data[6] = (o.data[2]*data[4]) + (o.data[6]*data[5]) + (o.data[10]*data[6]);
            result.data[10] = (o.data[2]*data[8]) + (o.data[6]*data[9]) + (o.data[10]*data[10]);

            result.data[3] = (o.data[3]*data[0]) + (o.data[7]*data[1]) + (o.data[11]*data[2]) + data[3];
            result.data[7] = (o.data[3]*data[4]) + (o.data[7]*data[5]) + (o.data[11]*data[6]) + data[7];
            result.data[11] = (o.data[3]*data[8]) + (o.data[7]*data[9]) + (o.data[11]*data[10]) + data[11];

            return result;
        }

        mat4 operator+(const mat4 &o) const
        {
            mat4 result;
            result.data[0] = data[0]+o.data[0];
            result.data[1] = data[1]+o.data[1];
            result.data[2] = data[2]+o.data[2];
            result.data[3] = data[3]+o.data[3];

            result.data[4] = data[4]+o.data[4];
            result.data[5] = data[5]+o.data[5];
            result.data[6] = data[6]+o.data[6];
            result.data[7] = data[7]+o.data[7];

            result.data[8] = data[8]+o.data[8];
            result.data[9] = data[9]+o.data[9];
            result.data[10] = data[10]+o.data[10];
            result.data[11] = data[11]+o.data[11];

            return result;
        }

		// Transform the vector by this matrix
        vec3 operator*(const vec3 &vector) const
        {
            return vec3(
                vector.x * data[0] +
                vector.y * data[1] +
                vector.z * data[2] + data[3],

                vector.x * data[4] +
                vector.y * data[5] +
                vector.z * data[6] + data[7],

                vector.x * data[8] +
                vector.y * data[9] +
                vector.z * data[10] + data[11]
            );
        }

		// Transform the vector by this matrix
        vec3 transform(const vec3 &vector) const
        {
            return (*this)*vector;
        }

		// Matrix translated by the vector
        mat4 translate(const vec3 &vector) const
        {
            return (*this) + mat4(vec3(), vec3(), vec3(), vector);
        }

        float getDeterminant() const;
        void setInverse(const mat4 &m);

		// Return the inverse matrix
        mat4 inverse() const
        {
            mat4 result;
            result.setInverse(*this);
            return result;
        }

		// Invert this matrix
        void invert()
        {
            setInverse(*this);
        }

		// Transform direction vector
        vec3 transformDirection(const vec3 &vector) const
        {
            return vec3(
                vector.x * data[0] +
                vector.y * data[1] +
                vector.z * data[2],

                vector.x * data[4] +
                vector.y * data[5] +
                vector.z * data[6],

                vector.x * data[8] +
                vector.y * data[9] +
                vector.z * data[10]
            );
        }

		// Transforms a direction vector (does not change position)
		// (To this function give correct results the matrix should be a pure rotation matrix)
		// (The inverse of a rotation matrix is its transpose)
        vec3 transformInverseDirection(const vec3 &vector) const
        {
            return vec3(
                vector.x * data[0] +
                vector.y * data[4] +
                vector.z * data[8],

                vector.x * data[1] +
                vector.y * data[5] +
                vector.z * data[9],

                vector.x * data[2] +
                vector.y * data[6] +
                vector.z * data[10]
            );
        }

		// Transforms a vector
		// (To this function give correct results the matrix should be a pure rotation matrix)
		// (The inverse of a rotation matrix is its transpose)
        vec3 transformInverse(const vec3 &vector) const
        {
            vec3 tmp = vector;
            tmp.x -= data[3];
            tmp.y -= data[7];
            tmp.z -= data[11];
            return vec3(
                tmp.x * data[0] +
                tmp.y * data[4] +
                tmp.z * data[8],

                tmp.x * data[1] +
                tmp.y * data[5] +
                tmp.z * data[9],

                tmp.x * data[2] +
                tmp.y * data[6] +
                tmp.z * data[10]
            );
        }

		// Gets a vector representing one axis (one column) in the matrix.
        vec3 getAxisVector(int i) const
        {
            return vec3(data[i], data[i+4], data[i+8]);
        }

        // Sets this matrix to be the rotation matrix corresponding to
        // the given quaternion.
        void setOrientationAndPos(const quat &q, const vec3 &pos)
        {
            data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
            data[1] = 2*q.i*q.j + 2*q.k*q.r;
            data[2] = 2*q.i*q.k - 2*q.j*q.r;
            data[3] = pos.x;

            data[4] = 2*q.i*q.j - 2*q.k*q.r;
            data[5] = 1 - (2*q.i*q.i  + 2*q.k*q.k);
            data[6] = 2*q.j*q.k + 2*q.i*q.r;
            data[7] = pos.y;

            data[8] = 2*q.i*q.k + 2*q.j*q.r;
            data[9] = 2*q.j*q.k - 2*q.i*q.r;
            data[10] = 1 - (2*q.i*q.i  + 2*q.j*q.j);
            data[11] = pos.z;
        }
    };
	//------------------------------------------------------------//
	//--------------------------- mat3 ---------------------------//
	//------------------------------------------------------------//
	
    class mat3
    {
    public:
        float data[9];

        mat3()
        {
            data[0] = data[1] = data[2] = data[3] = data[4] 
				= data[5] = data[6] = data[7] = data[8]= 0;
        }

        mat3(float diag)
        {
            data[1] = data[2] = data[3] = data[5] = data[6] = data[7] = 0;
            data[0] = data[4] = data[8] = diag;
        }

		// Creates a new matrix from three components (column vectors)
        mat3(const vec3 &compOne, const vec3 &compTwo,
            const vec3 &compThree)
        {
            setComponents(compOne, compTwo, compThree);
        }

    	// Creates a new matrix with explicit coefficients.
        mat3(float c0, float c1, float c2, float c3, float c4, float c5,
            float c6, float c7, float c8)
        {
            data[0] = c0; data[1] = c1; data[2] = c2;
            data[3] = c3; data[4] = c4; data[5] = c5;
            data[6] = c6; data[7] = c7; data[8] = c8;
        }

        void setDiagonal(float a, float b, float c)
        {
            data[0] = a;
            data[4] = b;
            data[8] = c;
        }


		// Set skew symmetric matrix from vector
        void setSkewSymmetric(const vec3 vector)
        {
            data[0] = data[4] = data[8] = 0;
            data[1] = -vector.z;
            data[2] = vector.y;
            data[3] = vector.z;
            data[5] = -vector.x;
            data[6] = -vector.y;
            data[7] = vector.x;
        }

		// Set matrix from three column vectors
        void setComponents(const vec3 &compOne, const vec3 &compTwo,
            const vec3 &compThree)
        {
            data[0] = compOne.x;
            data[1] = compTwo.x;
            data[2] = compThree.x;
            data[3] = compOne.y;
            data[4] = compTwo.y;
            data[5] = compThree.y;
            data[6] = compOne.z;
            data[7] = compTwo.z;
            data[8] = compThree.z;

        }

		// Transform the vector
        vec3 operator*(const vec3 &vector) const
        {
            return vec3(
                vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
                vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
                vector.x * data[6] + vector.y * data[7] + vector.z * data[8]
            );
        }

		// Transform the vector
        vec3 transform(const vec3 &vector) const
        {
            return (*this) * vector;
        }

		// Transform the vector by the transpose of this matrix
        vec3 transformTranspose(const vec3 &vector) const
        {
            return vec3(
                vector.x * data[0] + vector.y * data[3] + vector.z * data[6],
                vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
                vector.x * data[2] + vector.y * data[5] + vector.z * data[8]
            );
        }

		// Get row i
        vec3 getRowVector(int i) const
        {
            return vec3(data[i*3], data[i*3+1], data[i*3+2]);
        }

		// Get axis/column i
        vec3 getAxisVector(int i) const
        {
            return vec3(data[i], data[i+3], data[i+6]);
        }

		// Sets matrix to be the inverse of the another
        void setInverse(const mat3 &m)
        {
            float t4 = m.data[0]*m.data[4];
            float t6 = m.data[0]*m.data[5];
            float t8 = m.data[1]*m.data[3];
            float t10 = m.data[2]*m.data[3];
            float t12 = m.data[1]*m.data[6];
            float t14 = m.data[2]*m.data[6];

            // Calculate the determinant
            float t16 = (t4*m.data[8] - t6*m.data[7] - t8*m.data[8]+
                        t10*m.data[7] + t12*m.data[5] - t14*m.data[4]);

            // Make sure the determinant is non-zero.
            if (t16 == (float)0.0f) return;
            float t17 = 1/t16;

            data[0] = (m.data[4]*m.data[8]-m.data[5]*m.data[7])*t17;
            data[1] = -(m.data[1]*m.data[8]-m.data[2]*m.data[7])*t17;
            data[2] = (m.data[1]*m.data[5]-m.data[2]*m.data[4])*t17;
            data[3] = -(m.data[3]*m.data[8]-m.data[5]*m.data[6])*t17;
            data[4] = (m.data[0]*m.data[8]-t14)*t17;
            data[5] = -(t6-t10)*t17;
            data[6] = (m.data[3]*m.data[7]-m.data[4]*m.data[6])*t17;
            data[7] = -(m.data[0]*m.data[7]-t12)*t17;
            data[8] = (t4-t8)*t17;
        }

		// Get the inverse
        mat3 inverse() const
        {
            mat3 result;
            result.setInverse(*this);
            return result;
        }

		// Inverts this matrix
        void invert()
        {
            setInverse(*this);
        }

		// Set matrix to be transpose of the given matrix
        void setTranspose(const mat3 &m)
        {
            data[0] = m.data[0];
            data[1] = m.data[3];
            data[2] = m.data[6];
            data[3] = m.data[1];
            data[4] = m.data[4];
            data[5] = m.data[7];
            data[6] = m.data[2];
            data[7] = m.data[5];
            data[8] = m.data[8];
        }

		// Returns the transpose
        mat3 transpose() const
        {
            mat3 result;
            result.setTranspose(*this);
            return result;
        }

		// Multiply matrices
        mat3 operator*(const mat3 &o) const
        {
            return mat3(
                data[0]*o.data[0] + data[1]*o.data[3] + data[2]*o.data[6],
                data[0]*o.data[1] + data[1]*o.data[4] + data[2]*o.data[7],
                data[0]*o.data[2] + data[1]*o.data[5] + data[2]*o.data[8],

                data[3]*o.data[0] + data[4]*o.data[3] + data[5]*o.data[6],
                data[3]*o.data[1] + data[4]*o.data[4] + data[5]*o.data[7],
                data[3]*o.data[2] + data[4]*o.data[5] + data[5]*o.data[8],

                data[6]*o.data[0] + data[7]*o.data[3] + data[8]*o.data[6],
                data[6]*o.data[1] + data[7]*o.data[4] + data[8]*o.data[7],
                data[6]*o.data[2] + data[7]*o.data[5] + data[8]*o.data[8]
                );
        }

        void operator*=(const mat3 &o)
        {
            float t1;
            float t2;
            float t3;

            t1 = data[0]*o.data[0] + data[1]*o.data[3] + data[2]*o.data[6];
            t2 = data[0]*o.data[1] + data[1]*o.data[4] + data[2]*o.data[7];
            t3 = data[0]*o.data[2] + data[1]*o.data[5] + data[2]*o.data[8];
            data[0] = t1;
            data[1] = t2;
            data[2] = t3;

            t1 = data[3]*o.data[0] + data[4]*o.data[3] + data[5]*o.data[6];
            t2 = data[3]*o.data[1] + data[4]*o.data[4] + data[5]*o.data[7];
            t3 = data[3]*o.data[2] + data[4]*o.data[5] + data[5]*o.data[8];
            data[3] = t1;
            data[4] = t2;
            data[5] = t3;

            t1 = data[6]*o.data[0] + data[7]*o.data[3] + data[8]*o.data[6];
            t2 = data[6]*o.data[1] + data[7]*o.data[4] + data[8]*o.data[7];
            t3 = data[6]*o.data[2] + data[7]*o.data[5] + data[8]*o.data[8];
            data[6] = t1;
            data[7] = t2;
            data[8] = t3;
        }

		// Multiply matrix with scalar
        void operator*=(const float scalar)
        {
            data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
            data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
            data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
        }

		// Component-wise addition
        void operator+=(const mat3 &o)
        {
            data[0] += o.data[0]; data[1] += o.data[1]; data[2] += o.data[2];
            data[3] += o.data[3]; data[4] += o.data[4]; data[5] += o.data[5];
            data[6] += o.data[6]; data[7] += o.data[7]; data[8] += o.data[8];
        }

		// Set the matrix to be the rotation matrix to the given quaternion
        void setOrientation(const quat &q)
        {
            data[0] = 1 - (2*q.j*q.j + 2*q.k*q.k);
            data[1] = 2*q.i*q.j + 2*q.k*q.r;
            data[2] = 2*q.i*q.k - 2*q.j*q.r;
            data[3] = 2*q.i*q.j - 2*q.k*q.r;
            data[4] = 1 - (2*q.i*q.i  + 2*q.k*q.k);
            data[5] = 2*q.j*q.k + 2*q.i*q.r;
            data[6] = 2*q.i*q.k + 2*q.j*q.r;
            data[7] = 2*q.j*q.k - 2*q.i*q.r;
            data[8] = 1 - (2*q.i*q.i  + 2*q.j*q.j);
        }

		// Interpolate two matrices
        static mat3 linearInterpolate(const mat3& a, const mat3& b, float prop);

		//---------- Physics tensor calculations ----------//
		// Set matrix from inertia tensor values
        void setInertiaTensorCoeffs(float ix, float iy, float iz,
            float ixy=0, float ixz=0, float iyz=0)
        {
            data[0] = ix;
            data[1] = data[3] = -ixy;
            data[2] = data[6] = -ixz;
            data[4] = iy;
            data[5] = data[7] = -iyz;
            data[8] = iz;
        }

		// Set matrix as an inertia tensor of a rectangular block
		// TODO calculate for each shape
        void setBlockInertiaTensor(const vec3 &halfSizes, float mass)
        {
            vec3 squares = halfSizes*halfSizes;
            setInertiaTensorCoeffs(0.3f*mass*(squares.y + squares.z),
                0.3f*mass*(squares.x + squares.z),
                0.3f*mass*(squares.x + squares.y));
        }

    };

}
#endif// ATTA_MATRIX_H
