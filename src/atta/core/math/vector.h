//--------------------------------------------------
// Atta Math
// vector.h
// Date: 2020-12-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_MATH_VECTOR_H
#define ATTA_CORE_MATH_VECTOR_H

namespace atta
{
    template <typename T>
    class vector4;
    template <typename T>
    class vector3;
    template <typename T>
    class vector2;

    //----------------------------------------//
    //--------------- Vector 4 ---------------//
    //----------------------------------------//
    template <typename T>
    class vector4
    {
    public:
        T x, y, z, w;

    public:
        vector4(): x(0), y(0), z(0), w(0) {}
        vector4(T _x, T _y, T _z, T _w): x(_x), y(_y), z(_z), w(_w) {}
        vector4(T _x): x(_x), y(_x), z(_x), w(_x) {}
        vector4(vector3<T> vector, T _w): x(vector.x), y(vector.y), z(vector.z), w(_w) {}
        vector4(T _x, vector3<T> vector): x(_x), y(vector.x), z(vector.y), w(vector.z) {}
        vector4(vector2<T> vector, T _z, T _w): x(vector.x), y(vector.y), z(_z), w(_w) {}
        vector4(T _x, vector2<T> vector, T _w): x(_x), y(vector.x), z(vector.y), w(_w) {}
        vector4(vector2<T> v0, vector2<T> v1): x(v0.x), y(v0.y), z(v1.x), w(v1.y) {}
        vector4(T _x, T _y, vector2<T> vector): x(_x), y(_y), z(vector.x), w(vector.y) {}

        T operator[](unsigned i) const;

        // Add vector
        template <typename U>
        void operator+=(const vector4<U>& v);
        template <typename U>
        vector4<T> operator+(const vector4<U>& v) const;

        // Add scalar
        template <typename U>
        void operator+=(const U value);
        template <typename U>
        vector4<T> operator+(const U value) const;

        // Subtract vector
        template <typename U>
        void operator-=(const vector4<U>& v);
        template <typename U>
        vector4<T> operator-(const vector4<U>& v) const;

        // Subtract scalar
        template <typename U>
        void operator-=(const U value);
        template <typename U>
        vector4<T> operator-(const U value) const;
        vector4<T> operator-() const;

        // Multiply scalar
        template <typename U>
        void operator*=(const U value);
        template <typename U>
        vector4<T> operator*(const U value) const;

        // Divide scalar
        template <typename U>
        void operator/=(const U value);
        template <typename U>
        vector4<T> operator/(const U value) const;

        // Divide vector
        template <typename U>
        void operator/=(const vector4<U>& v);
        template <typename U>
        vector4<T> operator/(const vector4<U>& v) const;

        // Multiply vector
        template <typename U>
        void operator*=(const vector4<U> vector);
        template <typename U>
        vector4<T> operator*(const vector4<U> vector) const;

        // Dot
        template <typename U>
        auto dot(const vector4<U> &vec) const;

        // Length
        float length() const;
        float squareLength() const;

        // Normalize
        void normalize();

        // Unit
        vector4<T> unit() const;

        // Logical operators
        template <typename U>
        bool operator==(const vector4<U>& other) const;
        template <typename U>
        bool operator!=(const vector4<U>& other) const;
        template <typename U>
        bool operator<(const vector4<U>& other) const;
        template <typename U>
        bool operator>(const vector4<U>& other) const;
        template <typename U>
        bool operator<=(const vector4<U>& other) const;
        template <typename U>
        bool operator>=(const vector4<U>& other) const;

        // Helpers
        void clear();
        void invert();
        T maxComponent();
        T minComponent();
        std::string toString() const;
    };

    // Right multiply scalar
    template <typename T>
    vector4<T> operator*(T value, vector4<T> const& vec);

    // Normalize
    template <typename T>
    inline vector4<T> normalize(const vector4<T>& v);

    // Dot
    template <typename T>
    inline auto dot(const vector4<T>& v1, const vector4<T>& v2);

    // Length
    template <typename T>
    inline float length(const vector4<T>& v);

    // <<
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const vector4<T>& v);

    //----------------------------------------//
    //--------------- Vector 3 ---------------//
    //----------------------------------------//
    template <typename T>
    class vector3
    {
    public:
        T x, y, z;
        //private:
        //	T pad;// 4 word alignment

    public:
        vector3(): x(0), y(0), z(0) {}
        vector3(T _x): x(_x), y(_x), z(_x) {}
        vector3(T _x, T _y, T _z): x(_x), y(_y), z(_z) {}
        template <typename U>
        vector3(vector4<U> v): x(v.x), y(v.y), z(v.z) {}
        template <typename U>
        vector3(vector2<U> v, T _z = 0.0f): x(v.x), y(v.y), z(_z) {}
        template <typename U>
        vector3(T _x, vector2<U> v): x(_x), y(v.x), z(v.y) {}

        T operator[](unsigned i) const;

        // Add vector
        template <typename U>
        void operator+=(const vector3<U>& v);
        template <typename U>
        vector3<T> operator+(const vector3<U>& v) const;

        // Add scalar
        template <typename U>
        void operator+=(const U value);
        template <typename U>
        vector3<T> operator+(const U value) const;

        // Subtract vector
        template <typename U>
        void operator-=(const vector3<U>& v);
        template <typename U>
        vector3<T> operator-(const vector3<U>& v) const;

        // Subtract scalar
        template <typename U>
        void operator-=(const U value);
        template <typename U>
        vector3<T> operator-(const U value) const;
        vector3<T> operator-() const;

        // Multiply vector
        template <typename U>
        void operator*=(const vector3<U> vector);
        template <typename U>
        vector3<T> operator*(const vector3<U> vector) const;

        // Multiply scalar
        template <typename U>
        void operator*=(const U value);
        template <typename U>
        vector3<T> operator*(const U value) const;

        // Divide vector
        template <typename U>
        void operator/=(const vector3<U>& v);
        template <typename U>
        vector3<T> operator/(const vector3<U>& v) const;

        // Divide scalar
        template <typename U>
        void operator/=(const U value);
        template <typename U>
        vector3<T> operator/(const U value) const;

        // Vector products
        template <typename U>
        vector3<T> cross(const vector3<U> &vector) const;

        // Dot
        template <typename U>
        auto dot(const vector3<U> &vector) const;

        // Length
        float length() const;
        float squareLength() const;

        // Normalize
        void normalize();

        // Unit
        vector3<T> unit() const;

        // Logical operators
        template <typename U>
        bool operator==(const vector3<U>& other) const;
        template <typename U>
        bool operator!=(const vector3<U>& other) const;
        template <typename U>
        bool operator<(const vector3<U>& other) const;
        template <typename U>
        bool operator>(const vector3<U>& other) const;
        template <typename U>
        bool operator<=(const vector3<U>& other) const;
        template <typename U>
        bool operator>=(const vector3<U>& other) const;

        // Helpers
        void clear();
        void invert();

        // Min max
        T minComponent();
        T maxComponent();

        std::string toString() const;
    };

    // Right multiply scalar
    template <typename T>
    vector3<T> operator*(T value, vector3<T> const &vec);

    // Cross
    template <typename T>
    inline vector3<T> cross(const vector3<T> &v1, const vector3<T> &v2);

    // Normalize
    template <typename T>
    inline vector3<T> normalize(const vector3<T> &v);

    // Dot
    template <typename T>
    inline auto dot(const vector3<T> &v1, const vector3<T> &v2);

    // Length
    template <typename T>
    inline float length(const vector3<T>& v);

    // <<
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const vector3<T>& v);

    //----------------------------------------//
    //--------------- Vector 2 ---------------//
    //----------------------------------------//
    template <typename T>
    class vector2
    {
    public:
        T x, y;

        vector2(): x(0), y(0) {}
        vector2(T _x, T _y): x(_x), y(_y) {}
        vector2(T _x): x(_x), y(_x) {}
        template <typename U>
        vector2(vector3<U> v): x(v.x), y(v.y) {}
        template <typename U>
        vector2(vector4<U> v): x(v.x), y(v.y) {}

        template <typename U>
        void operator=(vector2<U> v);

        // vector2 casting
        template <typename U>
        operator vector2<U>();

        T operator[](unsigned i) const;

        // Add vector
        template <typename U>
        void operator+=(const vector2<U>& v);
        template <typename U>
        vector2<T> operator+(const vector2<U>& v) const;

        // Add scalar
        template <typename U>
        void operator+=(const U v);
        template <typename U>
        vector2<T> operator+(const U v) const;

        // Subtract vector
        template <typename U>
        void operator-=(const vector2<U>& v);
        template <typename U>
        vector2<T> operator-(const vector2<U>& v) const;

        // Subtract scalar
        template <typename U>
        void operator-=(const U value);
        template <typename U>
        vector2<T> operator-(const U value) const;
        vector2<T> operator-() const;

        // Multiply scalar
        template <typename U>
        void operator*=(const U value);
        template <typename U>
        vector2<T> operator*(const U value) const;

        // Divide scalar
        template <typename U>
        void operator/=(const U value);
        template <typename U>
        vector2<T> operator/(const U value) const;

        // Divide vector
        template <typename U>
        void operator/=(const vector2<U>& v);
        template <typename U>
        vector2<T> operator/(const vector2<U>& v) const;

        // Multiply vector
        template <typename U>
        void operator*=(const vector2<U> vector);
        template <typename U>
        vector2<T> operator*(const vector2<U> vector) const;

        template <typename U>
        auto dot(const vector2<U> &vector) const;

        // Length
        float length() const;
        float squareLength() const;

        // Normalize
        void normalize();

        // Unit
        vector2<T> unit() const;

        // Logical compare
        template <typename U>
        bool operator==(const vector2<U>& other) const;
        template <typename U>
        bool operator!=(const vector2<U>& other) const;
        template <typename U>
        bool operator<(const vector2<U>& other) const;
        template <typename U>
        bool operator>(const vector2<U>& other) const;
        template <typename U>
        bool operator<=(const vector2<U>& other) const;
        template <typename U>
        bool operator>=(const vector2<U>& other) const;

        // Helpers
        void clear();
        void invert();

        // Min max
        T minComponent();
        T maxComponent();

        std::string toString() const;
    };

    // Right multiply scalar
    template <typename T>
    vector2<T> operator*(T value, const vector2<T>& vec);

    // Normalize
    template <typename T>
    inline vector2<T> normalize(const vector2<T>& v);

    // Dot
    template <typename T>
    inline auto dot(const vector2<T>& v1, const vector2<T>& v2);

    // Length
    template <typename T>
    inline float length(const vector2<T>& v);

    // <<
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const vector2<T>& v);

    //----------------------------------------//
    //---------------- Vector ----------------//
    //----------------------------------------//
    // TODO implement with array (vector2, vector3, vector4 can be a template specialization)
    template <typename T>
    class vector
    {
    public:
        std::vector<T> data;
        size_t n;

        vector();
        vector(size_t _n);
        vector(size_t _n, T val);
        vector(std::initializer_list<T> l);
        ~vector();

        T& operator[](size_t i);
        T at(size_t i) const;

        template <typename U>
        void operator=(std::vector<U> d);

        // Basic operations
        // +
        template <typename U>
        vector<T> operator+(const vector<U>& v) const;
        template <typename U>
        vector<T> operator+(const U v) const;
        template <typename U>
        void operator+=(const vector<U>& v);
        template <typename U>
        void operator+=(const U v);
        // -
        template <typename U>
        vector<T> operator-(const vector<U>& v) const;
        template <typename U>
        vector<T> operator-(const U v) const;
        template <typename U>
        void operator-=(const vector<U>& v);
        template <typename U>
        void operator-=(const U v);
        // -
        vector<T>& operator-();
        // *
        template <typename U>
        vector<T> operator*(const vector<U>& v) const;
        template <typename U>
        vector<T> operator*(const U v) const;
        template <typename U>
        void operator*=(const vector<U>& v);
        template <typename U>
        void operator*=(const U v);
        // /
        template <typename U>
        vector<T> operator/(const vector<U>& v) const;
        template <typename U>
        vector<T> operator/(const U v) const;
        template <typename U>
        void operator/=(const vector<U>& v);
        template <typename U>
        void operator/=(const U v);

        void normalize();
        T length();

        std::string toString() const;

        // Dot
        template <typename U, typename V>
        friend auto dot(const vector<U> &v1, const vector<V> &v2);
    };

    // <<
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const vector<T>& v);

    using vec2 = vector2<float>;
    using vec2f = vector2<float>;
    using vec2d = vector2<double>;
    using vec2i = vector2<int>;
    using vec2u = vector2<int>;

    using vec3 = vector3<float>;
    using vec3f = vector3<float>;
    using vec3d = vector3<double>;
    using vec3i = vector3<int>;
    using vec3u = vector3<unsigned>;

    using vec4 = vector4<float>;
    using vec4f = vector4<float>;
    using vec4d = vector4<double>;
    using vec4i = vector4<int>;
    using vec4u = vector4<unsigned>;

    using vec = vector<float>;
    using vecf = vector<float>;
    using vecd = vector<double>;
    using veci = vector<int>;
}

namespace std {
    template<> struct hash<atta::vec3> {
        size_t operator()(atta::vec3 const& vec) const {
            return ((hash<float>()(vec.x) ^
                        (hash<float>()(vec.y) << 1)) >> 1) ^
                (hash<float>()(vec.z) << 1);
        }
    };

    template<> struct hash<atta::vec2> {
        size_t operator()(atta::vec2 const& vec) const {
            return ((hash<float>()(vec.x) ^
                        (hash<float>()(vec.y) << 1)) >> 1);
        }
    };
}

#include <atta/core/math/vector4.inl>
#include <atta/core/math/vector3.inl>
#include <atta/core/math/vector2.inl>
#include <atta/core/math/vector.inl>
#endif// ATTA_CORE_MATH_VECTOR_H
