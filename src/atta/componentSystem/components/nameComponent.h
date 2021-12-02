//--------------------------------------------------
// Atta Component System
// nameComponent.h
// Date: 2021-09-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>

namespace atta
{
	/// %Component to give an entity a name
    struct NameComponent final : public Component
    {
        char name[32];
    };
    ATTA_REGISTER_COMPONENT(NameComponent)
    template<> ComponentRegistry::Description& TypedComponentRegistry<NameComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_NAME_COMPONENT_H
