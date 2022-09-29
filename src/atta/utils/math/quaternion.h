//--------------------------------------------------
// Atta Math
// quaternion.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_MATH_QUATERNION_H
#define ATTA_UTILS_MATH_QUATERNION_H

#include <atta/utils/math/vector.h>

namespace atta {
class mat3;
class quat {
  public:
    float r; // Real component
    float i; // First complex
    float j; // Second complex
    float k; // Third complex

    quat() : r(1), i(0), j(0), k(0) {}
    quat(const vec3& v) { setEuler(v); }
    quat(const float r, const float i, const float j, const float k) : r(r), i(i), j(j), k(k) {}

    void normalize();

    // quat multiplication
    void operator*=(const quat& multiplier);
    quat operator*(const quat& multiplier);
    quat operator-();

    // Multiply scalar
    template <typename U>
    void operator*=(const U value);
    template <typename U>
    quat operator*(const U value) const;

    void operator+=(const vec3& vec);
    bool operator!=(const quat& other);

    void addScaledVector(const vec3& vec, float scale);
    void rotateAroundAxis(const vec3& axis, float angle);

    void setRotationFromVectors(const vec3& before, const vec3& after);
    void setEuler(const vec3& e);
    vec3 getEuler() const;
    void set2DAngle(float angle);
    float get2DAngle() const;
    void setAxisAngle(const vec3& v);
    vec3 getAxisAngle() const;
    mat3 getRotationMatrix() const;

    std::string toString() const;
};

inline quat eulerToQuat(const vec3& e) {
    quat q;
    q.setEuler(e);
    return q;
}

inline vec3 quatToEuler(const quat& q) {
    vec3 e = q.getEuler();
    return e;
}

// <<
inline std::ostream& operator<<(std::ostream& os, const quat& q) { return os << q.toString(); }
} // namespace atta
#endif // ATTA_UTILS_MATH_QUATERNION_H
