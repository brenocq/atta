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
#include <atta/core/stringId.h>

namespace atta
{
#define ATTA_REGISTER_COMPONENT(TYPE) \
    template<> \
    inline ::atta::TypedComponentRegistry<TYPE>& \
    ::atta::ComponentRegistration<TYPE>::reg = \
        ::atta::TypedComponentRegistry<TYPE>::getInstance();

    class ComponentManager;
    class ComponentRegistry
    {
    public:
        using Type = StringId;
        ComponentRegistry(unsigned sizeofT, std::string typeidName, size_t typeidHash):
            _sizeof(sizeofT), _typeidName(typeidName), _typeidHash(typeidHash), _index(0), _poolCreated(false)
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
        
        // FIXME Sometimes crashing when trying to delete the description
        struct Description
        {
            std::string name;
            std::vector<AttributeDescription> attributeDescriptions;
            unsigned maxInstances = 1024;// Maximum number of component instances
            std::map<std::string, std::function<void(std::ostream& os, void* data)>> serialize;
            std::map<std::string, std::function<void(std::istream& is, void* data)>> deserialize;
        };

        virtual void renderUI(Component* component) = 0;

        virtual void serialize(std::ostream& os, Component* component) = 0;
        virtual void deserialize(std::istream& is, Component* component) = 0;
        unsigned getSerializedSize(Component* component);
        
        virtual Description& getDescription() = 0;
        virtual std::vector<uint8_t> getDefault() = 0;
        unsigned getSizeof() const { return _sizeof; }
        std::string getTypeidName() const { return _typeidName; }
        size_t getTypeidHash() const { return _typeidHash; }
        ComponentId getId() const { return COMPONENT_POOL_SSID_BY_NAME(_typeidName); }
        unsigned getIndex() const { return _index; }
        bool getPoolCreated() const { return _poolCreated; }
        void setPoolCreated(bool poolCreated) { _poolCreated = poolCreated; }

    protected:
        void registerToComponentManager();

        template<ComponentRegistry::AttributeType attributeType>
        void renderUIAttribute(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId) {}

        unsigned _sizeof;// sizeof(T)
        std::string _typeidName;// typeid(T).name()
        size_t _typeidHash;// typeid(T).hash_code()

        // Component index starting from 0
        // This index is useful to access the entity component without iterating over the entity block
        unsigned _index;
        bool _poolCreated;

    private:
        void setIndex(unsigned index) { _index = index; }
        friend ComponentManager;
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
        static void renderUI(T* component) { getInstance().renderUIImpl(component); }

        void serialize(std::ostream& os, Component* component) override { serializeImpl(os, (T*)component); }
        void deserialize(std::istream& is, Component* component) override { deserializeImpl(is, (T*)component); }
        static void serialize(std::ostream& os, T* component) { getInstance().serializeImpl(os, component); }
        static void deserialize(std::istream& is, T* component) { getInstance().deserializeImpl(is, component); }

        std::vector<uint8_t> getDefault() override;

        Description& getDescription() override;
        static Description* description;
    private:
        TypedComponentRegistry<T>();

        void renderUIImpl(T* component);
        void serializeImpl(std::ostream& os, T* component);
        void deserializeImpl(std::istream& is, T* component);
    };

    template<typename T>
    class ComponentRegistration
    {
        static ::atta::TypedComponentRegistry<T>& reg;
    };

    //---------- Default component register description ----------//
    template<typename T>
    ComponentRegistry::Description& TypedComponentRegistry<T>::getDescription()
    {
        static ComponentRegistry::Description desc = 
        {
            typeid(T).name(),
            {
                { ComponentRegistry::AttributeType::CUSTOM, 0, "custom" },
            }
        };

        return desc;
    }

    template<typename T>
    ComponentRegistry::Description* TypedComponentRegistry<T>::description = nullptr;

    //---------- Attribute helpers ----------//
    inline std::ostream& operator<<(std::ostream& os, ComponentRegistry& c)
    {
        return os << c.getDescription().name;
    }
}

#include <atta/componentSystem/componentRegistry.inl>
#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_REGISTRY_H
