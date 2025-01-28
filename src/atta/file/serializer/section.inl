//--------------------------------------------------
// Atta File Module
// section.inl
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::file {

template <typename Test, template <typename...> class Ref>
struct is_specialization : std::false_type {};
template <template <typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

//----- vec2 check -----//
// Primary template (false case)
template <typename T>
struct is_vector2 : std::false_type {};
// Specialization for vector2
template <typename T>
struct is_vector2<vector2<T>> : std::true_type {};
// Helper variable template for convenience (C++14 and later)
template <typename T>
inline constexpr bool is_vector2_v = is_vector2<T>::value;

//----- vec3 check -----//
// Primary template (false case)
template <typename T>
struct is_vector3 : std::false_type {};
// Specialization for vector3
template <typename T>
struct is_vector3<vector3<T>> : std::true_type {};
// Helper variable template for convenience (C++14 and later)
template <typename T>
inline constexpr bool is_vector3_v = is_vector3<T>::value;

//----- vec4 check -----//
// Primary template (false case)
template <typename T>
struct is_vector4 : std::false_type {};
// Specialization for vector4
template <typename T>
struct is_vector4<vector4<T>> : std::true_type {};
// Helper variable template for convenience (C++14 and later)
template <typename T>
inline constexpr bool is_vector4_v = is_vector4<T>::value;

//---------- Type conversions ----------//
template <typename T>
std::string toString(const T& value) {
    using U = std::decay_t<T>;

    // Handle std::vector
    if constexpr (is_specialization<U, std::vector>::value) {
        std::string result = "{";
        for (size_t i = 0; i < value.size(); i++) {
            result += toString(value[i]); // Recursively call for vector elements
            if (i != value.size() - 1) {
                result += ", ";
            }
        }
        result += "}";
        return result;
    }
    // Handle std::initializer_list
    else if constexpr (is_specialization<U, std::initializer_list>::value) {
        std::string result = "{";
        size_t i = 0;
        for (const auto& elem : value) {
            result += toString(elem);
            if (++i != value.size()) {
                result += ", ";
            }
        }
        result += "}";
        return result;
    }
    // Handle booleans
    else if constexpr (std::is_same_v<U, bool>) {
        return value ? "true" : "false";
    }
    // Handle arithmetic types
    else if constexpr (std::is_arithmetic_v<U>) {
        return std::to_string(value);
    }
    // Handle vector2
    else if constexpr (atta::file::is_vector2_v<U>) {
        return "vector2(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ")";
    }
    // Handle vector3
    else if constexpr (atta::file::is_vector3_v<U>) {
        return "vector3(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ")";
    }
    // Handle vector4
    else if constexpr (atta::file::is_vector4_v<U>) {
        return "vector4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " +
               std::to_string(value.w) + ")";
    }
    // Handle quat
    else if constexpr (std::is_same_v<U, atta::quat>) {
        return "quaternion(" + std::to_string(value.i) + ", " + std::to_string(value.j) + ", " + std::to_string(value.k) + ", " +
               std::to_string(value.r) + ")";
    }
    // Handle strings
    else if constexpr (std::is_convertible_v<U, std::string>) {
        return "\"" + std::string(value) + "\"";
    }
    // Unsupported type
    else {
        LOG_WARN("SectionData", "Serialization of type [w]$0[] is not supported", typeid(U).name());
        return {};
    }
    return {};
}

//---------- SectionData ----------//
template <typename T>
void SectionData::operator=(T&& value) {
    _str = atta::file::toString(value);
}

template <typename T>
void SectionData::operator=(std::initializer_list<T> value) {
    _str = atta::file::toString(value);
}

} // namespace atta::file
