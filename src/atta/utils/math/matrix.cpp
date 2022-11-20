//--------------------------------------------------
// Atta Math
// matrix.cpp
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/math/matrix.h>

namespace atta {
//------------------------------------------------------------//
//--------------------------- mat4 ---------------------------//
//------------------------------------------------------------//
mat4::mat4() {
    data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = data[9] = data[10] = data[11] = data[12] = data[13] =
        data[14] = data[15] = 0;
}

mat4::mat4(float diag) {
    data[1] = data[2] = data[3] = data[4] = data[6] = data[7] = data[8] = data[9] = data[11] = data[12] = data[13] = data[14] = 0;
    data[0] = data[5] = data[10] = data[15] = diag;
}

mat4::mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33) {
    data[0] = m00;
    data[1] = m01;
    data[2] = m02;
    data[3] = m03;
    data[4] = m10;
    data[5] = m11;
    data[6] = m12;
    data[7] = m13;
    data[8] = m20;
    data[9] = m21;
    data[10] = m22;
    data[11] = m23;
    data[12] = m30;
    data[13] = m31;
    data[14] = m32;
    data[15] = m33;
}

// Create from column vectors
mat4::mat4(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3) {
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
    data[12] = 0.0f;
    data[13] = 0.0f;
    data[14] = 0.0f;
    data[15] = 1.0f;
}

mat4::mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3) {
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
    data[12] = v0.w;
    data[13] = v1.w;
    data[14] = v2.w;
    data[15] = v3.w;
}

mat4::mat4(const mat4& mat) {
    data[0] = mat.data[0];
    data[1] = mat.data[1];
    data[2] = mat.data[2];
    data[3] = mat.data[3];
    data[4] = mat.data[4];
    data[5] = mat.data[5];
    data[6] = mat.data[6];
    data[7] = mat.data[7];
    data[8] = mat.data[8];
    data[9] = mat.data[9];
    data[10] = mat.data[10];
    data[11] = mat.data[11];
    data[12] = mat.data[12];
    data[13] = mat.data[13];
    data[14] = mat.data[14];
    data[15] = mat.data[15];
}

mat4::mat4(const mat3& mat) {
    data[0] = mat.data[0];
    data[1] = mat.data[1];
    data[2] = mat.data[2];
    data[3] = 0;
    data[4] = mat.data[3];
    data[5] = mat.data[4];
    data[6] = mat.data[5];
    data[7] = 0;
    data[8] = mat.data[6];
    data[9] = mat.data[7];
    data[10] = mat.data[8];
    data[11] = 0;

    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 1;
}

// Create from base vector and global position
mat4 mat4::baseAndPos(const vec3& X, const vec3& Y, const vec3& Z, const vec3& pos) {
    mat4 res;
    res.data[0] = X.x;
    res.data[1] = Y.x;
    res.data[2] = Z.x;
    res.data[3] = dot(X, pos);

    res.data[4] = X.y;
    res.data[5] = Y.y;
    res.data[6] = Z.y;
    res.data[7] = dot(Y, pos);

    res.data[8] = X.z;
    res.data[9] = Y.z;
    res.data[10] = Z.z;
    res.data[11] = dot(Z, pos);

    res.data[12] = 0.0f;
    res.data[13] = 0.0f;
    res.data[14] = 0.0f;
    res.data[15] = 1.0f;

    return res;
}

void mat4::setDiagonal(float a, float b, float c) {
    data[0] = a;
    data[5] = b;
    data[10] = c;
    data[15] = 1;
}

vec4 mat4::col(unsigned i) const { return vec4(data[i], data[i + 4], data[i + 8], data[i + 12]); }

vec4 mat4::row(unsigned i) const { return vec4(data[4 * i], data[4 * i + 1], data[4 * i + 2], data[4 * i + 3]); }

void mat4::operator=(const mat4& o) {
    data[0] = o.data[0];
    data[1] = o.data[1];
    data[2] = o.data[2];
    data[3] = o.data[3];
    data[4] = o.data[4];
    data[5] = o.data[5];
    data[6] = o.data[6];
    data[7] = o.data[7];
    data[8] = o.data[8];
    data[9] = o.data[9];
    data[10] = o.data[10];
    data[11] = o.data[11];
    data[12] = o.data[12];
    data[13] = o.data[13];
    data[14] = o.data[14];
    data[15] = o.data[15];
}

mat4 mat4::operator*(const mat4& o) const {
    mat4 result;
    result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]) + (o.data[12] * data[3]);
    result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]) + (o.data[12] * data[7]);
    result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]) + (o.data[12] * data[11]);
    result.data[12] = (o.data[0] * data[12]) + (o.data[4] * data[13]) + (o.data[8] * data[14]) + (o.data[12] * data[15]);

    result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]) + (o.data[13] * data[3]);
    result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]) + (o.data[13] * data[7]);
    result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]) + (o.data[13] * data[11]);
    result.data[13] = (o.data[1] * data[12]) + (o.data[5] * data[13]) + (o.data[9] * data[14]) + (o.data[13] * data[15]);

    result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]) + (o.data[14] * data[3]);
    result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]) + (o.data[14] * data[7]);
    result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]) + (o.data[14] * data[11]);
    result.data[14] = (o.data[2] * data[12]) + (o.data[6] * data[13]) + (o.data[10] * data[14]) + (o.data[14] * data[15]);

    result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + (o.data[15] * data[3]);
    result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + (o.data[15] * data[7]);
    result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + (o.data[15] * data[11]);
    result.data[15] = (o.data[3] * data[12]) + (o.data[7] * data[13]) + (o.data[11] * data[14]) + (o.data[15] * data[15]);

    return result;
}

mat4 mat4::operator()(const mat4& o) const { return (*this) * o; }

mat4 mat4::operator+(const mat4& o) const {
    mat4 result;
    result.data[0] = data[0] + o.data[0];
    result.data[1] = data[1] + o.data[1];
    result.data[2] = data[2] + o.data[2];
    result.data[3] = data[3] + o.data[3];

    result.data[4] = data[4] + o.data[4];
    result.data[5] = data[5] + o.data[5];
    result.data[6] = data[6] + o.data[6];
    result.data[7] = data[7] + o.data[7];

    result.data[8] = data[8] + o.data[8];
    result.data[9] = data[9] + o.data[9];
    result.data[10] = data[10] + o.data[10];
    result.data[11] = data[11] + o.data[11];

    result.data[12] = data[12] + o.data[12];
    result.data[13] = data[13] + o.data[13];
    result.data[14] = data[14] + o.data[14];
    result.data[15] = data[15] + o.data[15];

    return result;
}

mat4 mat4::operator*(const float v) const {
    mat4 result;
    result.data[0] = data[0] * v;
    result.data[1] = data[1] * v;
    result.data[2] = data[2] * v;
    result.data[3] = data[3] * v;

    result.data[4] = data[4] * v;
    result.data[5] = data[5] * v;
    result.data[6] = data[6] * v;
    result.data[7] = data[7] * v;

    result.data[8] = data[8] * v;
    result.data[9] = data[9] * v;
    result.data[10] = data[10] * v;
    result.data[11] = data[11] * v;

    result.data[12] = data[12] * v;
    result.data[13] = data[13] * v;
    result.data[14] = data[14] * v;
    result.data[15] = data[15] * v;

    return result;
}

// Transform the vector by this matrix
vec3 mat4::operator*(const vec3& vector) const {
    return vec3(vector.x * data[0] + vector.y * data[1] + vector.z * data[2] + data[3],

                vector.x * data[4] + vector.y * data[5] + vector.z * data[6] + data[7],

                vector.x * data[8] + vector.y * data[9] + vector.z * data[10] + data[11]);
}

// Transform the vector by this matrix
vec4 mat4::operator*(const vec4& vector) const {
    return vec4(vector.x * data[0] + vector.y * data[1] + vector.z * data[2] + vector.w * data[3],

                vector.x * data[4] + vector.y * data[5] + vector.z * data[6] + vector.w * data[7],

                vector.x * data[8] + vector.y * data[9] + vector.z * data[10] + vector.w * data[11],

                vector.x * data[12] + vector.y * data[13] + vector.z * data[14] + vector.w * data[15]);
}

// Transform the vector by this matrix
vec3 mat4::transform(const vec3& vector) const { return (*this) * vector; }

// Matrix translated by the vector
mat4 mat4::translate(const vec3& vector) const { return (*this) + mat4(vec4(), vec4(), vec4(), vec4(vector, 0)); }

// Matrix rotated by the vector TODO
mat4 mat4::rotate(const vec3& w, float angle) const {
    mat4 rot = rotationFromAxisAngle(w, angle);
    return rot * (*this);
}

// Invert this matrix
void mat4::invert() {
    mat4 ori = *this;
    setInverse(ori);
}

void mat4::transpose() {

    std::swap(data[1], data[4]);
    std::swap(data[2], data[8]);
    std::swap(data[3], data[12]);

    std::swap(data[6], data[9]);
    std::swap(data[7], data[13]);

    std::swap(data[11], data[14]);
}

// Transform direction vector
vec3 mat4::transformDirection(const vec3& vector) const {
    return vec3(vector.x * data[0] + vector.y * data[1] + vector.z * data[2],

                vector.x * data[4] + vector.y * data[5] + vector.z * data[6],

                vector.x * data[8] + vector.y * data[9] + vector.z * data[10]);
}

// Transforms a direction vector (does not change position)
// (To this function give correct results the matrix should be a pure rotation matrix)
// (The inverse of a rotation matrix is its transpose)
vec3 mat4::transformInverseDirection(const vec3& vector) const {
    return vec3(vector.x * data[0] + vector.y * data[4] + vector.z * data[8],

                vector.x * data[1] + vector.y * data[5] + vector.z * data[9],

                vector.x * data[2] + vector.y * data[6] + vector.z * data[10]);
}

// Transforms a vector
// (To this function give correct results the matrix should be a pure rotation matrix)
// (The inverse of a rotation matrix is its transpose)
vec3 mat4::transformInverse(const vec3& vector) const {
    vec3 tmp = vector;
    tmp.x -= data[3];
    tmp.y -= data[7];
    tmp.z -= data[11];
    return vec3(tmp.x * data[0] + tmp.y * data[4] + tmp.z * data[8],

                tmp.x * data[1] + tmp.y * data[5] + tmp.z * data[9],

                tmp.x * data[2] + tmp.y * data[6] + tmp.z * data[10]);
}

// Gets a vector representing one axis (one column) in the matrix.
vec3 mat4::getAxisVector(int i) const { return vec3(data[i], data[i + 4], data[i + 8]); }

// Sets this matrix to be the rotation matrix corresponding to
// the given quaternion.
void mat4::setPosOri(const vec3& pos, const quat& q) {
    mat[0][0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
    mat[0][1] = 2 * q.i * q.j - 2 * q.k * q.r;
    mat[0][2] = 2 * q.i * q.k + 2 * q.j * q.r;
    mat[0][3] = pos.x;

    mat[1][0] = 2 * q.i * q.j + 2 * q.k * q.r;
    mat[1][1] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
    mat[1][2] = 2 * q.j * q.k - 2 * q.i * q.r;
    mat[1][3] = pos.y;

    mat[2][0] = 2 * q.i * q.k - 2 * q.j * q.r;
    mat[2][1] = 2 * q.j * q.k + 2 * q.i * q.r;
    mat[2][2] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
    mat[2][3] = pos.z;

    mat[3][0] = 0;
    mat[3][1] = 0;
    mat[3][2] = 0;
    mat[3][3] = 1;
}

void mat4::setPosOriScale(const vec3& pos, const quat& q, const vec3& scale) {
    data[0] = (1 - (2 * q.j * q.j + 2 * q.k * q.k)) * scale.x;
    data[1] = (2 * q.i * q.j + 2 * q.k * q.r) * scale.y;
    data[2] = (2 * q.i * q.k - 2 * q.j * q.r) * scale.z;
    data[3] = pos.x;

    data[4] = (2 * q.i * q.j - 2 * q.k * q.r) * scale.x;
    data[5] = (1 - (2 * q.i * q.i + 2 * q.k * q.k)) * scale.y;
    data[6] = (2 * q.j * q.k + 2 * q.i * q.r) * scale.z;
    data[7] = pos.y;

    data[8] = (2 * q.i * q.k + 2 * q.j * q.r) * scale.x;
    data[9] = (2 * q.j * q.k - 2 * q.i * q.r) * scale.y;
    data[10] = (1 - (2 * q.i * q.i + 2 * q.j * q.j)) * scale.z;
    data[11] = pos.z;

    data[12] = 0;
    data[13] = 0;
    data[14] = 0;
    data[15] = 1;
}

vec3 mat4::getPosition() const {
    vec3 pos;
    pos.x = mat[0][3];
    pos.y = mat[1][3];
    pos.z = mat[2][3];

    return pos;
}

void mat4::getPosOriScale(vec3& pos, quat& q, vec3& scale) const {
    pos.x = mat[0][3];
    pos.y = mat[1][3];
    pos.z = mat[2][3];
    scale = vec3(length(vec3(mat[0][0], mat[1][0], mat[2][0])), length(vec3(mat[0][1], mat[1][1], mat[2][1])),
                 length(vec3(mat[0][2], mat[1][2], mat[2][2])));

    double b1_squared = 0.25 * (1.0 + mat[0][0] / scale.x + mat[1][1] / scale.y + mat[2][2] / scale.z);
    if (b1_squared > 0.001) {
        double b1 = sqrt(b1_squared);

        double over_b1_4 = 0.25 / b1;
        double b2 = -(mat[2][1] / scale.y - mat[1][2] / scale.z) * over_b1_4;
        double b3 = -(mat[0][2] / scale.z - mat[2][0] / scale.x) * over_b1_4;
        double b4 = -(mat[1][0] / scale.x - mat[0][1] / scale.y) * over_b1_4;

        q = quat(b1, b2, b3, b4);
    }
}

float mat4::determinant() const {
    return data[3] * data[6] * data[9] * data[12] - data[2] * data[7] * data[9] * data[12] - data[3] * data[5] * data[10] * data[12] +
           data[1] * data[7] * data[10] * data[12] + data[2] * data[5] * data[11] * data[12] - data[1] * data[6] * data[11] * data[12] -
           data[3] * data[6] * data[8] * data[13] + data[2] * data[7] * data[8] * data[13] + data[3] * data[4] * data[10] * data[13] -
           data[0] * data[7] * data[10] * data[13] - data[2] * data[4] * data[11] * data[13] + data[0] * data[6] * data[11] * data[13] +
           data[3] * data[5] * data[8] * data[14] - data[1] * data[7] * data[8] * data[14] - data[3] * data[4] * data[9] * data[14] +
           data[0] * data[7] * data[9] * data[14] + data[1] * data[4] * data[11] * data[14] - data[0] * data[5] * data[11] * data[14] -
           data[2] * data[5] * data[8] * data[15] + data[1] * data[6] * data[8] * data[15] + data[2] * data[4] * data[9] * data[15] -
           data[0] * data[6] * data[9] * data[15] - data[1] * data[4] * data[10] * data[15] + data[0] * data[5] * data[10] * data[15];
}

void mat4::setInverse(const mat4& m) {
    // Make sure the determinant is non-zero
    float det = m.determinant();
    if (det == 0) {
        // Log::warning("mat4", "Inverse not possible with zero determinant");
        return;
    }
    float idet = 1.0f / det;

    data[0] = (m.data[5] * m.data[10] * m.data[15] - m.data[5] * m.data[11] * m.data[14] - m.data[9] * m.data[6] * m.data[15] +
               m.data[9] * m.data[7] * m.data[14] + m.data[13] * m.data[6] * m.data[11] - m.data[13] * m.data[7] * m.data[10]) *
              idet;

    data[4] = (-m.data[4] * m.data[10] * m.data[15] + m.data[4] * m.data[11] * m.data[14] + m.data[8] * m.data[6] * m.data[15] -
               m.data[8] * m.data[7] * m.data[14] - m.data[12] * m.data[6] * m.data[11] + m.data[12] * m.data[7] * m.data[10]) *
              idet;

    data[8] = (m.data[4] * m.data[9] * m.data[15] - m.data[4] * m.data[11] * m.data[13] - m.data[8] * m.data[5] * m.data[15] +
               m.data[8] * m.data[7] * m.data[13] + m.data[12] * m.data[5] * m.data[11] - m.data[12] * m.data[7] * m.data[9]) *
              idet;

    data[12] = (-m.data[4] * m.data[9] * m.data[14] + m.data[4] * m.data[10] * m.data[13] + m.data[8] * m.data[5] * m.data[14] -
                m.data[8] * m.data[6] * m.data[13] - m.data[12] * m.data[5] * m.data[10] + m.data[12] * m.data[6] * m.data[9]) *
               idet;

    data[1] = (-m.data[1] * m.data[10] * m.data[15] + m.data[1] * m.data[11] * m.data[14] + m.data[9] * m.data[2] * m.data[15] -
               m.data[9] * m.data[3] * m.data[14] - m.data[13] * m.data[2] * m.data[11] + m.data[13] * m.data[3] * m.data[10]) *
              idet;

    data[5] = (m.data[0] * m.data[10] * m.data[15] - m.data[0] * m.data[11] * m.data[14] - m.data[8] * m.data[2] * m.data[15] +
               m.data[8] * m.data[3] * m.data[14] + m.data[12] * m.data[2] * m.data[11] - m.data[12] * m.data[3] * m.data[10]) *
              idet;

    data[9] = (-m.data[0] * m.data[9] * m.data[15] + m.data[0] * m.data[11] * m.data[13] + m.data[8] * m.data[1] * m.data[15] -
               m.data[8] * m.data[3] * m.data[13] - m.data[12] * m.data[1] * m.data[11] + m.data[12] * m.data[3] * m.data[9]) *
              idet;

    data[13] = (m.data[0] * m.data[9] * m.data[14] - m.data[0] * m.data[10] * m.data[13] - m.data[8] * m.data[1] * m.data[14] +
                m.data[8] * m.data[2] * m.data[13] + m.data[12] * m.data[1] * m.data[10] - m.data[12] * m.data[2] * m.data[9]) *
               idet;

    data[2] = (m.data[1] * m.data[6] * m.data[15] - m.data[1] * m.data[7] * m.data[14] - m.data[5] * m.data[2] * m.data[15] +
               m.data[5] * m.data[3] * m.data[14] + m.data[13] * m.data[2] * m.data[7] - m.data[13] * m.data[3] * m.data[6]) *
              idet;

    data[6] = (-m.data[0] * m.data[6] * m.data[15] + m.data[0] * m.data[7] * m.data[14] + m.data[4] * m.data[2] * m.data[15] -
               m.data[4] * m.data[3] * m.data[14] - m.data[12] * m.data[2] * m.data[7] + m.data[12] * m.data[3] * m.data[6]) *
              idet;

    data[10] = (m.data[0] * m.data[5] * m.data[15] - m.data[0] * m.data[7] * m.data[13] - m.data[4] * m.data[1] * m.data[15] +
                m.data[4] * m.data[3] * m.data[13] + m.data[12] * m.data[1] * m.data[7] - m.data[12] * m.data[3] * m.data[5]) *
               idet;

    data[14] = (-m.data[0] * m.data[5] * m.data[14] + m.data[0] * m.data[6] * m.data[13] + m.data[4] * m.data[1] * m.data[14] -
                m.data[4] * m.data[2] * m.data[13] - m.data[12] * m.data[1] * m.data[6] + m.data[12] * m.data[2] * m.data[5]) *
               idet;

    data[3] = (-m.data[1] * m.data[6] * m.data[11] + m.data[1] * m.data[7] * m.data[10] + m.data[5] * m.data[2] * m.data[11] -
               m.data[5] * m.data[3] * m.data[10] - m.data[9] * m.data[2] * m.data[7] + m.data[9] * m.data[3] * m.data[6]) *
              idet;

    data[7] = (m.data[0] * m.data[6] * m.data[11] - m.data[0] * m.data[7] * m.data[10] - m.data[4] * m.data[2] * m.data[11] +
               m.data[4] * m.data[3] * m.data[10] + m.data[8] * m.data[2] * m.data[7] - m.data[8] * m.data[3] * m.data[6]) *
              idet;

    data[11] = (-m.data[0] * m.data[5] * m.data[11] + m.data[0] * m.data[7] * m.data[9] + m.data[4] * m.data[1] * m.data[11] -
                m.data[4] * m.data[3] * m.data[9] - m.data[8] * m.data[1] * m.data[7] + m.data[8] * m.data[3] * m.data[5]) *
               idet;

    data[15] = (m.data[0] * m.data[5] * m.data[10] - m.data[0] * m.data[6] * m.data[9] - m.data[4] * m.data[1] * m.data[10] +
                m.data[4] * m.data[2] * m.data[9] + m.data[8] * m.data[1] * m.data[6] - m.data[8] * m.data[2] * m.data[5]) *
               idet;
}

std::string mat4::toString() const {
    std::string res =
        "\n[[" + std::to_string(data[0]) + ", " + std::to_string(data[1]) + ", " + std::to_string(data[2]) + ", " + std::to_string(data[3]) + "],";

    res += "\n [" + std::to_string(data[4]) + ", " + std::to_string(data[5]) + ", " + std::to_string(data[6]) + ", " + std::to_string(data[7]) + "],";

    res +=
        "\n [" + std::to_string(data[8]) + ", " + std::to_string(data[9]) + ", " + std::to_string(data[10]) + ", " + std::to_string(data[11]) + "],";
    res += "\n [" + std::to_string(data[12]) + ", " + std::to_string(data[13]) + ", "

           + std::to_string(data[14]) + ", " + std::to_string(data[15]) + "]]\n";
    return res;
}

//------------------------------------------------------------//
//--------------------------- mat3 ---------------------------//
//------------------------------------------------------------//
mat3::mat3() { data[0] = data[1] = data[2] = data[3] = data[4] = data[5] = data[6] = data[7] = data[8] = 0; }

mat3::mat3(float diag) {
    data[1] = data[2] = data[3] = data[5] = data[6] = data[7] = 0;
    data[0] = data[4] = data[8] = diag;
}

// Creates a new matrix from three components (column vectors)
mat3::mat3(const vec3& compOne, const vec3& compTwo, const vec3& compThree) { setComponents(compOne, compTwo, compThree); }

// Creates a new matrix with explicit coefficients.
mat3::mat3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8) {
    data[0] = c0;
    data[1] = c1;
    data[2] = c2;
    data[3] = c3;
    data[4] = c4;
    data[5] = c5;
    data[6] = c6;
    data[7] = c7;
    data[8] = c8;
}

mat3::mat3(const mat4& mat) {
    data[0] = mat.data[0];
    data[1] = mat.data[1];
    data[2] = mat.data[2];
    data[3] = mat.data[4];
    data[4] = mat.data[5];
    data[5] = mat.data[6];
    data[6] = mat.data[8];
    data[7] = mat.data[9];
    data[8] = mat.data[10];
}

void mat3::setDiagonal(float a, float b, float c) {
    data[0] = a;
    data[4] = b;
    data[8] = c;
}

// Set skew symmetric matrix from vector
void mat3::setSkewSymmetric(const vec3& vec) {
    data[0] = data[4] = data[8] = 0;
    data[1] = -vec.z;
    data[2] = vec.y;
    data[3] = vec.z;
    data[5] = -vec.x;
    data[6] = -vec.y;
    data[7] = vec.x;
}

// Set matrix from three column vectors
void mat3::setComponents(const vec3& compOne, const vec3& compTwo, const vec3& compThree) {
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
vec3 mat3::operator*(const vec3& vector) const {
    return vec3(vector.x * data[0] + vector.y * data[1] + vector.z * data[2], vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
                vector.x * data[6] + vector.y * data[7] + vector.z * data[8]);
}

// Transform the vector
vec3 mat3::transform(const vec3& vector) const { return (*this) * vector; }

// Transform the vector by the transpose of this matrix
// Example usage: When multiplying by the inverse of an orthogonal matrix (phy local->world to world->local)
vec3 mat3::transformTranspose(const vec3& vector) const {
    return vec3(vector.x * data[0] + vector.y * data[3] + vector.z * data[6], vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
                vector.x * data[2] + vector.y * data[5] + vector.z * data[8]);
}

// Get row i
vec3 mat3::getRowVector(int i) const { return vec3(data[i * 3], data[i * 3 + 1], data[i * 3 + 2]); }

// Get axis/column i
vec3 mat3::getAxisVector(int i) const { return vec3(data[i], data[i + 3], data[i + 6]); }

// Sets matrix to be the inverse of the another
void mat3::setInverse(const mat3& m) {
    float t4 = m.data[0] * m.data[4];
    float t6 = m.data[0] * m.data[5];
    float t8 = m.data[1] * m.data[3];
    float t10 = m.data[2] * m.data[3];
    float t12 = m.data[1] * m.data[6];
    float t14 = m.data[2] * m.data[6];

    // Calculate the determinant
    float t16 = (t4 * m.data[8] - t6 * m.data[7] - t8 * m.data[8] + t10 * m.data[7] + t12 * m.data[5] - t14 * m.data[4]);

    // Make sure the determinant is non-zero
    if (t16 == (float)0.0f) {
        // Log::warning("mat3", "Inverse not possible with zero determinant");
        return;
    }

    float t17 = 1 / t16;

    data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * t17;
    data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * t17;
    data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * t17;
    data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * t17;
    data[4] = (m.data[0] * m.data[8] - t14) * t17;
    data[5] = -(t6 - t10) * t17;
    data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * t17;
    data[7] = -(m.data[0] * m.data[7] - t12) * t17;
    data[8] = (t4 - t8) * t17;
}

// Inverts this matrix
void mat3::invert() {
    mat3 ori = *this;
    setInverse(ori);
}

void mat3::transpose() {
    std::swap(data[1], data[3]);
    std::swap(data[2], data[6]);
    std::swap(data[5], data[7]);
}

// Multiply matrices
mat3 mat3::operator*(const mat3& o) const {
    return mat3(data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6], data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
                data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

                data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6], data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
                data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

                data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6], data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
                data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]);
}

mat3 mat3::operator()(const mat3& o) const { return (*this) * o; }

void mat3::operator*=(const mat3& o) {
    data[0] = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
    data[1] = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
    data[2] = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];

    data[3] = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
    data[4] = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
    data[5] = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];

    data[6] = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
    data[7] = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
    data[8] = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
}

// Multiply matrix with scalar
mat3 mat3::operator*(const float scalar) const {
    mat3 result;
    result.data[0] = data[0] * scalar;
    result.data[1] = data[1] * scalar;
    result.data[2] = data[2] * scalar;
    result.data[3] = data[3] * scalar;
    result.data[4] = data[4] * scalar;
    result.data[5] = data[5] * scalar;
    result.data[6] = data[6] * scalar;
    result.data[7] = data[7] * scalar;
    result.data[8] = data[8] * scalar;

    return result;
}

void mat3::operator*=(const float scalar) {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;
    data[4] *= scalar;
    data[5] *= scalar;
    data[6] *= scalar;
    data[7] *= scalar;
    data[8] *= scalar;
}

// Component-wise addition
mat3 mat3::operator+(const mat3& o) const {
    mat3 result;
    result.data[0] = data[0] + o.data[0];
    result.data[1] = data[1] + o.data[1];
    result.data[2] = data[2] + o.data[2];

    result.data[3] = data[3] + o.data[3];
    result.data[4] = data[4] + o.data[4];
    result.data[5] = data[5] + o.data[5];

    result.data[6] = data[6] + o.data[6];
    result.data[7] = data[7] + o.data[7];
    result.data[8] = data[8] + o.data[8];

    return result;
}

// Component-wise addition
void mat3::operator+=(const mat3& o) {
    data[0] += o.data[0];
    data[1] += o.data[1];
    data[2] += o.data[2];
    data[3] += o.data[3];
    data[4] += o.data[4];
    data[5] += o.data[5];
    data[6] += o.data[6];
    data[7] += o.data[7];
    data[8] += o.data[8];
}

// Set the matrix to be the rotation matrix to the given quaternion
void mat3::setOrientation(const quat& q) {
    data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
    data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
    data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
    data[3] = 2 * q.i * q.j - 2 * q.k * q.r;
    data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
    data[5] = 2 * q.j * q.k + 2 * q.i * q.r;
    data[6] = 2 * q.i * q.k + 2 * q.j * q.r;
    data[7] = 2 * q.j * q.k - 2 * q.i * q.r;
    data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
}

// Interpolate two matrices
mat3 mat3::linearInterpolate(const mat3& a, const mat3& b, float prop) {
    mat3 result;
    for (unsigned i = 0; i < 9; i++) {
        result.data[i] = a.data[i] * (1 - prop) + b.data[i] * prop;
    }
    return result;
}

std::string mat3::toString() const {
    std::string res = "\n[[" + std::to_string(data[0]) + ", " + std::to_string(data[1]) + ", " + std::to_string(data[2]) + "],";
    res += "\n [" + std::to_string(data[3]) + ", " + std::to_string(data[4]) + ", " + std::to_string(data[5]) + "],";
    res += "\n [" + std::to_string(data[6]) + ", " + std::to_string(data[7]) + ", " + std::to_string(data[8]) + "]]";
    return res;
}

//------------------------------------------------------------//
//--------------------------- mat2 ---------------------------//
//------------------------------------------------------------//
mat2::mat2() { data[0] = data[1] = data[2] = data[3] = 0; }

mat2::mat2(float diag) {
    data[1] = data[2] = 0;
    data[0] = data[3] = diag;
}

// Creates a new matrix from three components (column vectors)
mat2::mat2(const vec2& colOne, const vec2& colTwo) { setColumns(colOne, colTwo); }

// Creates a new matrix with explicit coefficients.
mat2::mat2(float c0, float c1, float c2, float c3) {
    data[0] = c0;
    data[1] = c1;
    data[2] = c2;
    data[3] = c3;
}

mat2::mat2(const mat4& mat) {
    data[0] = mat.data[0];
    data[1] = mat.data[1];
    data[2] = mat.data[4];
    data[3] = mat.data[5];
}

mat2::mat2(const mat3& mat) {
    data[0] = mat.data[0];
    data[1] = mat.data[1];
    data[2] = mat.data[3];
    data[3] = mat.data[4];
}

vec2 mat2::getDiagonal() { return vec2(data[0], data[3]); }

void mat2::setDiagonal(vec2 diag) {
    data[0] = diag.x;
    data[3] = diag.y;
}

void mat2::setDiagonal(float a, float b) {
    data[0] = a;
    data[3] = b;
}

void mat2::setColumns(const vec2& colOne, const vec2& colTwo) {
    data[0] = colOne.x;
    data[1] = colTwo.x;
    data[2] = colOne.y;
    data[3] = colTwo.y;
}

// Transform the vector
vec2 mat2::operator*(const vec2& vector) const { return vec2(vector.x * data[0] + vector.y * data[1], vector.x * data[2] + vector.y * data[3]); }

// Transform the vector
vec2 mat2::transform(const vec2& vector) const { return (*this) * vector; }

// Transform the vector by the transpose of this matrix
// Example usage: When multiplying by the inverse of an orthogonal matrix (phy local->world to world->local)
vec2 mat2::transformTranspose(const vec2& vector) const {
    return vec2(vector.x * data[0] + vector.y * data[2], vector.x * data[1] + vector.y * data[3]);
}

// Get row i
vec2 mat2::getRowVector(int i) const { return vec2(data[i * 2], data[i * 2 + 1]); }

// Get column i
vec2 mat2::getColumnVector(int i) const { return vec2(data[i], data[i + 2]); }

// Sets matrix to be the inverse of the another
void mat2::setInverse(const mat2& m) { LOG_WARN("mat2", "Inverse was not implemented yet"); }

// Inverts this matrix
void mat2::invert() {
    mat2 ori = *this;
    setInverse(ori);
}

void mat2::transpose() { std::swap(data[1], data[2]); }

// Multiply matrices
mat2 mat2::operator*(const mat2& o) const {
    return mat2(data[0] * o.data[0] + data[1] * o.data[2], data[0] * o.data[1] + data[1] * o.data[3],

                data[2] * o.data[0] + data[3] * o.data[2], data[2] * o.data[1] + data[3] * o.data[3]);
}

mat2 mat2::operator()(const mat2& o) const { return (*this) * o; }

void mat2::operator*=(const mat2& o) {
    mat2 res = (*this) * o;
    data[0] = res.data[0];
    data[1] = res.data[1];
    data[2] = res.data[2];
    data[3] = res.data[3];
}

// Multiply matrix with scalar
mat2 mat2::operator*(const float scalar) const {
    mat2 result;
    result.data[0] = data[0] * scalar;
    result.data[1] = data[1] * scalar;
    result.data[2] = data[2] * scalar;
    result.data[3] = data[3] * scalar;

    return result;
}

void mat2::operator*=(const float scalar) {
    data[0] *= scalar;
    data[1] *= scalar;
    data[2] *= scalar;
    data[3] *= scalar;
}

// Component-wise addition
mat2 mat2::operator+(const mat2& o) const {
    mat2 result;
    result.data[0] = data[0] + o.data[0];
    result.data[1] = data[1] + o.data[1];
    result.data[2] = data[2] + o.data[2];
    result.data[3] = data[3] + o.data[3];

    return result;
}

// Component-wise addition
void mat2::operator+=(const mat2& o) {
    data[0] += o.data[0];
    data[1] += o.data[1];
    data[2] += o.data[2];
    data[3] += o.data[3];
}

// Set the matrix to be the rotation matrix to the given quaternion
void mat2::setOrientation(float angle) {
    data[0] = cos(angle);
    data[1] = -sin(angle);
    data[2] = sin(angle);
    data[3] = cos(angle);
}

std::string mat2::toString() const {
    std::string res = "\n[[" + std::to_string(data[0]) + ", " + std::to_string(data[1]) + ", " + "],";
    res += "\n [" + std::to_string(data[2]) + ", " + std::to_string(data[3]) + "]]";
    return res;
}
} // namespace atta
