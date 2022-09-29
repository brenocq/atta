//--------------------------------------------------
// Atta Math
// quaternion.cpp
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/math/matrix.h>
#include <atta/utils/math/quaternion.h>

namespace atta {

void quat::normalize() {
    float d = r * r + i * i + j * j + k * k;

    // Check for zero length quaternion, and use the no-rotation
    // quaternion in that case
    if (d < std::numeric_limits<float>::epsilon()) {
        r = 1;
        return;
    }

    d = 1.0f / std::sqrt(d);
    r *= d;
    i *= d;
    j *= d;
    k *= d;
}

// quat multiplication
void quat::operator*=(const quat& multiplier) {
    quat q = *this;
    r = q.r * multiplier.r - q.i * multiplier.i - q.j * multiplier.j - q.k * multiplier.k;
    i = q.r * multiplier.i + q.i * multiplier.r + q.j * multiplier.k - q.k * multiplier.j;
    j = q.r * multiplier.j + q.j * multiplier.r + q.k * multiplier.i - q.i * multiplier.k;
    k = q.r * multiplier.k + q.k * multiplier.r + q.i * multiplier.j - q.j * multiplier.i;
}

quat quat::operator*(const quat& multiplier) {
    quat result = (*this);
    result *= multiplier;
    return result;
}

quat quat::operator-() {
    quat q = (*this);
    q.r *= -1;
    return q;
}

// Multiply scalar
template <typename U>
void quat::operator*=(const U value) {
    r *= value;
    i *= value;
    j *= value;
    k *= value;
}

template <typename U>
quat quat::operator*(const U value) const {
    quat q;
    q.r = r * value;
    q.i = i * value;
    q.j = j * value;
    q.k = k * value;
    return q;
}

void quat::operator+=(const vec3& vec) {
    quat q(0, vec.x, vec.y, vec.z);
    q *= *this;

    r += q.r * 0.5f;
    i += q.i * 0.5f;
    j += q.j * 0.5f;
    k += q.k * 0.5f;
}

bool quat::operator!=(const quat& other) {
    return std::abs(other.i - i) > 0.00001f || std::abs(other.j - j) > 0.00001f || std::abs(other.k - k) > 0.00001f ||
           std::abs(other.r - r) > 0.00001f;
}

void quat::addScaledVector(const vec3& vec, float scale) {
    quat q(0, vec.x * scale, vec.y * scale, vec.z * scale);
    q *= *this;
    r += q.r * 0.5f;
    i += q.i * 0.5f;
    j += q.j * 0.5f;
    k += q.k * 0.5f;
}

void quat::rotateAroundAxis(const vec3& axis, float angle) {
    // Axis must be an unit vector
    // The angle must be in radians
    // Counter clockwise rotation around the axis
    float halfAngle = angle / 2.0f;
    float c = cos(halfAngle);
    float s = sin(halfAngle);
    quat q(c, s * axis.x, s * axis.y, s * axis.z);
    q.normalize();
    normalize();
    (*this) *= q;
}

void quat::setRotationFromVectors(const vec3& before, const vec3& after) {
    // Given two vectors, return the quaternion representing the shortest rotation between the two vectors
    vec3 rotAxis = cross(before, after);
    float cosTheta = dot(before, after);
    float halfAngle = acos(cosTheta) * 0.5f;
    // Degenerated cases
    if (cosTheta == 1) // Equal directions
        return;
    else if (cosTheta == -1) // Opposite directions
    {
        // Choose one of the tangent vectors
        if (before.x > before.y)
            rotAxis = cross(before, vec3(0, 1, 0));
        else
            rotAxis = cross(before, vec3(1, 0, 0));
    }
    rotAxis.normalize();
    float cosHalfAngle = cos(halfAngle);
    float sinHalfAngle = sin(halfAngle);

    // Create rotation quaternion
    r = cosHalfAngle;
    i = sinHalfAngle * rotAxis.x;
    j = sinHalfAngle * rotAxis.y;
    k = sinHalfAngle * rotAxis.z;
}

void quat::setEuler(const vec3& e) {
    // ZYX euler
    r = cos(e.x / 2.0) * cos(e.y / 2.0) * cos(e.z / 2.0) + sin(e.x / 2.0) * sin(e.y / 2.0) * sin(e.z / 2.0);
    i = sin(e.x / 2.0) * cos(e.y / 2.0) * cos(e.z / 2.0) - cos(e.x / 2.0) * sin(e.y / 2.0) * sin(e.z / 2.0);
    j = cos(e.x / 2.0) * sin(e.y / 2.0) * cos(e.z / 2.0) + sin(e.x / 2.0) * cos(e.y / 2.0) * sin(e.z / 2.0);
    k = cos(e.x / 2.0) * cos(e.y / 2.0) * sin(e.z / 2.0) - sin(e.x / 2.0) * sin(e.y / 2.0) * cos(e.z / 2.0);
    normalize();
}

vec3 quat::getEuler() const {
    // ZYX euler
    vec3 e;
    e.x = atan2(2.0 * (r * i + j * k), 1 - 2.0 * (i * i + j * j));
    e.y = asin(2.0 * (r * j - k * i));
    e.z = atan2(2.0 * (r * k + i * j), 1 - 2.0 * (j * j + k * k));
    return e;
}

void quat::set2DAngle(float angle) {
    // ZYX euler
    r = cos(-angle / 2.0);
    i = 0;
    j = 0;
    k = sin(-angle / 2.0);
}

float quat::get2DAngle() const { return -atan2(2.0 * (r * k + i * j), 1 - 2.0 * (j * j + k * k)); }

void quat::setAxisAngle(const vec3& v) {
    // TODO validate
    float angle = v.length();
    vec3 axis = atta::normalize(v);
    float halfAngle = angle / 2.0f;
    float c = cos(halfAngle);
    float s = sin(halfAngle);
    r = c;
    i = s * axis.x;
    j = s * axis.y;
    k = s * axis.z;
}

vec3 quat::getAxisAngle() const {
    vec3 result;
    quat q = *this;
    q.normalize();
    float angle = 2 * acos(r);
    float s = sqrt(1 - r * r);
    if (s < 0.001) {
        result.x = q.i;
        result.y = q.j;
        result.z = q.k;
    } else {
        result.x = q.i / s;
        result.y = q.j / s;
        result.z = q.k / s;
    }
    result *= angle;
    return result;
}

mat3 quat::getRotationMatrix() const {
    mat3 result;
    result.mat[0][0] = 2 * (r * r + i * i) - 1;
    result.mat[0][1] = 2 * (i * j - r * k);
    result.mat[0][2] = 2 * (i * k + r * j);

    result.mat[1][0] = 2 * (i * j + r * k);
    result.mat[1][1] = 2 * (r * r + j * j) - 1;
    result.mat[1][2] = 2 * (j * k - r * i);

    result.mat[2][0] = 2 * (i * k - r * j);
    result.mat[2][1] = 2 * (j * k + r * i);
    result.mat[2][2] = 2 * (r * r + k * k) - 1;

    result.transpose();
    return result;
}

std::string quat::toString() const {
    return "quat{r=" + std::to_string(r) + ", i=" + std::to_string(i) + ", j=" + std::to_string(j) + ", k=" + std::to_string(k) + "}";
}

} // namespace atta
