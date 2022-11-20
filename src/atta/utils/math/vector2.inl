//--------------------------------------------------
// Atta Math
// vector2.inl
// Date: 2021-08-28
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta {

//----------------------------------------//
//---------------- vector2 ---------------//
//----------------------------------------//
// vector2 casting
template <typename T>
template <typename U>
vector2<T>::operator vector2<U>() {
    vector2<U> u;
    u.x = (U)x;
    u.y = (U)y;
    return u;
}

template <typename T>
T vector2<T>::operator[](unsigned i) const {
    if (i == 0)
        return x;
    return y;
}

// Add vector
template <typename T>
template <typename U>
void vector2<T>::operator+=(const vector2<U>& v) {
    x += v.x;
    y += v.y;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator+(const vector2<U>& v) const {
    return vector2<T>(x + v.x, y + v.y);
}

// Add scalar
template <typename T>
template <typename U>
void vector2<T>::operator+=(const U value) {
    x += value;
    y += value;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator+(const U value) const {
    return vector2<T>(x + value, y + value);
}

// Subtract vector
template <typename T>
template <typename U>
void vector2<T>::operator-=(const vector2<U>& v) {
    x -= v.x;
    y -= v.y;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator-(const vector2<U>& v) const {
    return vector2<T>(x - v.x, y - v.y);
}

// Subtract scalar
template <typename T>
template <typename U>
void vector2<T>::operator-=(const U value) {
    x -= value;
    y -= value;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator-(const U value) const {
    return vector2<T>(x - value, y - value);
}
template <typename T>
vector2<T> vector2<T>::operator-() const {
    return vector2<T>(-x, -y);
}

// Multiply scalar
template <typename T>
template <typename U>
void vector2<T>::operator*=(const U value) {
    x *= value;
    y *= value;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator*(const U value) const {
    return vector2<T>(x * value, y * value);
}

// Divide scalar
template <typename T>
template <typename U>
void vector2<T>::operator/=(const U value) {
    x /= value;
    y /= value;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator/(const U value) const {
    return vector2<T>(x / value, y / value);
}

// Divide vector
template <typename T>
template <typename U>
void vector2<T>::operator/=(const vector2<U>& v) {
    x /= v.x;
    y /= v.y;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator/(const vector2<U>& v) const {
    return vector2<T>(x / v.x, y / v.y);
}

// Multiply vector
template <typename T>
template <typename U>
void vector2<T>::operator*=(const vector2<U> vector) {
    x *= vector.x;
    y *= vector.y;
}
template <typename T>
template <typename U>
vector2<T> vector2<T>::operator*(const vector2<U> vector) const {
    return vector2<T>(x * vector.x, y * vector.y);
}

// Dot
template <typename T>
template <typename U>
auto vector2<T>::dot(const vector2<U>& vector) const {
    return x * vector.x + y * vector.y;
}

// Length
template <typename T>
float vector2<T>::length() const {
    return sqrt(squareLength());
}
template <typename T>
float vector2<T>::squareLength() const {
    return x * x + y * y;
}

// Normalize
template <typename T>
void vector2<T>::normalize() {
    float l = length();
    if (l > 0)
        (*this) *= 1.0f / l;
}

// Unit
template <typename T>
vector2<T> vector2<T>::unit() const {
    vector2<T> result = *this;
    result.normalize();
    return result;
}

// Logical operators
template <typename T>
template <typename U>
bool vector2<T>::operator==(const vector2<U>& other) const {
    return x == other.x && y == other.y;
}
template <typename T>
template <typename U>
bool vector2<T>::operator!=(const vector2<U>& other) const {
    return !(*this == other);
}
template <typename T>
template <typename U>
bool vector2<T>::operator<(const vector2<U>& other) const {
    return x < other.x && y < other.y;
}
template <typename T>
template <typename U>
bool vector2<T>::operator>(const vector2<U>& other) const {
    return x > other.x && y > other.y;
}
template <typename T>
template <typename U>
bool vector2<T>::operator<=(const vector2<U>& other) const {
    return x <= other.x && y <= other.y;
}
template <typename T>
template <typename U>
bool vector2<T>::operator>=(const vector2<U>& other) const {
    return x >= other.x && y >= other.y;
}

// Helpers
template <typename T>
void vector2<T>::clear() {
    x = y = 0;
}

template <typename T>
void vector2<T>::invert() {
    x = -x;
    y = -y;
}

// Min max
template <typename T>
T vector2<T>::minComponent() {
    return std::min(x, y);
}
template <typename T>
T vector2<T>::maxComponent() {
    return std::max(x, y);
}

template <typename T>
std::string vector2<T>::toString() const {
    return "vec2{" + std::to_string(x) + ", " + std::to_string(y) + "}";
}

//---------- External ----------//
// Right multiply scalar
template <typename T>
vector2<T> operator*(T value, vector2<T> const& vec) {
    return vec * value;
}

// Normalize
template <typename T>
inline vector2<T> normalize(const vector2<T>& v) {
    float l = v.length();
    if (l > 0)
        return v * 1.0f / l;
    return v;
}

// Dot
template <typename T>
inline auto dot(const vector2<T>& v1, const vector2<T>& v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Length
template <typename T>
inline float length(const vector2<T>& v) {
    return v.length();
}

template <typename T>
inline vector2<T> max(const vector2<T>& v1, const vector2<T>& v2) {
    return {std::max(v1.x, v2.x), std::max(v1.y, v2.y)};
}

template <typename T>
inline vector2<T> min(const vector2<T>& v1, const vector2<T>& v2) {
    return {std::min(v1.x, v2.x), std::min(v1.y, v2.y)};
}

// <<
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const vector2<T>& v) {
    return os << v.toString();
}

} // namespace atta
