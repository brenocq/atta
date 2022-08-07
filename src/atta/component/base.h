//--------------------------------------------------
// Atta Component Module
// base.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_BASE_H
#define ATTA_COMPONENT_BASE_H
#include <atta/core/stringId.h>

namespace atta::component {
#define COMPONENT_POOL_SID(T) SID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SSID(T) SSID((std::string("Component_") + typeid(T).name() + "Allocator").c_str())
#define COMPONENT_POOL_SID_BY_NAME(typeidTname) SID((std::string("Component_") + typeidTname + "Allocator").c_str())
#define COMPONENT_POOL_SSID_BY_NAME(typeidTname) SSID((std::string("Component_") + typeidTname + "Allocator").c_str())

using EntityId = int;           // Index inside entity pool
using ComponentId = StringHash; // Component allocator name hash (COMPONENT_POOL_SID(T) result)

enum class AttributeType {
    // Base types
    BOOL = 0,
    CHAR,
    INT8,
    INT16,
    INT32,
    INT64,
    UINT8,
    UINT16,
    UINT32,
    UINT64,
    FLOAT32,
    FLOAT64,

    // Atta types
    VECTOR_BOOL = 100,
    VECTOR_CHAR,    // atta::vector<char>
    VECTOR_INT8,    // atta::vector<int8_t>
    VECTOR_INT16,   // atta::vector<int16_t>
    VECTOR_INT32,   // atta::vector<int>
    VECTOR_INT64,   // atta::vector<long>
    VECTOR_UINT8,   // atta::vector<uint8_t>
    VECTOR_UINT16,  // atta::vector<uint16_t>
    VECTOR_UINT32,  // atta::vector<unsigned>
    VECTOR_UINT64,  // atta::vector<uint64_t>
    VECTOR_FLOAT32, // atta::vector<float>
    VECTOR_FLOAT64, // atta::vector<double>

    MATRIX_BOOL = 200,
    MATRIX_CHAR,    // atta::matrix<char>
    MATRIX_INT8,    // atta::matrix<int8_t>
    MATRIX_INT16,   // atta::matrix<int16_t>
    MATRIX_INT32,   // atta::matrix<int>
    MATRIX_INT64,   // atta::matrix<long>
    MATRIX_UINT8,   // atta::matrix<uint8_t>
    MATRIX_UINT16,  // atta::matrix<uint16_t>
    MATRIX_UINT32,  // atta::matrix<unsigned>
    MATRIX_UINT64,  // atta::matrix<uint64_t>
    MATRIX_FLOAT32, // atta::matrix<float>
    MATRIX_FLOAT64, // atta::matrix<double>

    QUAT = 300,
    RAY,

    STRINGID = 400,
    // Custom types
    CUSTOM
};

struct AttributeDescription {
    AttributeType type;
    unsigned offset; // Offset in bytes from the beginning of the component to the attribute (can use the offsetof() macro)
    std::string name;
    std::any min;
    std::any max;
    float step;
    std::vector<std::any> options;
};

// FIXME Sometimes crashing when trying to delete the description
struct ComponentDescription {
    std::string name;
    std::vector<AttributeDescription> attributeDescriptions;
    unsigned maxInstances = 1024; // Maximum number of component instances
    std::map<std::string, std::function<void(std::ostream& os, void* data)>> serialize;
    std::map<std::string, std::function<void(std::istream& is, void* data)>> deserialize;
    std::map<std::string, std::function<void(void* data, std::string imguiId)>> renderUI;
};
} // namespace atta::component

#endif // ATTA_COMPONENT_BASE_H
