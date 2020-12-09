//--------------------------------------------------
// Atta - Robot Simulator
// matrix.cpp
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "matrix.h"

namespace atta
{
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
