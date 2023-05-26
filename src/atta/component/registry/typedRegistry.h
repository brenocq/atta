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
#define ATTA_REGISTER_COMPONENT(TYPE) \
    template <> \
    inline ::atta::component::TypedRegistry<TYPE>& ::atta::component::ComponentRegistration<TYPE>::reg = \
        ::atta::component::TypedRegistry<TYPE>::getInstance();
// clang-format on

template <typename T>
class TypedRegistry : public Registry {
  public:
    struct Description;

    static TypedRegistry<T>& getInstance() {
        static TypedRegistry<T> instance;
        return instance;
    }

    void renderUI(Component* component) override { renderUIImpl((T*)component); }
    static void renderUI(T* component) { getInstance().renderUIImpl(component); }

    void serialize(std::ostream& os, Component* component) override { serializeImpl(os, (T*)component); }
    void deserialize(std::istream& is, Component* component) override { deserializeImpl(is, (T*)component); }
    static void serialize(std::ostream& os, T* component) { getInstance().serializeImpl(os, component); }
    static void deserialize(std::istream& is, T* component) { getInstance().deserializeImpl(is, component); }

    std::vector<uint8_t> getDefault() override;

    ComponentDescription& getDescription() override;
    static ComponentDescription* description;

  private:
    TypedRegistry<T>();

    void renderUIImpl(T* component);
    void serializeImpl(std::ostream& os, T* component);
    void deserializeImpl(std::istream& is, T* component);
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
class ComponentRegistration {
    static ::atta::component::TypedRegistry<T>& reg;
};

} // namespace atta::component

#include <atta/component/registry/typedRegistry.inl>

#endif // ATTA_COMPONENT_REGISTRY_TYPED_REGISTRY_H
