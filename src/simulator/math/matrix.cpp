//--------------------------------------------------
// Atta Math
// matrix.cpp
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "matrix.h"

namespace atta
{

	mat4::mat4()
	{
		data[0] = data[1] = data[2] = data[3] =
			data[4] = data[5] = data[6] = data[7] =
			data[8] = data[9] = data[10] = data[11] = 0;
	}

	mat4::mat4(float diag)
	{
		data[1] = data[2] = data[3] = data[4] = data[6] =
			data[7] = data[8] = data[9] = data[11] = 0;
		data[0] = data[5] = data[10] = diag;
	}

	// Create from column vectors
	mat4::mat4(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &v3)
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

	void mat4::setDiagonal(float a, float b, float c)
	{
		data[0] = a;
		data[5] = b;
		data[10] = c;
	}

	mat4 mat4::operator*(const mat4 &o) const
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

	mat4 mat4::operator+(const mat4 &o) const
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
	vec3 mat4::operator*(const vec3 &vector) const
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
	vec3 mat4::transform(const vec3 &vector) const
	{
		return (*this)*vector;
	}

	// Matrix translated by the vector
	mat4 mat4::translate(const vec3 &vector) const
	{
		return (*this) + mat4(vec3(), vec3(), vec3(), vector);
	}

	// Return the inverse matrix
	mat4 mat4::inverse() const
	{
		mat4 result;
		result.setInverse(*this);
		return result;
	}

	// Invert this matrix
	void mat4::invert()
	{
		setInverse(*this);
	}

	// Transform direction vector
	vec3 mat4::transformDirection(const vec3 &vector) const
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
	vec3 mat4::transformInverseDirection(const vec3 &vector) const
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
	vec3 mat4::transformInverse(const vec3 &vector) const
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
	vec3 mat4::getAxisVector(int i) const
	{
		return vec3(data[i], data[i+4], data[i+8]);
	}

	// Sets this matrix to be the rotation matrix corresponding to
	// the given quaternion.
	void mat4::setOrientationAndPos(const quat &q, const vec3 &pos)
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


    float mat4::getDeterminant() const
	{
		return -data[8]*data[5]*data[2]+
			data[4]*data[9]*data[2]+
			data[8]*data[1]*data[6]-
			data[0]*data[9]*data[6]-
			data[4]*data[1]*data[10]+
			data[0]*data[5]*data[10];
	}

    void mat4::setInverse(const mat4 &m)
	{
		// Make sure the determinant is non-zero
		float det = getDeterminant();
		if (det == 0) return;
		det = 1.0f/det;

		data[0] = (-m.data[9]*m.data[6]+m.data[5]*m.data[10])*det;
		data[4] = (m.data[8]*m.data[6]-m.data[4]*m.data[10])*det;
		data[8] = (-m.data[8]*m.data[5]+m.data[4]*m.data[9])*det;

		data[1] = (m.data[9]*m.data[2]-m.data[1]*m.data[10])*det;
		data[5] = (-m.data[8]*m.data[2]+m.data[0]*m.data[10])*det;
		data[9] = (m.data[8]*m.data[1]-m.data[0]*m.data[9])*det;

		data[2] = (-m.data[5]*m.data[2]+m.data[1]*m.data[6])*det;
		data[6] = (+m.data[4]*m.data[2]-m.data[0]*m.data[6])*det;
		data[10] = (-m.data[4]*m.data[1]+m.data[0]*m.data[5])*det;

		data[3] = (m.data[9]*m.data[6]*m.data[3]
				   -m.data[5]*m.data[10]*m.data[3]
				   -m.data[9]*m.data[2]*m.data[7]
				   +m.data[1]*m.data[10]*m.data[7]
				   +m.data[5]*m.data[2]*m.data[11]
				   -m.data[1]*m.data[6]*m.data[11])*det;
		data[7] = (-m.data[8]*m.data[6]*m.data[3]
				   +m.data[4]*m.data[10]*m.data[3]
				   +m.data[8]*m.data[2]*m.data[7]
				   -m.data[0]*m.data[10]*m.data[7]
				   -m.data[4]*m.data[2]*m.data[11]
				   +m.data[0]*m.data[6]*m.data[11])*det;
		data[11] =(m.data[8]*m.data[5]*m.data[3]
				   -m.data[4]*m.data[9]*m.data[3]
				   -m.data[8]*m.data[1]*m.data[7]
				   +m.data[0]*m.data[9]*m.data[7]
				   +m.data[4]*m.data[1]*m.data[11]
				   -m.data[0]*m.data[5]*m.data[11])*det;
	}

	mat3 mat3::linearInterpolate(const mat3& a, const mat3& b, float prop)
	{
		mat3 result;
		for (unsigned i = 0; i < 9; i++) {
			result.data[i] = a.data[i] * (1-prop) + b.data[i] * prop;
		}
		return result;
	}
}
