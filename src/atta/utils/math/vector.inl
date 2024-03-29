//--------------------------------------------------
// Atta Math
// vector.inl
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta {
//----------------------------------------//
//---------------- vector ----------------//
//----------------------------------------//
template <typename T>
vector<T>::vector() : n(0){};

template <typename T>
vector<T>::vector(size_t _n) : n(_n) {
    data.resize(n);
};

template <typename T>
vector<T>::vector(size_t _n, T val) : n(_n) {
    data.resize(n);
    for (size_t i = 0; i < n; i++)
        data[i] = val;
};

template <typename T>
vector<T>::vector(std::initializer_list<T> l) : n(l.size()) {
    data = l;
}

template <typename T>
vector<T>::~vector() {}

template <typename T>
T& vector<T>::operator[](size_t i) {
    return data[i];
}

template <typename T>
T vector<T>::at(size_t i) const {
    return data.at(i);
}

template <typename T>
template <typename U>
void vector<T>::operator=(std::vector<U> d) {
    data = d;
}

template <typename T>
std::string vector<T>::toString() const {
    std::string res = "[";
    for (size_t i = 0; i < n; i++)
        res += std::to_string(data[i]) + (i != n - 1 ? ", " : "]");

    return res;
}

// ++++++++++
template <typename T>
template <typename U>
vector<T> vector<T>::operator+(const vector<U>& v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = (*this).data[i] + v.data[i];
    return res;
}

template <typename T>
template <typename U>
vector<T> vector<T>::operator+(const U v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = data[i] + v;
    return res;
}

template <typename T>
template <typename U>
void vector<T>::operator+=(const vector<U>& v) {
    for (size_t i = 0; i < n; i++)
        data[i] += v.data[i];
}

template <typename T>
template <typename U>
void vector<T>::operator+=(const U v) {
    for (size_t i = 0; i < n; i++)
        data[i] += v;
}

// ----------
template <typename T>
template <typename U>
vector<T> vector<T>::operator-(const vector<U>& v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = (*this).data[i] - v.data[i];
    return res;
}

template <typename T>
template <typename U>
vector<T> vector<T>::operator-(const U v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = data[i] - v;
    return res;
}

template <typename T>
template <typename U>
void vector<T>::operator-=(const vector<U>& v) {
    for (size_t i = 0; i < n; i++)
        data[i] -= v.data[i];
}

template <typename T>
template <typename U>
void vector<T>::operator-=(const U v) {
    for (size_t i = 0; i < n; i++)
        data[i] -= v;
}

template <typename T>
vector<T>& vector<T>::operator-() {
    for (size_t i = 0; i < n; i++)
        data[i] *= -1;
    return (*this);
}

// **********
template <typename T>
template <typename U>
vector<T> vector<T>::operator*(const vector<U>& v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = (*this).data[i] * v.data[i];
    return res;
}

template <typename T>
template <typename U>
vector<T> vector<T>::operator*(const U v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = data[i] * v;
    return res;
}

template <typename T>
template <typename U>
void vector<T>::operator*=(const vector<U>& v) {
    for (size_t i = 0; i < n; i++)
        data[i] *= v.data[i];
}

template <typename T>
template <typename U>
void vector<T>::operator*=(const U v) {
    for (size_t i = 0; i < n; i++)
        data[i] *= v;
}

// //////////
template <typename T>
template <typename U>
vector<T> vector<T>::operator/(const vector<U>& v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = (*this).data[i] / v.data[i];
    return res;
}

template <typename T>
template <typename U>
vector<T> vector<T>::operator/(const U v) const {
    vector<T> res(n);
    for (size_t i = 0; i < n; i++)
        res[i] = data[i] / v;
    return res;
}

template <typename T>
template <typename U>
void vector<T>::operator/=(const vector<U>& v) {
    for (size_t i = 0; i < n; i++)
        data[i] /= v.data[i];
}

template <typename T>
template <typename U>
void vector<T>::operator/=(const U v) {
    for (size_t i = 0; i < n; i++)
        data[i] /= v;
}

// Dot
template <typename U, typename V>
auto dot(const vector<U>& v1, const vector<V>& v2) {
    decltype(std::declval<U>() * std::declval<V>()) res = 0;
    for (size_t i = 0; i < v1.n; i++)
        res += v1.data[i] * v2.data[i];
    return res;
}

template <typename T>
void vector<T>::normalize() {
    T sum = 0;
    for (const T& val : data)
        sum += val * val;
    for (T& val : data)
        val /= sqrt(sum);
}

template <typename T>
T vector<T>::length() {
    T sum = 0;
    for (const T& val : data)
        sum += val * val;
    return sqrt(sum);
}

// <<
template <typename T>
inline std::ostream& operator<<(std::ostream& os, const vector<T>& v) {
    return os << v.toString();
}
} // namespace atta
