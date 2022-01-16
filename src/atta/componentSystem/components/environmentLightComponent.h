//--------------------------------------------------
// Atta Component System
// environmentLightComponent.h
// Date: 2022-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_SYSTEM_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
    struct EnvironmentLightComponent final : public Component
    {
        EnvironmentLightComponent();
        StringId sid;///< Environment texture relative path
    };
    ATTA_REGISTER_COMPONENT(EnvironmentLightComponent)
    template<> ComponentDescription& TypedComponentRegistry<EnvironmentLightComponent>::getDescription();
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H