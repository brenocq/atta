//--------------------------------------------------
// Atta Math
// bounds.h
// Date: 2020-12-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_MATH_BOUNDS_H
#define ATTA_UTILS_MATH_BOUNDS_H

#include <atta/utils/math/vector.h>
#include <atta/utils/math/ray.h>

namespace atta {
//---------- Bounds 3 ----------//
template <typename T>
class bounds3 {
  public:
    vector3<T> pMin, pMax;

    bounds3() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();

        pMin = vector3<T>(minNum, minNum, minNum);
        pMax = vector3<T>(maxNum, maxNum, maxNum);
    }

    bounds3(const vector3<T>& p) : pMin(p), pMax(p) {}

    bounds3(const vector3<T>& p1, const vector3<T>& p2) : pMin(min(p1, p2)), pMax(max(p1, p2)) {}

    inline bool intersectP(const ray& r, float* hitt0, float* hitt1) const;
};

//---------- Bounds 2 ----------//
template <typename T>
class bounds2 {
  public:
    vector2<T> pMin, pMax;

    bounds2() {
        T minNum = std::numeric_limits<T>::lowest();
        T maxNum = std::numeric_limits<T>::max();

        pMin = vector2<T>(minNum, minNum);
        pMax = vector2<T>(maxNum, maxNum);
    }

    bounds2(const vector2<T>& p) : pMin(p), pMax(p) {}
    bounds2(const vector2<T>& p1, const vector2<T>& p2) : pMin(min(p1, p2)), pMax(max(p1, p2)) {}
};

//---------- Inline functions ----------//
// Union bounds
template <typename T>
inline bounds3<T> unionb(const bounds3<T>& b, const vector3<T>& p) {
    return bounds3<T>(min(b.pMin, p), max(b.pMax, p));
}
template <typename T>
inline bounds2<T> unionb(const bounds2<T>& b, const vector2<T>& p) {
    return bounds2<T>(min(b.pMin, p), max(b.pMax, p));
}

template <typename T>
inline bounds3<T> unionb(const bounds3<T>& b1, const bounds3<T>& b2) {
    return bounds3<T>(min(b1.pMin, b2.pMin), max(b1.pMax, b2.pMax));
}
template <typename T>
inline bounds2<T> unionb(const bounds2<T>& b1, const bounds2<T>& b2) {
    return bounds2<T>(min(b1.pMin, b2.pMin), max(b1.pMax, b2.pMax));
}
// Intersect bounds
template <typename T>
inline bounds3<T> intersect(const bounds3<T>& b1, const bounds3<T>& b2) {
    return bounds3<T>(max(b1.pMin, b2.pMin), min(b1.pMax, b2.pMax));
}
template <typename T>
inline bounds2<T> intersect(const bounds2<T>& b1, const bounds2<T>& b2) {
    return bounds2<T>(max(b1.pMin, b2.pMin), min(b1.pMax, b2.pMax));
}
// Overlaps bounds
template <typename T>
inline bool overlaps(const bounds3<T>& b1, const bounds3<T>& b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    bool z = (b1.pMax.z >= b2.pMin.z) && (b1.pMin.z <= b2.pMax.z);
    return (x && y && z);
}
template <typename T>
inline bool overlaps(const bounds2<T>& b1, const bounds2<T>& b2) {
    bool x = (b1.pMax.x >= b2.pMin.x) && (b1.pMin.x <= b2.pMax.x);
    bool y = (b1.pMax.y >= b2.pMin.y) && (b1.pMin.y <= b2.pMax.y);
    return (x && y);
}
// Inside bounds
template <typename T>
inline bool inside(const vector3<T>& p, const bounds3<T>& b) {
    return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y && p.y <= b.pMax.y && p.z >= b.pMin.z && p.z <= b.pMax.z);
}
template <typename T>
inline bool inside(const vector2<T>& p, const bounds2<T>& b) {
    return (p.x >= b.pMin.x && p.x <= b.pMax.x && p.y >= b.pMin.y && p.y <= b.pMax.y);
}
// Inside exclusive bounds
template <typename T>
inline bool insideExclusive(const vector3<T>& p, const bounds3<T>& b) {
    return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y && p.y < b.pMax.y && p.z >= b.pMin.z && p.z < b.pMax.z);
}
template <typename T>
inline bool insideExclusive(const vector2<T>& p, const bounds2<T>& b) {
    return (p.x >= b.pMin.x && p.x < b.pMax.x && p.y >= b.pMin.y && p.y < b.pMax.y);
}
// Ray-bounds intersection
template <typename T>
inline bool bounds3<T>::intersectP(const ray& r, float* hitt0, float* hitt1) const {
    float t0 = 0, t1 = r.tMax;
    // Compare with each slab
    for (int i = 0; i < 3; i++) {
        float invRayDir = 1 / r.d[i];
        float tNear = (pMin[i] - r.o[i]) * invRayDir;
        float tFar = (pMax[i] - r.o[i]) * invRayDir;

        if (tNear > tFar)
            std::swap(tNear, tFar);
        // Update tFar to ensure robustness
        t0 = tNear > t0 ? tNear : t0;
        t1 = tFar > t1 ? tFar : t1;

        if (t0 > t1)
            return false;
    }
    if (hitt0)
        *hitt0 = t0;
    if (hitt1)
        *hitt1 = t1;
    return true;
}

using bnd3 = bounds3<float>;
using bnd3f = bounds3<float>;
using bnd3d = bounds3<double>;
using bnd3i = bounds3<int>;

using bnd2 = bounds2<float>;
using bnd2f = bounds2<float>;
using bnd2d = bounds2<double>;
using bnd2i = bounds2<int>;
} // namespace atta

#endif // ATTA_UTILS_MATH_BOUNDS_H
