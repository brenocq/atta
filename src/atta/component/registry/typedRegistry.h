//--------------------------------------------------
// Atta Component Module
// typedRegistry.h
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_REGISTRY_TYPED_REGISTRY_H
#define ATTA_COMPONENT_REGISTRY_TYPED_REGISTRY_H

#include <atta/component/base.h>
#include <atta/component/components/component.h>
#include <atta/component/registry/registry.h>

namespace atta::component {

// clang-format off
#define ATTA_REGISTER_COMPONENT(TYPE) inline ::atta::component::TypedRegistry<TYPE> typedRegistry##TYPE(#TYPE);
// clang-format on

template <typename T>
class TypedRegistry : public Registry {
  public:
    TypedRegistry<T>(std::string name);
    struct Description;

    void renderUI(Component* component) override;

    void serialize(std::ostream& os, Component* component) override;
    void deserialize(std::istream& is, Component* component) override;

    std::vector<uint8_t> getDefault() override;

    ComponentDescription& getDescription() override;
    static ComponentDescription* description;
    static ComponentId id;
};

//---------- Default component register description ----------//
template <typename T>
ComponentDescription& TypedRegistry<T>::getDescription() {
    static ComponentDescription desc = {typeid(T).name(),
                                        {
                                            {AttributeType::CUSTOM, 0, "custom"},
                                        }};

    return desc;
}

template <typename T>
ComponentDescription* TypedRegistry<T>::description = nullptr;

template <typename T>
ComponentId TypedRegistry<T>::id = 0;

} // namespace atta::component

#include <atta/component/registry/typedRegistry.inl>

#endif // ATTA_COMPONENT_REGISTRY_TYPED_REGISTRY_H
