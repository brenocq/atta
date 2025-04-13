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
        i = j = k = 0;
        return;
    }

    d = 1.0f / std::sqrt(d);
    r *= d;
    i *= d;
    j *= d;
    k *= d;
}

void quat::inverse() {
    float d = r * r + i * i + j * j + k * k;
    // Check for zero length quaternion, and use the no-rotation
    // quaternion in that case
    if (d < std::numeric_limits<float>::epsilon()) {
        r = 1;
        i = j = k = 0;
        return;
    }

    r = r / d;
    i = -i / d;
    j = -j / d;
    k = -k / d;
}

// quat multiplication
void quat::operator*=(const quat& q2) { *this = (*this) * q2; }

quat quat::operator*(const quat& other) const {
    quat result;
    result.r = r * other.r - i * other.i - j * other.j - k * other.k;
    result.i = r * other.i + i * other.r + j * other.k - k * other.j;
    result.j = r * other.j + j * other.r + k * other.i - i * other.k;
    result.k = r * other.k + k * other.r + i * other.j - j * other.i;
    return result;
}

// vec3 multiplication
void quat::rotateVector(vec3& vec) const {
    vec3 v = vec;
    vec3 u = {i, j, k};
    float s = r;
    vec = 2.0f * dot(u, v) * u + (s * s - dot(u, u)) * v + 2.0f * s * cross(u, v);
}

vec3 quat::operator*(const vec3& vec) const {
    vec3 result = vec;
    rotateVector(result);
    return result;
}

quat quat::operator-() const {
    quat q;
    q.r = -r;
    q.i = -i;
    q.j = -j;
    q.k = -k;
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

bool quat::operator==(const quat& other) const {
    return std::abs(other.i - i) < 0.00001f && std::abs(other.j - j) < 0.00001f && std::abs(other.k - k) < 0.00001f &&
           std::abs(other.r - r) < 0.00001f;
}

bool quat::operator!=(const quat& other) const { return !(*this == other); }

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
    float c = std::cos(halfAngle);
    float s = std::sin(halfAngle);
    quat q(c, s * axis.x, s * axis.y, s * axis.z);
    q.normalize();
    normalize();
    (*this) *= q;
}

void quat::setRotationFromVectors(const vec3& before, const vec3& after) {
    // Given two vectors, compute the quaternion representing the shortest rotation between them

    // It's safer to work with normalized vectors
    vec3 nBefore = before.normalized();
    vec3 nAfter = after.normalized();

    float cosTheta = dot(nBefore, nAfter);

    // Use a tolerance for floating point comparisons to check for parallel vectors
    const float parallelThreshold = 1.0f - std::numeric_limits<float>::epsilon() * 10; // A small tolerance

    if (cosTheta >= parallelThreshold) { // Vectors are pointing in the same direction
        // Set to identity quaternion (no rotation)
        r = 1.0f;
        i = 0.0f;
        j = 0.0f;
        k = 0.0f;
        return;
    }

    // Clamp cosTheta before acos due to potential floating point errors making it slightly > 1 or < -1
    float clampedCosTheta = std::max(-1.0f, std::min(1.0f, cosTheta));
    float angle = std::acos(clampedCosTheta); // Full angle between vectors

    vec3 rotAxis;

    if (cosTheta <= -parallelThreshold) { // Vectors are pointing in opposite directions (180 degrees)
        // Angle is PI. Need to find an arbitrary axis perpendicular to nBefore
        // A robust way to find a perpendicular vector:
        vec3 perpAxis = vec3(1.0f, 0.0f, 0.0f); // Try X-axis
        // If nBefore is (almost) parallel to the chosen axis, pick another one
        if (std::abs(dot(nBefore, perpAxis)) > 0.999f)
            perpAxis = vec3(0.0f, 1.0f, 0.0f); // Try Y-axis
        rotAxis = atta::normalize(cross(nBefore, perpAxis));
        angle = M_PI; // Explicitly set angle to PI for 180 degrees

        // Set using axis-angle formula for 180 degrees (halfAngle = PI/2)
        // r = cos(PI/2) = 0
        // {i, j, k} = sin(PI/2) * rotAxis = rotAxis
        r = 0.0f;
        i = rotAxis.x;
        j = rotAxis.y;
        k = rotAxis.z;
        return; // Done

    } else {
        // General case: vectors are not parallel
        rotAxis = atta::normalize(cross(nBefore, nAfter));
    }

    // Use the axis-angle formula: q = (cos(angle/2), sin(angle/2) * axis)
    float halfAngle = angle * 0.5f;
    float sinHalfAngle = std::sin(halfAngle);

    r = std::cos(halfAngle);
    i = sinHalfAngle * rotAxis.x;
    j = sinHalfAngle * rotAxis.y;
    k = sinHalfAngle * rotAxis.z;
}

void quat::setEuler(const vec3& e) {
    // ZYX euler
    r = std::cos(e.x / 2.0) * std::cos(e.y / 2.0) * std::cos(e.z / 2.0) + std::sin(e.x / 2.0) * std::sin(e.y / 2.0) * std::sin(e.z / 2.0);
    i = std::sin(e.x / 2.0) * std::cos(e.y / 2.0) * std::cos(e.z / 2.0) - std::cos(e.x / 2.0) * std::sin(e.y / 2.0) * std::sin(e.z / 2.0);
    j = std::cos(e.x / 2.0) * std::sin(e.y / 2.0) * std::cos(e.z / 2.0) + std::sin(e.x / 2.0) * std::cos(e.y / 2.0) * std::sin(e.z / 2.0);
    k = std::cos(e.x / 2.0) * std::cos(e.y / 2.0) * std::sin(e.z / 2.0) - std::sin(e.x / 2.0) * std::sin(e.y / 2.0) * std::cos(e.z / 2.0);
    normalize();
}

vec3 quat::getEuler() const {
    // ZYX euler
    vec3 e;
    e.x = std::atan2(2.0 * (r * i + j * k), 1 - 2.0 * (i * i + j * j));
    double sinp = 2.0 * (r * j - k * i);
    if (std::abs(sinp) >= 1)
        e.y = std::copysign(M_PI / 2, sinp);
    else
        e.y = std::asin(sinp);
    e.z = std::atan2(2.0 * (r * k + i * j), 1 - 2.0 * (j * j + k * k));
    return e;
}

void quat::set2DAngle(float angle) {
    r = std::cos(angle / 2.0);
    i = 0;
    j = 0;
    k = std::sin(angle / 2.0);
}

float quat::get2DAngle() const { return std::atan2(2.0 * (r * k + i * j), 1 - 2.0 * (j * j + k * k)); }

void quat::setAxisAngle(const vec3& v, float angle) {
    vec3 axis = atta::normalize(v);
    float halfAngle = angle / 2.0f;
    float c = std::cos(halfAngle);
    float s = std::sin(halfAngle);
    r = c;
    i = s * axis.x;
    j = s * axis.y;
    k = s * axis.z;
}

void quat::getAxisAngle(vec3& v, float& angle) const {
    quat q = *this;
    q.normalize();
    float s = std::sqrt(1 - q.r * q.r);
    vec3 result;
    if (s < 0.001) {
        result.x = q.i;
        result.y = q.j;
        result.z = q.k;
    } else {
        result.x = q.i / s;
        result.y = q.j / s;
        result.z = q.k / s;
    }
    v = result;
    angle = 2 * std::acos(q.r);
}

void quat::setRotationMatrix(const mat3& R) {
    // Compute the trace of the rotation matrix
    float trace = R.mat[0][0] + R.mat[1][1] + R.mat[2][2];

    if (trace > 0) {
        float S = std::sqrt(trace + 1.0f) * 2.0f; // S = 4 * r
        r = 0.25f * S;
        i = (R.mat[2][1] - R.mat[1][2]) / S;
        j = (R.mat[0][2] - R.mat[2][0]) / S;
        k = (R.mat[1][0] - R.mat[0][1]) / S;
    } else if (R.mat[0][0] > R.mat[1][1] && R.mat[0][0] > R.mat[2][2]) {
        float S = std::sqrt(1.0f + R.mat[0][0] - R.mat[1][1] - R.mat[2][2]) * 2.0f; // S = 4 * i
        r = (R.mat[2][1] - R.mat[1][2]) / S;
        i = 0.25f * S;
        j = (R.mat[0][1] + R.mat[1][0]) / S;
        k = (R.mat[0][2] + R.mat[2][0]) / S;
    } else if (R.mat[1][1] > R.mat[2][2]) {
        float S = std::sqrt(1.0f + R.mat[1][1] - R.mat[0][0] - R.mat[2][2]) * 2.0f; // S = 4 * j
        r = (R.mat[0][2] - R.mat[2][0]) / S;
        i = (R.mat[0][1] + R.mat[1][0]) / S;
        j = 0.25f * S;
        k = (R.mat[1][2] + R.mat[2][1]) / S;
    } else {
        float S = std::sqrt(1.0f + R.mat[2][2] - R.mat[0][0] - R.mat[1][1]) * 2.0f; // S = 4 * k
        r = (R.mat[1][0] - R.mat[0][1]) / S;
        i = (R.mat[0][2] + R.mat[2][0]) / S;
        j = (R.mat[1][2] + R.mat[2][1]) / S;
        k = 0.25f * S;
    }
    normalize();
}

mat3 quat::getRotationMatrix() const {
    mat3 R;
    R.mat[0][0] = 2 * (r * r + i * i) - 1;
    R.mat[0][1] = 2 * (i * j - r * k);
    R.mat[0][2] = 2 * (i * k + r * j);

    R.mat[1][0] = 2 * (i * j + r * k);
    R.mat[1][1] = 2 * (r * r + j * j) - 1;
    R.mat[1][2] = 2 * (j * k - r * i);

    R.mat[2][0] = 2 * (i * k - r * j);
    R.mat[2][1] = 2 * (j * k + r * i);
    R.mat[2][2] = 2 * (r * r + k * k) - 1;

    return R;
}

std::string quat::toString() const {
    return "quat{r=" + std::to_string(r) + ", i=" + std::to_string(i) + ", j=" + std::to_string(j) + ", k=" + std::to_string(k) + "}";
}

} // namespace atta
