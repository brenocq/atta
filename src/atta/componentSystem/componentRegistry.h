//--------------------------------------------------
// Atta Component System
// componentRegistry.h
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENT_REGISTRY_H
#define ATTA_COMPONENT_SYSTEM_COMPONENT_REGISTRY_H
#include <atta/componentSystem/base.h>
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/core/stringId.h>

namespace atta
{
#define ATTA_REGISTER_COMPONENT(TYPE) \
    template<> \
    inline const ::atta::TypedComponentRegistry<TYPE>& \
    ::atta::ComponentRegistration<TYPE>::reg = \
        ::atta::TypedComponentRegistry<TYPE>::getInstance();

    class ComponentRegistry
    {
    public:
        using Type = StringId;
        ComponentRegistry(unsigned sizeofT, std::string typeidName, size_t typeidHash):
            _sizeof(sizeofT), _typeidName(typeidName), _typeidHash(typeidHash)
        {
        }

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
        
        struct Description
        {
            std::string type;
            std::vector<AttributeDescription> attributeDescriptions;
            unsigned maxInstances = 1024;// Maximum number of component instances
            unsigned maxSerializedSize = 1024*1024;// Maximum number of bytes when the component is serialized
        };

        virtual void renderUI(Component* component) = 0;
        virtual void serialize(std::ostream& os, Component* component) = 0;
        virtual void deserialize(std::istream& is, Component* component) = 0;
        virtual std::vector<uint8_t> getDefault() = 0;

        virtual Description getDescription() const = 0;
        unsigned getSerializedSize(Component* component);
        unsigned getSizeof() { return _sizeof; }
        std::string getTypeidName() { return _typeidName; }
        size_t getTypeidHash() { return _typeidHash; }
        ComponentId getId() { return COMPONENT_POOL_SSID_BY_NAME(_typeidName); }

    protected:
        template<ComponentRegistry::AttributeType attributeType>
        void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}

        unsigned _sizeof;// sizeof(T)
        std::string _typeidName;// typeid(T).name()
        size_t _typeidHash;// typeid(T).hash_code()
    };

    template <typename T>
    class TypedComponentRegistry : public ComponentRegistry
    {
    public:
        static TypedComponentRegistry<T>& getInstance()
        {
            static TypedComponentRegistry<T> instance;
            return instance;
        }

        void renderUI(Component* component) override { renderUIImpl((T*)component); }
        void serialize(std::ostream& os, Component* component) override { serializeImpl(os, (T*)component); }
        void deserialize(std::istream& is, Component* component) override { deserializeImpl(is, (T*)component); }
        std::vector<uint8_t> getDefault() override;
        Description getDescription() const override { return description; }

        static void renderUI(T* component) { getInstance().renderUIImpl(component); }
        static void serialize(std::ostream& os, T* component) { getInstance().serializeImpl(os, component); }
        static void deserialize(std::istream& is, T* component) { getInstance().deserializeImpl(is, component); }

        static const ComponentRegistry::Description description;
    private:
        TypedComponentRegistry<T>();

        void renderUIImpl(T* component);
        void serializeImpl(std::ostream& os, T* component);
        void deserializeImpl(std::istream& is, T* component);
    };

    template<typename T>
    class ComponentRegistration
    {
        static const ::atta::TypedComponentRegistry<T>& reg;
    };

    //---------- Default component register description ----------//
    template<typename T>
    inline const ComponentRegistry::Description TypedComponentRegistry<T>::description = 
    {
        typeid(T).name(),
        {
            { ComponentRegistry::AttributeType::CUSTOM, 0, "custom" },
        }
    };

    //---------- Attribute helpers ----------//
    inline std::ostream& operator<<(std::ostream& os, const ComponentRegistry& c)
    {
        return os << c.getDescription().type;
    }
}

#include <atta/componentSystem/componentRegistry.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_REGISTRY_H
