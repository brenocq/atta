//--------------------------------------------------
// Atta Component System
// component.inl
// Date: 2021-11-04
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
#define ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(atribType)\
    template<> void Component::renderUIAttribute<Component::AttributeType::atribType> (Component::AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)

    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(CHAR);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(INT8);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(INT16);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(INT32);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(INT64);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(UINT8);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(UINT16);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(UINT32);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(UINT64);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(FLOAT32);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(FLOAT64);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(QUAT);
    ATTA_COMPONENT_RENDER_UI_ATTRIBUTE(STRINGID);

#define ATTA_TYPED_COMPONENT_RENDER_UI_CASE(ComponentType) \
                case AttributeType::ComponentType:\
                case AttributeType::VECTOR_##ComponentType:\
                    Component::renderUIAttribute<AttributeType::ComponentType>(aDesc, data, size, imguiId+aDesc.name);\
                    break;\

    template<typename T>
    void TypedComponent<T>::renderUI()
    {
        //DASSERT(this != nullptr, "Trying to call TypedComponent<$0>::renderUI() on nullptr component", std::string(typeid(T).name()));

        T* t = (T*)this;
        const TypeDescription typeDescription = T::typeDescription;
        const std::vector<AttributeDescription> attributeDescriptions = typeDescription.attributeDescriptions;

        //LOG_DEBUG("TypedComponent", "[w]Component<$0> $1[] has size $2", typeid(T).name(), typeDescription.type, sizeof(T));

        for(unsigned i = 0; i < attributeDescriptions.size(); i++)
        {
            AttributeDescription aDesc = attributeDescriptions[i];

            void* data = (void*)((uint8_t*)t+aDesc.offset);
            unsigned size = (i == attributeDescriptions.size()-1) ? 
                sizeof(T)-aDesc.offset : 
                attributeDescriptions[i+1].offset - aDesc.offset;

            //LOG_DEBUG("TypedComponent", "  - Attribute [w]$2[] has offset $0 and size $1", aDesc.offset, size, aDesc.name);

            const std::string imguiId = "##"+typeDescription.type;
            switch(aDesc.type)
            {
                case AttributeType::CHAR:
                    Component::renderUIAttribute<AttributeType::CHAR>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(INT8)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(INT16)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(INT32)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(INT64)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(UINT8)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(UINT16)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(UINT32)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(UINT64)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(FLOAT32)
                ATTA_TYPED_COMPONENT_RENDER_UI_CASE(FLOAT64)
                case AttributeType::QUAT:
                    Component::renderUIAttribute<AttributeType::QUAT>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                case AttributeType::STRINGID:
                    Component::renderUIAttribute<AttributeType::STRINGID>(aDesc, data, size, imguiId+aDesc.name);
                    break;
                default:
                    break;
            }
        }
    }

    template<typename T>
    std::vector<uint8_t> TypedComponent<T>::serialize()
    {
        return {};
    }

    template<typename T>
    void TypedComponent<T>::deserialize(std::vector<uint8_t> serialized)
    {

    }
}
