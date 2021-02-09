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
#include "simulator/helpers/log.h"

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
        mat4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
        mat4(const vec3 &v0, const vec3 &v1, const vec3 &v2, const vec3 &v3=vec3(0,0,0));
        mat4(const vec4 &v0, const vec4 &v1, const vec4 &v2, const vec4 &v3);
        mat4(const mat3 &mat);

		// Create from base vector and global position
		static mat4 baseAndPos(const vec3 &X, const vec3 &Y, const vec3 &Z, const vec3 &pos);

        float determinant() const;
        void setDiagonal(float a, float b, float c);
        void setInverse(const mat4 &m);
		vec4 col(unsigned i) const;
		vec4 row(unsigned i) const;

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
		mat4 rotate(const vec3 &w, float angle) const;
		mat4 operator*(const float v) const;

        mat4 inverse() const;
        void invert();
        void transpose();

        vec3 transformDirection(const vec3 &vector) const;
        vec3 transformInverseDirection(const vec3 &vector) const;

        vec3 transformInverse(const vec3 &vector) const;
        vec3 getAxisVector(int i) const;
        void setPosOri(const vec3 &pos, const quat &q);
        void setPosOriScale(const vec3 &pos, const quat &q, const vec3 &scale);

        vec3 rollPitchYaw();

		// Glm conversion
		//operator glm::mat4() const
		//{
		//	return glm::make_mat4(data);
		//}

		std::string toString() const;
    };

	inline mat4 operator*(const float value, mat4 const &mat)
	{
		return mat*value;
	}

	// Calculate look at matrix
	inline mat4 lookAt(vec3 eye, vec3 center, vec3 up)
	{
		// Calculate base vectors
		vec3 X,Y,Z;
  		Z = eye - center;
		Z.normalize();
		Y = up;
		X = cross(Y, Z);
		Y = cross(Z, X);
		X.normalize();
		Y.normalize();

		//mat4 res = mat4::baseAndPos(X,Y,Z, -eye);
		//res.data[14]=res.data[11];
		//res.data[11]=0;

		mat4 res;
		res.data[0] = X.x;
		res.data[1] = Y.x;
		res.data[2] = Z.x;
		res.data[3] = 0.0f;

		res.data[4] = X.y;
		res.data[5] = Y.y;
		res.data[6] = Z.y;
		res.data[7] = 0.0f;

		res.data[8] = X.z;
		res.data[9] = Y.z;
		res.data[10] = Z.z;
		res.data[11] = 0.0f;

		res.data[12] = dot(X, -eye);
		res.data[13] = dot(Y, -eye);
		res.data[14] = dot(Z, -eye);
		res.data[15] = 1.0f;

		//Log::debug("MVC", "res: $0", res.toString());

		// Return mat4 from base
		return res;
	}

	// Calculate perspective matrix
	inline mat4 perspective(float fov, float ratio, float near, float far)
	{
		mat4 res(1);
		float frustumDepth = far - near;
    	float oneOverDepth = 1 / frustumDepth;

		res.data[5] = 1 / tan(0.5f * fov);
		res.data[0] = -res.data[5] / ratio;
		res.data[10] = -(far + near) * oneOverDepth;
		res.data[11] = -2*far*near * oneOverDepth;
		res.data[14] = -1;
		res.data[15] = 0;

		return res;
	}

	// Calculate rotation matrix from euler angles
	inline mat4 rotationFromEuler(float R, float P, float Y)
	{
		// Rx(Roll)*Ry(Pitch)*Rz(Yaw)
		return mat4(cos(P)*cos(Y), 							-cos(P)*sin(Y),							sin(P),			0,
					sin(R)*sin(P)*cos(Y)+cos(R)*sin(Y),		-sin(R)*sin(P)*sin(Y)+cos(R)*cos(Y),	-sin(R)*cos(P),	0,
					-cos(R)*sin(P)*cos(Y)+sin(R)*sin(Y),	cos(R)*sin(P)*sin(Y)+sin(R)*cos(Y),		cos(R)*cos(P),	0,
					0,										0,										0,				1);
	}

	// Calculate matrix from position and orientation 
    inline mat4 posOri(const vec3 &pos, const quat &q)
	{
		mat4 res;
		res.setPosOri(pos, q);

		return res;
	}

	// Calculate matrix from position, orientation, and scale
    inline mat4 posOriScale(const vec3 &pos, const quat &q, const vec3 &scale)
	{
		mat4 res;
		res.setPosOriScale(pos, q, scale);

		return res;
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
        void setSkewSymmetric(const vec3 &vec);

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

		// Transpose this matrix
        void transpose();

		// Multiply matrices
        mat3 operator*(const mat3 &o) const;
        mat3 operator()(const mat3 &o) const;

        void operator*=(const mat3 &o);

		// Multiply matrix with scalar
        void operator*=(const float scalar);

		// Component-wise addition
        mat3 operator+(const mat3 &o) const;

		// Component-wise addition
        void operator+=(const mat3 &o);

		// Set the matrix to be the rotation matrix to the given quaternion
        void setOrientation(const quat &q);

		// Interpolate two matrices
        static mat3 linearInterpolate(const mat3& a, const mat3& b, float prop);

		std::string toString() const;
    };

	inline mat3 operator*(const float value, mat3 const &mat)
	{
		return mat*value;
	}

	//------------------------------------------------------------//
	//-------------------------- Inline --------------------------//
	//------------------------------------------------------------//
	inline mat4 transpose(mat4 mat)
	{
		mat4 result = mat;
		result.transpose();
		return result;
	}

	inline mat3 transpose(mat3 mat)
	{
		mat3 result = mat;
		result.transpose();
		return result;
	}

	inline mat4 inverse(mat4 mat)
	{
		mat4 result = mat;
		result.inverse();
		return result;
	}

	inline mat3 inverse(mat3 mat)
	{
		mat3 result = mat;
		result.inverse();
		return result;
	}


	// Calculate rotation matrix from axis and angle
	inline mat4 rotationFromAxisAngle(const vec3 &w, float angle)
	{
		// Rodrigues' formula
		mat3 Jw = mat3(vec3(0, w.z, -w.y), vec3(-w.z, 0, w.x), vec3(w.y, -w.x, 0));

		return mat4(mat3(1.0f) + sin(angle)*Jw + (1-cos(angle))*(Jw*Jw));
	}

}
#endif// ATTA_MATRIX_H
