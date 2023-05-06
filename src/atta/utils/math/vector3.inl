//--------------------------------------------------
// Atta Math
// vector3.inl
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta {

//----------------------------------------//
//---------------- vector3 ---------------//
//----------------------------------------//
template <typename T>
ATTA_CPU_GPU T vector3<T>::operator[](unsigned i) const {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    return z;
}

template <typename T>
ATTA_CPU_GPU T& vector3<T>::operator[](unsigned i) {
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    return z;
}

// Add vector
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator+=(const vector3<U>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator+(const vector3<U>& v) const {
    return vector3<T>(x + v.x, y + v.y, z + v.z);
}

// Add scalar
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator+=(const U value) {
    x += value;
    y += value;
    z += value;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator+(const U value) const {
    return vector3<T>(x + value, y + value, z + value);
}

// Subtract vector
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator-=(const vector3<U>& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator-(const vector3<U>& v) const {
    return vector3<T>(x - v.x, y - v.y, z - v.z);
}

// Subtract scalar
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator-=(const U value) {
    x -= value;
    y -= value;
    z -= value;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator-(const U value) const {
    return vector3<T>(x - value, y - value, z - value);
}
template <typename T>
ATTA_CPU_GPU vector3<T> vector3<T>::operator-() const {
    return vector3<T>(-x, -y, -z);
}

// Multiply scalar
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator*=(const U value) {
    x *= value;
    y *= value;
    z *= value;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator*(const U value) const {
    return vector3<T>(x * value, y * value, z * value);
}

// Divide scalar
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator/=(const U value) {
    x /= value;
    y /= value;
    z /= value;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator/(const U value) const {
    return vector3<T>(x / value, y / value, z / value);
}

// Divide vector
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator/=(const vector3<U>& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator/(const vector3<U>& v) const {
    return vector3<T>(x / v.x, y / v.y, z / v.z);
}

// Multiply vector
template <typename T>
template <typename U>
ATTA_CPU_GPU void vector3<T>::operator*=(const vector3<U> vector) {
    x *= vector.x;
    y *= vector.y;
    z *= vector.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::operator*(const vector3<U> vector) const {
    return vector3<T>(x * vector.x, y * vector.y, z * vector.z);
}

// Vector products
template <typename T>
template <typename U>
ATTA_CPU_GPU vector3<T> vector3<T>::cross(const vector3<U>& vector) const {
    // Using always double to avoid float-pointing error
    double xd = x, yd = y, zd = z;
    double vxd = vector.x, vyd = vector.y, vzd = vector.z;
    return vector3<T>((yd * vzd) - (zd * vyd), (zd * vxd) - (xd * vzd), (xd * vyd) - (yd * vxd));
}

// Dot
template <typename T>
template <typename U>
ATTA_CPU_GPU auto vector3<T>::dot(const vector3<U>& vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

// Length
template <typename T>
ATTA_CPU_GPU float vector3<T>::length() const {
    return sqrt(squareLength());
}

template <typename T>
ATTA_CPU_GPU float vector3<T>::squareLength() const {
    return x * x + y * y + z * z;
}

// Normalize
template <typename T>
ATTA_CPU_GPU void vector3<T>::normalize() {
    float l = length();
    if (l > 0) {
        (*this) *= 1.0f / l;
    }
}

// Unit
template <typename T>
ATTA_CPU_GPU vector3<T> vector3<T>::unit() const {
    vector3<T> result = *this;
    result.normalize();
    return result;
}

// Logical operators
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator==(const vector3<U>& other) const {
    return x == other.x && y == other.y && z == other.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator!=(const vector3<U>& other) const {
    return !(*this == other);
}
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator<(const vector3<U>& other) const {
    return x < other.x && y < other.y && z < other.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator>(const vector3<U>& other) const {
    return x > other.x && y > other.y && z > other.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator<=(const vector3<U>& other) const {
    return x <= other.x && y <= other.y && z <= other.z;
}
template <typename T>
template <typename U>
ATTA_CPU_GPU bool vector3<T>::operator>=(const vector3<U>& other) const {
    return x >= other.x && y >= other.y && z >= other.z;
}

// Helpers
template <typename T>
ATTA_CPU_GPU void vector3<T>::clear() {
    x = y = z = 0;
}

template <typename T>
ATTA_CPU_GPU void vector3<T>::invert() {
    x = -x;
    y = -y;
    z = -z;
}

// Min max
template <typename T>
ATTA_CPU_GPU T vector3<T>::minComponent() {
    return std::min(x, std::min(y, z));
}
template <typename T>
ATTA_CPU_GPU T vector3<T>::maxComponent() {
    return std::max(x, std::max(y, z));
}

template <typename T>
ATTA_CPU_GPU std::string vector3<T>::toString() const {
    return "vec3{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}";
}

//---------- External ----------//
// Right multiply scalar
template <typename T>
ATTA_CPU_GPU vector3<T> operator*(T value, vector3<T> const& vec) {
    return vec * value;
}

// Cross
template <typename T>
ATTA_CPU_GPU inline vector3<T> cross(const vector3<T>& v1, const vector3<T>& v2) {
    // Using always double to avoid float-pointing error
    double xd = v1.x, yd = v1.y, zd = v1.z;
    double vxd = v2.x, vyd = v2.y, vzd = v2.z;
    return vector3<T>((yd * vzd) - (zd * vyd), (zd * vxd) - (xd * vzd), (xd * vyd) - (yd * vxd));
}

// Normalize
template <typename T>
ATTA_CPU_GPU inline vector3<T> normalize(const vector3<T>& v) {
    float l = v.length();
    if (l > 0)
        return v * 1.0f / l;
    return v;
}

// Dot
template <typename T>
ATTA_CPU_GPU inline auto dot(const vector3<T>& v1, const vector3<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// Length
template <typename T>
ATTA_CPU_GPU inline float length(const vector3<T>& v) {
    return v.length();
}

template <typename T>
ATTA_CPU_GPU inline vector3<T> max(const vector3<T>& v1, const vector3<T>& v2) {
    return {std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z)};
}

template <typename T>
ATTA_CPU_GPU inline vector3<T> min(const vector3<T>& v1, const vector3<T>& v2) {
    return {std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z)};
}

// <<
template <typename T>
ATTA_CPU_GPU inline std::ostream& operator<<(std::ostream& os, const vector3<T>& v) {
    return os << v.toString();
}

} // namespace atta
