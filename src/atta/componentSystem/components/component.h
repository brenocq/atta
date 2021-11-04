//--------------------------------------------------
// Atta Component System
// component.h
// Date: 2021-11-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_COMPONENT_H
#include <atta/core/stringId.h>

namespace atta
{
    // TODO class virtual methods are increasing the component size by 8 bytes. Refactor to not increase component size
    class Component
    {
    public:
        using Type = StringId;

        enum class AttributeType
        {
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
            VECTOR_CHAR,// atta::vector<char>
            VECTOR_INT8,// atta::vector<int8_t>
            VECTOR_INT16,// atta::vector<int16_t>
            VECTOR_INT32,// atta::vector<int>
            VECTOR_INT64,// atta::vector<long>
            VECTOR_UINT8,// atta::vector<uint8_t>
            VECTOR_UINT16,// atta::vector<uint16_t>
            VECTOR_UINT32,// atta::vector<unsigned>
            VECTOR_UINT64,// atta::vector<uint64_t>
            VECTOR_FLOAT32,// atta::vector<float>
            VECTOR_FLOAT64,// atta::vector<double>

            MATRIX_BOOL = 200,
            MATRIX_CHAR,// atta::matrix<char>
            MATRIX_INT8,// atta::matrix<int8_t>
            MATRIX_INT16,// atta::matrix<int16_t>
            MATRIX_INT32,// atta::matrix<int>
            MATRIX_INT64,// atta::matrix<long>
            MATRIX_UINT8,// atta::matrix<uint8_t>
            MATRIX_UINT16,// atta::matrix<uint16_t>
            MATRIX_UINT32,// atta::matrix<unsigned>
            MATRIX_UINT64,// atta::matrix<uint64_t>
            MATRIX_FLOAT32,// atta::matrix<float>
            MATRIX_FLOAT64,// atta::matrix<double>

            QUAT = 300,
            RAY,

            STRINGID = 400,
            // Custom types
            CUSTOM
        };

        struct AttributeDescription
        {
            AttributeType type;
            unsigned offset;// Offset in bytes from the beginning of the component to the attribute (can use the offsetof() macro)
            std::string name;
            std::any min;
            std::any max;
            float step;
            std::vector<std::any> options;
        };
        
        struct TypeDescription
        {
            std::string type;
            std::vector<AttributeDescription> attributeDescriptions;
        };

        virtual void renderUI() = 0;
        virtual std::vector<uint8_t> serialize() = 0;
        virtual void deserialize(std::vector<uint8_t> serialized) = 0;

        virtual TypeDescription getTypeDescription() const = 0;

    protected:
        template<Component::AttributeType attributeType>
        void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}
    };

    template <typename T>
    class TypedComponent : public Component
    {
    public:
        void renderUI() override;
        std::vector<uint8_t> serialize() override;
        void deserialize(std::vector<uint8_t> serialized) override;

        Component::TypeDescription getTypeDescription() const override { return TypedComponent<T>::typeDescription; };
        static const Component::TypeDescription typeDescription;
    };

    template<typename T>
    inline const Component::TypeDescription TypedComponent<T>::typeDescription = 
    {
        typeid(T).name(),
        {
            { Component::AttributeType::CUSTOM, 0, "custom" },
        }
    };

    //---------- Attribute helpers ----------//
    inline std::ostream& operator<<(std::ostream& os, const Component& c)
    {
        return os << c.getTypeDescription().type;
    }

    // Could be resolved at compile time if used constexpr constructor, may use it in the future
    template <typename T1, typename T2>
    inline unsigned attributeOffset(T1 T2::*member)
    {
        return offsetof(T2, member);
        //T2 object {};
        //return static_cast<unsigned>(size_t(&(object.*member)) - size_t(&object));
    }
}

#include <atta/componentSystem/components/component.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_COMPONENT_H
