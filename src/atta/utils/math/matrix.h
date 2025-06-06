//--------------------------------------------------
// Atta Math
// matrix.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_MATH_MATRIX_HH
#define ATTA_UTILS_MATH_MATRIX_HH

#include <atta/utils/math/quaternion.h>
#include <atta/utils/math/vector.h>

namespace atta {
class mat3;
class mat4;

//------------------------------------------------------------//
//--------------------------- mat4 ---------------------------//
//------------------------------------------------------------//
class mat4 {
  public:
    union {
        float data[16];
        float mat[4][4];
    };

    mat4();
    mat4(float diag);
    mat4(float m00, float m01, float m02, float m03, float m10, float m11, float m12, float m13, float m20, float m21, float m22, float m23,
         float m30, float m31, float m32, float m33);
    mat4(const vec3& v0, const vec3& v1, const vec3& v2, const vec3& v3 = vec3(0, 0, 0));
    mat4(const vec4& v0, const vec4& v1, const vec4& v2, const vec4& v3);
    mat4(const mat4& mat);
    mat4(const mat3& mat);

    // Create from base vector and global position
    static mat4 baseAndPos(const vec3& X, const vec3& Y, const vec3& Z, const vec3& pos);

    float determinant() const;
    void setDiagonal(float a, float b, float c);
    void setInverse(const mat4& m);
    vec4 col(unsigned i) const;
    vec4 row(unsigned i) const;

    // Assing matrix
    void operator=(const mat4& o);

    // Multiply matrices
    mat4 operator*(const mat4& o) const;
    mat4 operator()(const mat4& o) const;

    // Sum matrices
    mat4 operator+(const mat4& o) const;

    // Multiply by vec4
    vec4 operator*(const vec4& vector) const;

    // Multiply by vec3
    vec3 operator*(const vec3& vector) const;

    vec3 transform(const vec3& vector) const;
    mat4 translate(const vec3& vector) const;
    mat4 rotate(const vec3& w, float angle) const;
    mat4 operator*(const float v) const;

    void invert();
    mat4 inverted() const;

    void transpose();
    mat4 transposed() const;

    vec3 transformDirection(const vec3& vector) const;
    vec3 transformInverseDirection(const vec3& vector) const;

    vec3 transformInverse(const vec3& vector) const;
    vec3 getAxisVector(int i) const;
    void setPosOri(const vec3& pos, const quat& q);
    void setPosOriScale(const vec3& pos, const quat& q, const vec3& scale);

    vec3 getPosition() const;
    quat getOrientation() const;
    vec3 getScale() const;
    void getPosOriScale(vec3& pos, quat& q, vec3& scale) const;

    std::string toString() const;
};

inline mat4 operator*(const float value, mat4 const& mat) { return mat * value; }

// Calculate look at matrix
inline mat4 lookAt(vec3 eye, vec3 center, vec3 up) {
    // Calculate base vectors
    vec3 X, Y, Z;
    Z = eye - center;
    Z.normalize();
    Y = up;
    X = cross(Y, Z);
    Y = cross(Z, X);
    X.normalize();
    Y.normalize();

    mat4 cameraToWorld;
    cameraToWorld.mat[0][0] = X.x;
    cameraToWorld.mat[1][0] = X.y;
    cameraToWorld.mat[2][0] = X.z;
    cameraToWorld.mat[3][0] = 0.0f;

    cameraToWorld.mat[0][1] = Y.x;
    cameraToWorld.mat[1][1] = Y.y;
    cameraToWorld.mat[2][1] = Y.z;
    cameraToWorld.mat[3][1] = 0.0f;

    cameraToWorld.mat[0][2] = Z.x;
    cameraToWorld.mat[1][2] = Z.y;
    cameraToWorld.mat[2][2] = Z.z;
    cameraToWorld.mat[3][2] = 0.0f;

    cameraToWorld.mat[0][3] = eye.x;
    cameraToWorld.mat[1][3] = eye.y;
    cameraToWorld.mat[2][3] = eye.z;
    cameraToWorld.mat[3][3] = 1.0f;

    mat4 worldToCamera;
    worldToCamera.setInverse(cameraToWorld);

    return worldToCamera; // Right-handed
}

// Calculate perspective matrix
inline mat4 perspective(float fov, float ratio, float near, float far) {
    mat4 res(0);
    float oneOverDepth = 1 / (far - near);

    res.mat[1][1] = 1 / tan(0.5f * fov);
    res.mat[0][0] = res.mat[1][1] / ratio;
    res.mat[2][2] = -(far + near) * oneOverDepth;
    res.mat[2][3] = -2 * far * near * oneOverDepth;
    res.mat[3][2] = -1;

    return res;
}

// Calculate orthographic matrix
inline mat4 orthographic(float height, float ratio, float depth = 10) {
    // height: how many meters will appear vertically on the screen
    // ratio: window ratio (width/height)
    // depth: maximum depth to render (in meters)

    mat4 res(1);
    float width = ratio * height;

    res.mat[0][0] = -2 / width;
    res.mat[1][1] = 2 / height;
    res.mat[2][2] = -1 / depth;

    return res;
}

// Calculate rotation matrix from euler angles
inline mat4 rotationFromEuler(float R, float P, float Y) {
    // Rx(Roll)*Ry(Pitch)*Rz(Yaw)
    return mat4(cos(P) * cos(Y), -cos(P) * sin(Y), sin(P), 0, sin(R) * sin(P) * cos(Y) + cos(R) * sin(Y), -sin(R) * sin(P) * sin(Y) + cos(R) * cos(Y),
                -sin(R) * cos(P), 0, -cos(R) * sin(P) * cos(Y) + sin(R) * sin(Y), cos(R) * sin(P) * sin(Y) + sin(R) * cos(Y), cos(R) * cos(P), 0, 0,
                0, 0, 1);
}

// Calculate matrix from position and orientation
inline mat4 posOri(const vec3& pos, const quat& q) {
    mat4 res;
    res.setPosOri(pos, q);
    return res;
}

// Calculate matrix from position, orientation, and scale
inline mat4 posOriScale(const vec3& pos, const quat& q, const vec3& scale) {
    mat4 res;
    res.setPosOriScale(pos, q, scale);
    return res;
}

// <<
inline std::ostream& operator<<(std::ostream& os, const mat4& m) { return os << m.toString(); }

//------------------------------------------------------------//
//--------------------------- mat3 ---------------------------//
//------------------------------------------------------------//
class mat3 {
  public:
    union {
        float data[9];
        float mat[3][3];
    };

    mat3();
    mat3(float diag);
    mat3(const vec3& compOne, const vec3& compTwo, const vec3& compThree);
    mat3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8);
    mat3(const mat4& mat);

    void setDiagonal(float a, float b, float c);

    // Set skew symmetric matrix from vector
    void setSkewSymmetric(const vec3& vec);

    // Set matrix from three column vectors
    void setComponents(const vec3& compOne, const vec3& compTwo, const vec3& compThree);

    // Transform the vector
    vec3 operator*(const vec3& vector) const;

    // Transform the vector
    vec3 transform(const vec3& vector) const;

    // Transform the vector by the transpose of this matrix
    vec3 transformTranspose(const vec3& vector) const;

    // Get row i
    vec3 getRowVector(int i) const;

    // Get axis/column i
    vec3 getAxisVector(int i) const;

    // Sets matrix to be the inverse of the another
    void setInverse(const mat3& m);

    // Inverts this matrix
    void invert();

    // Get the inverted matrix
    mat3 inverted() const;

    // Transpose this matrix
    void transpose();

    // Get the transposed matrix
    mat3 transposed() const;

    // Assing matrix
    // mat3 operator=(const mat3 &o);

    // Multiply matrices
    mat3 operator*(const mat3& o) const;
    mat3 operator()(const mat3& o) const;

    void operator*=(const mat3& o);

    // Multiply matrix with scalar
    mat3 operator*(const float scalar) const;
    void operator*=(const float scalar);

    // Component-wise addition
    mat3 operator+(const mat3& o) const;

    // Component-wise addition
    void operator+=(const mat3& o);

    // Set the matrix to be the rotation matrix to the given quaternion
    void setOrientation(const quat& q);

    // Interpolate two matrices
    static mat3 linearInterpolate(const mat3& a, const mat3& b, float prop);

    std::string toString() const;
};

// <<
inline std::ostream& operator<<(std::ostream& os, const mat3& m) { return os << m.toString(); }

//------------------------------------------------------------//
//--------------------------- mat2 ---------------------------//
//------------------------------------------------------------//
class mat2 {
  public:
    union {
        float data[4];
        float mat[2][2];
    };

    mat2();
    mat2(float diag);
    mat2(const vec2& colOne, const vec2& colTwo);
    mat2(float c0, float c1, float c2, float c3);
    mat2(const mat4& mat);
    mat2(const mat3& mat);

    vec2 getDiagonal();
    void setDiagonal(vec2 diag);
    void setDiagonal(float a, float b);

    // Set matrix from three column vectors
    void setColumns(const vec2& colOne, const vec2& colTwo);

    // Transform the vector
    vec2 operator*(const vec2& vector) const;

    // Transform the vector
    vec2 transform(const vec2& vector) const;

    // Transform the vector by the transpose of this matrix
    vec2 transformTranspose(const vec2& vector) const;

    // Get row i
    vec2 getRowVector(int i) const;

    // Get column i
    vec2 getColumnVector(int i) const;

    // Sets matrix to be the inverse of the another
    void setInverse(const mat2& m);

    // Inverts this matrix
    void invert();

    // Get the inverted matrix
    mat2 inverted() const;

    // Transpose this matrix
    void transpose();

    // Get the transposed matrix
    mat2 transposed() const;

    // Multiply matrices
    mat2 operator*(const mat2& o) const;
    mat2 operator()(const mat2& o) const;

    void operator*=(const mat2& o);

    // Multiply matrix with scalar
    mat2 operator*(const float scalar) const;
    void operator*=(const float scalar);

    // Component-wise addition
    mat2 operator+(const mat2& o) const;

    // Component-wise addition
    void operator+=(const mat2& o);

    // Set the matrix to be the rotation matrix to the given angle
    void setOrientation(float angle);

    std::string toString() const;
};

// <<
inline std::ostream& operator<<(std::ostream& os, const mat2& m) { return os << m.toString(); }

//------------------------------------------------------------//
//--------------------------- mat ---------------------------//
//------------------------------------------------------------//
template <typename T>
class matrix {
  public:
    unsigned nrows, ncols;
    std::vector<vector<T>> rows;

    constexpr matrix() : nrows(0), ncols(0) {}
    matrix(unsigned _nrows, unsigned _ncols);
    matrix(unsigned _nrows, unsigned _ncols, T val);
    template <typename U>
    matrix(const matrix<U>& m);
    ~matrix();

    // Access
    vector<T>& operator[](unsigned i);

    // Basic operations
    // +
    template <typename U>
    matrix<T> operator+(const matrix<U>& o) const;
    template <typename U>
    void operator+=(const matrix<U>& o);
    // -
    template <typename U>
    matrix<T> operator-(const matrix<U>& o) const;
    template <typename U>
    void operator-=(const matrix<U>& o);
    // *
    template <typename U>
    matrix<T> operator*(const matrix<U>& o);
    template <typename U>
    matrix<T> operator*(U v);
    template <typename U>
    void operator*=(const matrix<U>& o);
    template <typename U>
    void operator*=(U v);

    // Matrix operations
    matrix<T>& transpose();

    // Vector operations
    template <typename U>
    vector<U> operator*(const vector<U>& v);

    std::string toString() const;
};

template <typename T>
inline matrix<T> transpose(const matrix<T>& m);

// <<
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const matrix<T>& m) {
    return os << m.toString();
}

//------------------------------------------------------------//
//-------------------------- Inline --------------------------//
//------------------------------------------------------------//
inline mat3 operator*(const float value, const mat3& mat) { return mat * value; }

inline mat4 transpose(mat4 mat) {
    mat4 result = mat;
    result.transpose();
    return result;
}

inline mat3 transpose(mat3 mat) {
    mat3 result = mat;
    result.transpose();
    return result;
}

inline mat4 inverse(mat4 mat) {
    mat4 result;
    result.setInverse(mat);
    return result;
}

inline mat3 inverse(mat3 mat) {
    mat3 result;
    result.setInverse(mat);
    return result;
}

// Calculate rotation matrix from axis and angle
inline mat4 rotationFromAxisAngle(const vec3& w, float angle) {
    // Rodrigues' formula
    mat3 Jw = mat3(vec3(0, w.z, -w.y), vec3(-w.z, 0, w.x), vec3(w.y, -w.x, 0));

    return mat4(mat3(1.0f) + sin(angle) * Jw + (1 - cos(angle)) * (Jw * Jw));
}

using mat = matrix<float>;
using matf = matrix<float>;
using matd = matrix<double>;
using mati = matrix<int>;
} // namespace atta

#include <atta/utils/math/matrix.inl>
#endif // ATTA_UTILS_MATH_MATRIX_H
