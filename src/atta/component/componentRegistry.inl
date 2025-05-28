//--------------------------------------------------
// Atta Component Module
// componentRegistry.inl
// Date: 2021-11-05
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::component {
#define ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(atribType)                                                                                       \
    template <>                                                                                                                                      \
    void ComponentRegistry::renderUIAttribute<AttributeType::atribType>(AttributeDescription aDesc, void* d, unsigned size, std::string imguiId)

ATTA_COMPONENT_REGISTER_RENDER_UI_ATTRIBUTE(BOOL);
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

} // namespace atta::component
