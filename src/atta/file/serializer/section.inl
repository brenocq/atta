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
        return "vec2(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ")";
    }
    // Handle vector3
    else if constexpr (atta::file::is_vector3_v<U>) {
        return "vec3(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ")";
    }
    // Handle vec4
    else if constexpr (atta::file::is_vector4_v<U>) {
        return "vec4(" + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " + std::to_string(value.w) +
               ")";
    }
    // Handle quat
    else if constexpr (std::is_same_v<U, atta::quat>) {
        return "quat(" + std::to_string(value.i) + ", " + std::to_string(value.j) + ", " + std::to_string(value.k) + ", " + std::to_string(value.r) +
               ")";
    }
    // Handle strings
    else if constexpr (std::is_convertible_v<U, std::string>) {
        return "\"" + std::string(value) + "\"";
    }
    // Handle convertible to bool (bool reference in std::vector<bool>)
    else if constexpr (std::is_convertible_v<U, bool>) {
        return static_cast<bool>(value) ? "true" : "false";
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

template <typename T>
SectionData::operator T() const {
    using U = std::decay_t<T>;
    if (_str.empty()) {
        LOG_WARN("SectionData", "Trying to cast empty SectionData to type [w]$0[]", typeid(U).name());
        return U();
    }

    // Handle std::vector
    if constexpr (is_specialization<U, std::vector>::value) {
        if (_str.front() != '{' || _str.back() != '}') {
            LOG_WARN("SectionData", "Invalid vector format in SectionData when casting to [w]std::vector[]: [w]$0", _str);
            return U();
        }

        // Parse the vector
        using ValueType = typename U::value_type;
        U result;
        std::string element;
        int nestedLevel = 0;
        for (char ch : _str) {
            if ((ch == ',' || ch == '}') && nestedLevel == 1) {
                // Comma at the top level: finalize the current element

                // Remove whitespaces from the start/end of the element
                element.erase(0, element.find_first_not_of(" \t"));
                element.erase(element.find_last_not_of(" \t") + 1);

                // Recursively convert each element to ValueType
                SectionData temp;
                temp._str = std::move(element);
                result.push_back(static_cast<ValueType>(temp));
                element.clear();
            } else {
                // Adjust nested level for parentheses
                if (ch == '(' || ch == '{')
                    nestedLevel++;
                if (ch == ')' || ch == '}')
                    nestedLevel--;

                // Skip initial "{"
                if (nestedLevel == 1 && ch == '{')
                    continue;

                // Add the character to the current element
                element += ch;
            }
        }

        return result;
    }
    // Handle booleans
    else if constexpr (std::is_same_v<U, bool>) {
        return _str == "true" ? true : false;
    }
    // Handle arithmetic types (int, float, double, etc.)
    else if constexpr (std::is_arithmetic_v<U>) {
        std::istringstream iss(_str);
        U value{};
        if (iss >> value) {
            return value;
        }
        LOG_WARN("SectionData", "Failed to cast SectionData to arithmetic type");
        return U();
    }
    // Handle vector2
    else if constexpr (is_vector2<U>::value) {
        if (_str.substr(0, 4) != "vec2") {
            LOG_WARN("SectionData", "Invalid format for vec2: [w]$0", _str);
            return U();
        }
        std::istringstream iss(_str.substr(4)); // Skip the "vec2" prefix
        char discard;
        U result{};
        if (iss >> discard && discard == '(' && iss >> std::ws >> result.x >> std::ws >> discard && discard == ',' &&
            iss >> std::ws >> result.y >> std::ws >> discard && discard == ')') {
            return result;
        }
        LOG_WARN("SectionData", "Failed to cast SectionData to vec2: [w]$0", _str);
        return U();
    }
    // Handle vector3
    else if constexpr (is_vector3<U>::value) {
        if (_str.substr(0, 4) != "vec3") {
            LOG_WARN("SectionData", "Invalid format for vec3: [w]$0", _str);
            return U();
        }
        std::istringstream iss(_str.substr(4)); // Skip the "vec3" prefix
        char discard;
        U result{};
        if (iss >> discard && discard == '(' && iss >> result.x && iss >> discard && discard == ',' && iss >> result.y && iss >> discard &&
            discard == ',' && iss >> result.z && iss >> discard && discard == ')') {
            return result;
        }
        LOG_WARN("SectionData", "Failed to cast SectionData to vec3");
        return U();
    }
    // Handle vector4
    else if constexpr (is_vector4<U>::value) {
        if (_str.substr(0, 4) != "vec4") {
            LOG_WARN("SectionData", "Invalid format for vec4: [w]$0", _str);
            return U();
        }
        std::istringstream iss(_str.substr(4)); // Skip the "vec4" prefix
        char discard;
        U result{};
        if (iss >> discard && discard == '(' && iss >> result.x && iss >> discard && discard == ',' && iss >> result.y && iss >> discard &&
            discard == ',' && iss >> result.z && iss >> discard && discard == ',' && iss >> result.w && iss >> discard && discard == ')') {
            return result;
        }
        LOG_WARN("SectionData", "Failed to cast SectionData to vec4");
        return U();
    }
    // Handle quaternion
    else if constexpr (std::is_same_v<U, atta::quat>) {
        if (_str.substr(0, 4) != "quat") {
            LOG_WARN("SectionData", "Invalid format for quat: [w]$0", _str);
            return U();
        }
        std::istringstream iss(_str.substr(4)); // Skip the "quat" prefix
        char discard;
        U result{};
        if (iss >> discard && discard == '(' && iss >> result.i && iss >> discard && discard == ',' && iss >> result.j && iss >> discard &&
            discard == ',' && iss >> result.k && iss >> discard && discard == ',' && iss >> result.r && iss >> discard && discard == ')') {
            return result;
        }
        LOG_WARN("SectionData", "Failed to cast SectionData to quat");
        return U();
    }
    // Handle strings
    else if constexpr (std::is_convertible_v<U, std::string>) {
        if (_str.front() == '"' && _str.back() == '"')
            return _str.substr(1, _str.size() - 2);
        return U();
    }
    // Handle unsupported types
    else {
        LOG_WARN("SectionData", "Casting SectionData to type [w]$0[] is not supported", typeid(U).name());
        return U();
    }
}

} // namespace atta::file
