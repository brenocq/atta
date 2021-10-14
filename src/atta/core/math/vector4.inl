//--------------------------------------------------
// Atta Math
// vector4.inl
// Date: 2021-08-27
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    //----------------------------------------//
    //---------------- vector4 ---------------//
    //----------------------------------------//
    template <typename T>
    T vector4<T>::operator[](unsigned i) const
    {
        if(i == 0) return x;
        if(i == 1) return y;
        if(i == 2) return z;
        return w;
    }

    // Add vector
    template <typename T>
    template <typename U>
    void vector4<T>::operator+=(const vector4<U>& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        w += v.w;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator+(const vector4<U>& v) const
    {
        return vector4<T>(x+v.x, y+v.y, z+v.z, w+v.w);
    }

    // Add scalar
    template <typename T>
    template <typename U>
    void vector4<T>::operator+=(const U value)
    {
        x += value;
        y += value;
        z += value;
        w += value;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator+(const U value) const
    {
        return vector4<T>(x+value, y+value, z+value, w+value);
    }

    // Subtract vector
    template <typename T>
    template <typename U>
    void vector4<T>::operator-=(const vector4<U>& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        w -= v.w;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator-(const vector4<U>& v) const
    {
        return vector4<T>(x-v.x, y-v.y, z-v.z, w-v.w);
    }

    // Subtract scalar
    template <typename T>
    template <typename U>
    void vector4<T>::operator-=(const U value)
    {
        x -= value;
        y -= value;
        z -= value;
        w -= value;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator-(const U value) const
    {
        return vector4<T>(x-value, y-value, z-value, w-value);
    }
    template <typename T>
    vector4<T> vector4<T>::operator-() const
    {
        return vector4<T>(-x, -y, -z, -w);
    }

    // Multiply scalar
    template <typename T>
    template <typename U>
    void vector4<T>::operator*=(const U value)
    {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator*(const U value) const
    {
        return vector4<T>(x*value, y*value, z*value, w*value);
    }

    // Divide scalar
    template <typename T>
    template <typename U>
    void vector4<T>::operator/=(const U value)
    {
        x /= value;
        y /= value;
        z /= value;
        w /= value;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator/(const U value) const
    {
        return vector4<T>(x/value, y/value, z/value, w/value);
    }

    // Divide vector
    template <typename T>
    template <typename U>
    void vector4<T>::operator/=(const vector4<U>& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
        w /= v.w;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator/(const vector4<U>& v) const
    {
        return vector4<T>(x/v.x, y/v.y, z/v.z, w/v.w);
    }

    // Multiply vector
    template <typename T>
    template <typename U>
    void vector4<T>::operator*=(const vector4<U> vector)
    {
        x *= vector.x;
        y *= vector.y;
        z *= vector.z;
        w *= vector.w;
    }
    template <typename T>
    template <typename U>
    vector4<T> vector4<T>::operator*(const vector4<U> vector) const
    {
        return vector4<T>(x*vector.x, y*vector.y, z*vector.z, w*vector.w);
    }

    // Dot
    template <typename T>
    template <typename U>
    auto vector4<T>::dot(const vector4<U> &vec) const
    {
        return x*vec.x + y*vec.y + z*vec.z + w*vec.w;
    }

    // Length
    template <typename T>
    float vector4<T>::length() const
    {
        return sqrt(squareLength());
    }

    template <typename T>
    float vector4<T>::squareLength() const
    {
        return x*x + y*y + z*z + w*w;
    }

    // Normalize
    template <typename T>
    void vector4<T>::normalize()
    {
        float l = length();
        if(l > 0)
            (*this) *= 1.0f/l;
    }

    // Unit
    template <typename T>
    vector4<T> vector4<T>::unit() const
    {
        vector4<T> result = *this;
        result.normalize();
        return result;
    }

    // Logical operators
    template <typename T>
    template <typename U>
    bool vector4<T>::operator==(const vector4<U>& other) const
    {
        return x == other.x &&
            y == other.y &&
            z == other.z &&
            w == other.w;
    }
    template <typename T>
    template <typename U>
    bool vector4<T>::operator!=(const vector4<U>& other) const
    {
        return !(*this == other);
    }
    template <typename T>
    template <typename U>
    bool vector4<T>::operator<(const vector4<U>& other) const
    {
        return x < other.x && y < other.y && z < other.z && w < other.w;
    }
    template <typename T>
    template <typename U>
    bool vector4<T>::operator>(const vector4<U>& other) const
    {
        return x > other.x && y > other.y && z > other.z && w > other.w;
    }
    template <typename T>
    template <typename U>
    bool vector4<T>::operator<=(const vector4<U>& other) const
    {
        return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
    }
    template <typename T>
    template <typename U>
    bool vector4<T>::operator>=(const vector4<U>& other) const
    {
        return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
    }

    // Helpers
    template <typename T>
    void vector4<T>::clear()
    {
        x = y = z = w = 0;
    }
    template <typename T>
    void vector4<T>::invert()
    {
        x = -x;
        y = -y;
        z = -z;
        w = -w;
    }
    template <typename T>
    T vector4<T>::maxComponent()
    {
        return std::max(x, std::max(y, std::max(z, w)));
    }
    template <typename T>
    T vector4<T>::minComponent()
    {
        return std::min(x, std::min(y, std::min(z, w)));
    }
    template <typename T>
    std::string vector4<T>::toString() const
    {
        return "vec4{"+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+", "+std::to_string(w)+"}";
    }

    //---------- External ----------//
    // Right multiply scalar
    template <typename T>
    vector4<T> operator*(T value, vector4<T> const& vec)
    {
        return vec*value;
    }

    // Normalize
    template <typename T>
    inline vector4<T> normalize(const vector4<T>& v)
    {
        float l = v.length();
        if(l > 0)
            return v * 1.0f/l;
        return v;
    }

    // Dot
    template <typename T>
    inline auto dot(const vector4<T>& v1, const vector4<T>& v2)
    {
        return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z + v1.w*v2.w;
    }

    // Length
    template <typename T>
    inline float length(const vector4<T>& v)
    {
        return v.length();
    }

    // <<
    template <typename T>
    inline std::ostream& operator<<(std::ostream& os, const vector4<T>& v)
    {
        return os << v.toString();
    }
}
