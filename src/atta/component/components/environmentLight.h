//--------------------------------------------------
// Atta Component Module
// environmentLightComponent.h
// Date: 2022-01-03
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H
#define ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H
#include <atta/component/componentManager.h>

namespace atta::component {
struct EnvironmentLight final : public Component {
    EnvironmentLightComponent();
    StringId sid; ///< Environment texture relative path
};
ATTA_REGISTER_COMPONENT(EnvironmentLightComponent)
template <>
ComponentDescription& TypedComponentRegistry<EnvironmentLightComponent>::getDescription();
} // namespace atta::component

#endif // ATTA_COMPONENT_COMPONENTS_ENVIRONMENT_LIGHT_COMPONENT_H
