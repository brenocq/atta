//--------------------------------------------------
// Atta Component System
// typedComponentRegistry.h
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_TYPED_COMPONENT_REGISTRY_H
#define ATTA_COMPONENT_SYSTEM_TYPED_COMPONENT_REGISTRY_H
#include <atta/componentSystem/base.h>
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>

namespace atta
{
#define ATTA_REGISTER_COMPONENT(TYPE) \
    template<> \
    inline ::atta::TypedComponentRegistry<TYPE>& \
    ::atta::ComponentRegistration<TYPE>::reg = \
        ::atta::TypedComponentRegistry<TYPE>::getInstance();

    template <typename T>
    class TypedComponentRegistry : public ComponentRegistry
    {
    public:
        struct Description;

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

        ComponentDescription& getDescription() override;
        static ComponentDescription* description;
    private:
        TypedComponentRegistry<T>();

        void renderUIImpl(T* component);
        void serializeImpl(std::ostream& os, T* component);
        void deserializeImpl(std::istream& is, T* component);
    };

    //---------- Default component register description ----------//
    template<typename T>
    ComponentDescription& TypedComponentRegistry<T>::getDescription()
    {
        static ComponentDescription desc = 
        {
            typeid(T).name(),
            {
                { AttributeType::CUSTOM, 0, "custom" },
            }
        };

        return desc;
    }

    template<typename T>
    ComponentDescription* TypedComponentRegistry<T>::description = nullptr;


    template<typename T>
    class ComponentRegistration
    {
        static ::atta::TypedComponentRegistry<T>& reg;
    };
}

#include <atta/componentSystem/typedComponentRegistry.inl>
#endif// ATTA_COMPONENT_SYSTEM_TYPED_COMPONENT_REGISTRY_H
