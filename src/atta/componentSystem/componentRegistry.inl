//--------------------------------------------------
// Atta Component System
// componentRegistry.inl
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
#define ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(atribType)\
    template<> void ComponentRegistry::renderUIAttribute<ComponentRegistry::AttributeType::atribType> (ComponentRegistry::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)

    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(CHAR);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(INT8);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(INT16);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(INT32);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(INT64);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(UINT8);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(UINT16);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(UINT32);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(UINT64);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(FLOAT32);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(FLOAT64);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(QUAT);
    ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(STRINGID);

#define ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(ComponentType) \
                case AttributeType::ComponentType:\
                case AttributeType::VECTOR_##ComponentType:\
                    ComponentRegistry::renderUIAttribute<AttributeType::ComponentType>(aDesc, data, size, imguiId+aDesc.name);\
                    break;\

    template<typename T>
    TypedComponentRegistry<T>::TypedComponentRegistry():
        ComponentRegistry(sizeof(T), typeid(T).name(), typeid(T).hash_code())
    {
        //LOG_DEBUG("TypedComponentRegistry", "Created new registry for $0", typeid(T).name());
        ComponentManager::registerComponent(dynamic_cast<ComponentRegistry*>(this));
    }

    template<typename T>
    void TypedComponentRegistry<T>::renderUIImpl(T* component)
    {
        //DASSERT(this != nullptr, "Trying to call TypedComponentRegistry<$0>::renderUI() on nullptr component", std::string(typeid(T).name()));

        const std::vector<AttributeDescription> attributeDescriptions = description.attributeDescriptions;

        //LOG_DEBUG("TypedComponentRegistry", "[w]Component<$0> $1[] has size $2", typeid(T).name(), description.type, sizeof(T));

        for(unsigned i = 0; i < attributeDescriptions.size(); i++)
        {
            AttributeDescription aDesc = attributeDescriptions[i];

            void* data = (void*)((uint8_t*)component+aDesc.offset);
            unsigned size = (i == attributeDescriptions.size()-1) ? 
                sizeof(T)-aDesc.offset : 
                attributeDescriptions[i+1].offset - aDesc.offset;

            //LOG_DEBUG("TypedComponentRegistry", "  - Attribute [w]$2[] has offset $0 and size $1", aDesc.offset, size, aDesc.name);

            const std::string imguiId = "##"+description.type;
            switch(aDesc.type)
            {
                case AttributeType::CHAR:
                    ComponentRegistry::renderUIAttribute<AttributeType::CHAR>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(INT8)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(INT16)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(INT32)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(INT64)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(UINT8)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(UINT16)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(UINT32)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(UINT64)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(FLOAT32)
                ATTA_TYPED_COMPONENT_REGISTER_RENDER_UI_CASE(FLOAT64)
                case AttributeType::QUAT:
                    ComponentRegistry::renderUIAttribute<AttributeType::QUAT>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                case AttributeType::STRINGID:
                    ComponentRegistry::renderUIAttribute<AttributeType::STRINGID>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                default:
                    break;
            }
        }
    }

    template<typename T>
    void TypedComponentRegistry<T>::serializeImpl(std::ostream& os, T* component)
    {
        uint8_t* curr = reinterpret_cast<uint8_t*>(component);
        for(unsigned i = 0; i < description.attributeDescriptions.size(); i++)
        {
            auto aDesc = description.attributeDescriptions[i];
            unsigned size = (i == description.attributeDescriptions.size()-1)
                ? sizeof(T)-aDesc.offset 
                : description.attributeDescriptions[i+1].offset - aDesc.offset;
            switch(aDesc.type)
            {
                case AttributeType::STRINGID:
                {
                    StringId* sid = reinterpret_cast<StringId*>(curr);
                    std::string str = sid->getString();
                    os.write(str.c_str(), str.size());
                    os.put('\0');
                    break;
                }
                default:
                    os.write(reinterpret_cast<const char*>(curr), size);
            }
            curr += size;
        }
    }

    template<typename T>
    void TypedComponentRegistry<T>::deserializeImpl(std::istream& is, T* component)
    {
        char* curr = reinterpret_cast<char*>(component);
        for(unsigned i = 0; i < description.attributeDescriptions.size(); i++)
        {
            auto aDesc = description.attributeDescriptions[i];
            unsigned size = (i == description.attributeDescriptions.size()-1)
                ? sizeof(T)-aDesc.offset 
                : description.attributeDescriptions[i+1].offset - aDesc.offset;
            switch(aDesc.type)
            {
                case AttributeType::STRINGID:
                {
                    // Calculate string size
                    int init = is.tellg();
                    while(is.get()!='\0' && !is.eof());
                    int size = int(is.tellg())-init-1;

                    // Return to string first char
                    is.seekg(init);

                    // Read string
                    std::string str;
                    str.resize(size);
                    is.read(&str[0], size);
                    is.ignore();// jump \0
                    StringId sid(str);
                    // Save stringId to component
                    memcpy(curr, reinterpret_cast<char*>(&sid), sizeof(StringId));
                    break;
                }
                default:
                    is.read(reinterpret_cast<char*>(curr), size);
            }
            curr += size;
        }
    }

    template<typename T>
    std::vector<uint8_t> TypedComponentRegistry<T>::getDefault()
    {
        std::vector<uint8_t> defaultData(sizeof(T));
        T* newComponent = reinterpret_cast<T*>(defaultData.data());
        *newComponent = T{};
        return defaultData;
    }
}
